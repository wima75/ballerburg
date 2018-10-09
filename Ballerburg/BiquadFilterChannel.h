#pragma once
#include <MasterChannel.h>

class BiquadFilterChannel
{
public:
	BiquadFilterChannel(void);
	~BiquadFilterChannel(void);

	void SetChannel(MasterChannel* newChannel);

	void SetBufferLength(int length);

	void Generate(int length);
	short* GetBuffer();

private:
	int bufferLength;
	MasterChannel* channel;

	short* outBuffer;
	short* inBuffer;

	int zL, zR, zzL, zzR;
	int a0, a1, a2, b0, b1, b2;
};
