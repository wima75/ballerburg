#pragma once
class PsgDeviceChannel
{
public:
	PsgDeviceChannel(void);
	~PsgDeviceChannel(void);

	void SetClock(int hz);
	void SetMode(int newMode);
	void SetDevice(int target);
	void SetBufferLength(int length);
	short* GetBuffer();
	void Generate(int length);

	static const int CLOCK_4MHZ = 4000000;
	static const int CLOCK_3_58MHZ = 3579545;
	static const int MODE_UNSIGNED = 0;
	static const int MODE_SIGNED = 1;
	static const int DEVICE_PSG = 0;
	static const int DEVICE_SSG = 1;
	static const int DEVICE_AY_3_8910 = 0;
	static const int DEVICE_YM_2149 = 1;
	static const int DEVICE_SN76489 = 2;
	static const int REGISTER_AY_CH_A_TP_LOW = 0;
	static const int REGISTER_AY_CH_A_TP_HIGH = 1;
	static const int REGISTER_AY_CH_B_TP_LOW = 2;
	static const int REGISTER_AY_CH_B_TP_HIGH = 3;
	static const int REGISTER_AY_CH_C_TP_LOW = 4;
	static const int REGISTER_AY_CH_C_TP_HIGH = 5;
	static const int REGISTER_AY_NOISE_TP = 6;
	static const int REGISTER_AY_MIXER = 7;
	static const int REGISTER_AY_CH_A_VOLUME = 8;
	static const int REGISTER_AY_CH_B_VOLUME = 9;
	static const int REGISTER_AY_CH_C_VOLUME = 10;
	static const int REGISTER_AY_EP_LOW = 11;
	static const int REGISTER_AY_EP_HIGH = 12;
	static const int REGISTER_AY_EP_CONTROL = 13;
	static const int REGISTER_AY_IO_A = 14;
	static const int REGISTER_AY_IO_B = 15;
	static const int REGISTER_SN_CH_A_TP = 8;
	static const int REGISTER_SN_CH_A_VOLUME = 9;
	static const int REGISTER_SN_CH_B_TP = 10;
	static const int REGISTER_SN_CH_B_VOLUME = 11;
	static const int REGISTER_SN_CH_C_TP = 12;
	static const int REGISTER_SN_CH_C_VOLUME = 13;
	static const int REGISTER_SN_NOISE_CONTROL = 14;
	static const int REGISTER_SN_NOISE_VOLUME = 15;
	static const int REGISTER_SN_CH_A_TP_HIGH = 0;
	static const int REGISTER_SN_CH_B_TP_HIGH = 2;
	static const int REGISTER_SN_CH_C_TP_HIGH = 4;

	void WriteRegister(int address, int value);

private:
	void InitRegisterAY();
	void WriteRegisterAY(int address, int value);

	static const int DEFAULT_AY_CH_A_TP_LOW = 0x55;
	static const int DEFAULT_AY_CH_A_TP_HIGH = 0x00;
	static const int DEFAULT_AY_CH_B_TP_LOW = 0x00;
	static const int DEFAULT_AY_CH_B_TP_HIGH = 0x00;
	static const int DEFAULT_AY_CH_C_TP_LOW = 0x00;
	static const int DEFAULT_AY_CH_C_TP_HIGH = 0x00;
	static const int DEFAULT_AY_NOISE_TP = 0x00;
	static const int DEFAULT_AY_MIXER = 0xb8;
	static const int DEFAULT_AY_CH_A_VOLUME = 0x00;
	static const int DEFAULT_AY_CH_B_VOLUME = 0x00;
	static const int DEFAULT_AY_CH_C_VOLUME = 0x00;
	static const int DEFAULT_AY_EP_LOW = 0x0b;
	static const int DEFAULT_AY_EP_HIGH = 0x00;
	static const int DEFAULT_AY_EP_CONTROL = 0x00;
	static const int DEFAULT_SN_CH_A_TP = 0x00;
	static const int DEFAULT_SN_CH_A_VOLUME = 0x0f;
	static const int DEFAULT_SN_CH_B_TP = 0x00;
	static const int DEFAULT_SN_CH_B_VOLUME = 0x0f;
	static const int DEFAULT_SN_CH_C_TP = 0x00;
	static const int DEFAULT_SN_CH_C_VOLUME = 0x0f;
	static const int DEFAULT_SN_NOISE_CONTROL = 0xe0;
	static const int DEFAULT_SN_NOISE_VOLUME = 0xff;
	static const int DEFAULT_SN_CH_A_TP_HIGH = 0x00;
	static const int DEFAULT_SN_CH_B_TP_HIGH = 0x00;
	static const int DEFAULT_SN_CH_C_TP_HIGH = 0x00;
	static const int REGISTERS = 16;
	static const int CHANNELS = 3;
	static const int REGISTER_MIN_VALUE = 0;
	static const int REGISTER_MAX_VALUE = 255;
	static const int BITS_PER_BYTE = 8;
	static const int NOISE_TP_MASK = 0x1f;
	static const int MIXER_CH_A_TONE = 1;
	static const int MIXER_CH_A_NOISE = 8;
	static const int MIXER_CH_B_TONE = 2;
	static const int MIXER_CH_B_NOISE = 16;
	static const int MIXER_CH_C_TONE = 4;
	static const int MIXER_CH_C_NOISE = 32;
	static const int VOLUME_MASK = 0x0f;
	static const int ENVELOPE_MASK = 0x10;
	static const int CH_A = 0;
	static const int CH_B = 1;
	static const int CH_C = 2;
	static const int CLOCK_BIAS = 16000;
	static const int STEP_BIAS = 18;
	static const int VOLUME_BIAS = 3;
	static const short DEFAULT_AY_SEED = -1;
	static const short DEFAULT_SN_SEED = -32768;
	static const short UPDATE_SEED_MASK = 0x0009;
	static const int UPDATE_SEED_RSHIFT = 3;
	static const int UPDATE_SEED_LSHIFT = 15;
	static const int SHORT_MASK = 0xffff;
	static const int HALF_MASK = 0x0f;
	static const int HALF_SHIFT = 4;
	static const int BYTE_MSB_MASK = 0x80;
	static const int ADDRESS_MASK = 0x07;
	static const int VALUE_MASK = 0x3f;
	static const int LOWER_TWO_BITS_MASK = 0x03;

	static const short VOLUME_TABLE[3][32];

	int clock;
	int mode;
	int device;
	int activeRegister;
	static const short volumeTable[32];
	int baseStep;
	short* buffer;
	int _register[REGISTERS];
	short volume[CHANNELS];
	bool envelope[CHANNELS];
	bool active[CHANNELS];
	short seed;
	int stepTone[CHANNELS];
	int countTone[CHANNELS];
	bool mixerTone[CHANNELS];
	int stepNoise;
	int countNoise;
	bool mixerNoise[CHANNELS];
	bool feedback;
	int volumeNoise;

};
