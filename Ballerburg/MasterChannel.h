#pragma once
#include <vector>
#include <PsgDeviceChannel.h>
using std::vector;
class MasterChannel
{
public:
	MasterChannel(void);
	~MasterChannel(void);

	static const int SAMPLE_FREQUENCY = 44100;
	static const int MAX_WAVE_VALUE = 32767;
	static const int MIN_WAVE_VALUE = -32767;
	static const int MSEC_PER_SEC = 1000;
	static const int DEFAULT_VOLUME = 16;
	
	void SetBufferLength(int length);
	void Generate(int length);
	bool AddChannel(PsgDeviceChannel* channel);
	short* GetBuffer();
private:
	void ReconstructBuffers();
	void _generate(int base, int length);
	int bufferLength;
	int intervalLength;
	int intervalRestLength;
	int volume;
	vector<PsgDeviceChannel*> channels;
	short** buffers;
	short* buffer;
};
