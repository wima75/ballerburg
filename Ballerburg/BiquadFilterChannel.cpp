#include "pch.h"
#include "BiquadFilterChannel.h"

BiquadFilterChannel::BiquadFilterChannel(void)
{
	bufferLength = 0;
	zL = 0;
	zR = 0;
	zzL = 0;
	zzR = 0;
	a0 = 1;
	a1 = 0;
	a2 = 0;
	b0 = 1;
	b1 = 0;
	b2 = 0;
}

void BiquadFilterChannel::SetBufferLength(int length)
{
	outBuffer = new short[length];
	bufferLength = length;
	if(channel != NULL)
	{
		channel->SetBufferLength(length);
		inBuffer = channel->GetBuffer();
	}
}

void BiquadFilterChannel::Generate(int length)
{
	channel->Generate(length);
	for(int i = 0;i<length;i+=2) 
	{
		int L = inBuffer[i + 0] - a1 * zL - a2 * zzL;
		int R = inBuffer[i + 1] - a1 * zR - a2 * zzR;
		outBuffer[i + 0] =
                b0 * L + b1 * zL + b2 * zzL;
        outBuffer[i + 1] =
                b0 * R + b1 * zR + b2 * zzR;
        zzL = zL;
        zzR = zR;
        zL = L;
        zR = R;
	}
}

short* BiquadFilterChannel::GetBuffer()
{
	return outBuffer;
}

void BiquadFilterChannel::SetChannel(MasterChannel* channel)
{ 
	channel->SetBufferLength(bufferLength);
	inBuffer = channel->GetBuffer();
	this->channel = channel;
}

BiquadFilterChannel::~BiquadFilterChannel(void)
{
}
