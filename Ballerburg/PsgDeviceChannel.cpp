#include "pch.h"
#include "PsgDeviceChannel.h"
#include "MasterChannel.h"


PsgDeviceChannel::PsgDeviceChannel(void)
{
	clock = CLOCK_3_58MHZ;
	mode = MODE_UNSIGNED;
	device = DEVICE_AY_3_8910;
	activeRegister = 0;
	baseStep = 0;
	//buffer = NULL;
	seed = DEFAULT_AY_SEED;
	stepNoise = 0;
	countNoise = 0;
	feedback = false;
	volumeNoise = 0;

	SetClock(CLOCK_3_58MHZ);
	SetMode(MODE_UNSIGNED);
	SetDevice(DEVICE_AY_3_8910);
}
void PsgDeviceChannel::SetMode(int newMode)
{
	mode = newMode;
}

void PsgDeviceChannel::SetDevice(int target)
{
	device = target;

	for (int i = 0; i < PsgDeviceChannel::CHANNELS; i++) {
		active[i] = true;
		countTone[i] = 0;
	}
	InitRegisterAY();
}

void PsgDeviceChannel::SetBufferLength(int length)
{
	buffer = new short[length];
}

short* PsgDeviceChannel::GetBuffer() 
{
	return buffer;
}

void PsgDeviceChannel::Generate(int length)
{
	for(int offset = 0;offset < length; offset += 2)
	{
		countNoise += baseStep;
		if(countNoise > stepNoise)
		{
			short v = seed & UPDATE_SEED_MASK;
			v ^= (short)(((unsigned short)v) >> UPDATE_SEED_RSHIFT);
			seed = (short) (((int) seed & SHORT_MASK) >> 1);
			seed |= ((v << UPDATE_SEED_LSHIFT) & SHORT_MASK);
			countNoise -= stepNoise;
		}
		short value = 0;
		bool noise = 0 != (seed & 1);
		for (int channel = 0; channel < CHANNELS; channel++)
		{
			countTone[channel] += baseStep;
			if (countTone[channel] > stepTone[channel]) {
				countTone[channel] -= stepTone[channel];
				active[channel] = !active[channel];
			}
			if ((mixerTone[channel] && active[channel])
				|| (mixerNoise[channel] && noise)) {
					value += volume[channel];
			} else if (mixerTone[channel]
			&& mixerNoise[channel]
			&& mode == MODE_SIGNED) {
				value -= volume[channel];
			}
		}
		buffer[offset + 0] = value;
		buffer[offset + 1] = value;
	}
}

void PsgDeviceChannel::InitRegisterAY()
{
	WriteRegister(REGISTER_AY_CH_A_TP_HIGH, DEFAULT_AY_CH_A_TP_HIGH);
	WriteRegister(REGISTER_AY_CH_A_TP_LOW, DEFAULT_AY_CH_A_TP_LOW);
	WriteRegister(REGISTER_AY_CH_B_TP_HIGH, DEFAULT_AY_CH_B_TP_HIGH);
	WriteRegister(REGISTER_AY_CH_B_TP_LOW, DEFAULT_AY_CH_B_TP_LOW);
	WriteRegister(REGISTER_AY_CH_C_TP_HIGH, DEFAULT_AY_CH_C_TP_HIGH);
	WriteRegister(REGISTER_AY_CH_C_TP_LOW, DEFAULT_AY_CH_C_TP_LOW);
	WriteRegister(REGISTER_AY_NOISE_TP, DEFAULT_AY_NOISE_TP);
	WriteRegister(REGISTER_AY_MIXER, DEFAULT_AY_MIXER);
	WriteRegister(REGISTER_AY_CH_A_VOLUME, DEFAULT_AY_CH_A_VOLUME);
	WriteRegister(REGISTER_AY_CH_B_VOLUME, DEFAULT_AY_CH_B_VOLUME);
	WriteRegister(REGISTER_AY_CH_C_VOLUME, DEFAULT_AY_CH_C_VOLUME);
	WriteRegister(REGISTER_AY_EP_LOW, DEFAULT_AY_EP_LOW);
	WriteRegister(REGISTER_AY_EP_HIGH, DEFAULT_AY_EP_HIGH);
	WriteRegister(REGISTER_AY_EP_CONTROL, DEFAULT_AY_EP_CONTROL);
	seed = DEFAULT_AY_SEED;
}

void PsgDeviceChannel::WriteRegister(int address, int value)
{
	WriteRegisterAY(address, value);
}

void PsgDeviceChannel::WriteRegisterAY(int address, int value)
{
	if ((address > REGISTERS)
		|| (value < REGISTER_MIN_VALUE)
		|| (REGISTER_MAX_VALUE < value)) {
			throw ref new Platform::InvalidArgumentException("Undefined register: " + address);
	}
	_register[address] = value;
	switch(address)
	{
	case REGISTER_AY_CH_A_TP_LOW:
	case REGISTER_AY_CH_A_TP_HIGH:
		stepTone[CH_A] = ((_register[REGISTER_AY_CH_A_TP_HIGH]
		<< BITS_PER_BYTE)
			| (_register[REGISTER_AY_CH_A_TP_LOW]))
			<< STEP_BIAS;
		break;
	case REGISTER_AY_CH_B_TP_LOW:
	case REGISTER_AY_CH_B_TP_HIGH:
		stepTone[CH_B] = ((_register[REGISTER_AY_CH_B_TP_HIGH]
		<< BITS_PER_BYTE)
			| (_register[REGISTER_AY_CH_B_TP_LOW]))
			<< STEP_BIAS;
		break;
	case REGISTER_AY_CH_C_TP_LOW:
	case REGISTER_AY_CH_C_TP_HIGH:
		stepTone[CH_C] = ((_register[REGISTER_AY_CH_C_TP_HIGH]
		<< BITS_PER_BYTE)
			| (_register[REGISTER_AY_CH_C_TP_LOW]))
			<< STEP_BIAS;
		break;
	case REGISTER_AY_NOISE_TP:
		stepNoise = ((value & NOISE_TP_MASK) << 1) << (STEP_BIAS + 1);
		if (stepNoise < baseStep) {
			stepNoise = baseStep;
		}
		break;
	case REGISTER_AY_MIXER:
		mixerTone[CH_A] = 0 == (value & MIXER_CH_A_TONE);
		mixerTone[CH_B] = 0 == (value & MIXER_CH_B_TONE);
		mixerTone[CH_C] = 0 == (value & MIXER_CH_C_TONE);
		mixerNoise[CH_A] = 0 == (value & MIXER_CH_A_NOISE);
		mixerNoise[CH_B] = 0 == (value & MIXER_CH_B_NOISE);
		mixerNoise[CH_C] = 0 == (value & MIXER_CH_C_NOISE);
		break;
	case REGISTER_AY_CH_A_VOLUME:
		volume[CH_A] = (short) (volumeTable[(value & VOLUME_MASK) << 1]
		<< VOLUME_BIAS);
		envelope[CH_A] = 0 != (value & ENVELOPE_MASK);
		break;
	case REGISTER_AY_CH_B_VOLUME:
		volume[CH_B] = (short) (volumeTable[(value & VOLUME_MASK) << 1]
		<< VOLUME_BIAS);
		envelope[CH_B] = 0 != (value & ENVELOPE_MASK);
		break;
	case REGISTER_AY_CH_C_VOLUME:
		volume[CH_C] = (short) (volumeTable[(value & VOLUME_MASK) << 1]
		<< VOLUME_BIAS);
		envelope[CH_C] = 0 != (value & ENVELOPE_MASK);
		break;
	case REGISTER_AY_EP_LOW:
		// TODO
		break;
	case REGISTER_AY_EP_HIGH:
		// TODO
		break;
	case REGISTER_AY_EP_CONTROL:
		// TODO
		break;
	case REGISTER_AY_IO_A:
		break;
	case REGISTER_AY_IO_B:
		break;
	default:
		break;
	}
}

void PsgDeviceChannel::SetClock(int hz) 
{
	clock = hz;
	baseStep = (int) (unsigned long long) CLOCK_BIAS * (unsigned long long) clock / (unsigned long long) MasterChannel::SAMPLE_FREQUENCY;
}

PsgDeviceChannel::~PsgDeviceChannel(void)
{
}

const short PsgDeviceChannel::volumeTable[32] = { // DEVICE_AY_3_8910
	0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04,
	0x05, 0x06, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x12,
	0x16, 0x1A, 0x1F, 0x25, 0x2D, 0x35, 0x3F, 0x4C,
	0x5A, 0x6A, 0x7F, 0x97, 0xB4, 0xD6, 0xFF, 0xFF,
};

const short PsgDeviceChannel::VOLUME_TABLE[3][32] = {
	{ // DEVICE_AY_3_8910
		0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04,
			0x05, 0x06, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x12,
			0x16, 0x1A, 0x1F, 0x25, 0x2D, 0x35, 0x3F, 0x4C,
			0x5A, 0x6A, 0x7F, 0x97, 0xB4, 0xD6, 0xFF, 0xFF,
	},
	{ // DEVICE_YM_2149
		0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04,
			0x05, 0x06, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x12,
			0x16, 0x1A, 0x1F, 0x25, 0x2D, 0x35, 0x3F, 0x4C,
			0x5A, 0x6A, 0x7F, 0x97, 0xB4, 0xD6, 0xFF, 0xFF,
		},
		{ // DEVICE_SN76489
			0xFF, 0xCB, 0xA1, 0x80, 0x65, 0x50, 0x40, 0x33,
				0x28, 0x20, 0x19, 0x14, 0x10, 0x0C, 0x0A, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
};
