#include "pch.h"
#include <MasterChannel.h>
#include <PsgDeviceChannel.h>

MasterChannel::MasterChannel(void)
{
	bufferLength = 0;
	intervalLength = 0;
	intervalRestLength = 0;
	volume = DEFAULT_VOLUME;
}

void MasterChannel::SetBufferLength(int length) 
{
	//buffers.clear();
	buffers = NULL;
	buffer = new short[length];
	bufferLength = length;
	for(int i = 0;i<channels.size();i++)
	{
		channels[i]->SetBufferLength(length);
	}
	ReconstructBuffers();
}

void MasterChannel::Generate(int length)
{
	//if(buffers.empty()) return;
	if(buffers == NULL) return;
	_generate(0, length);
}

void MasterChannel::_generate(int base, int length) 
{
	int channels = this->channels.size();
	int ch;
	for(ch = 0;ch<channels;ch++)
	{
		this->channels[ch]->Generate(length);
	}
	for(int offset = 0;offset<length;offset++)
	{
		int value = 0;
		for(ch = 0;ch < channels;ch++) 
		{
			value += (int)buffers[ch][offset];
		}
		value *= volume;
		if(value > MAX_WAVE_VALUE)
		{
			value = MAX_WAVE_VALUE;
		}
		else if(value < MIN_WAVE_VALUE)
		{
			value = MIN_WAVE_VALUE;
		}
		buffer[base + offset] = (short) value;
	}
}

short* MasterChannel::GetBuffer() 
{
	return buffer;
}

bool MasterChannel::AddChannel(PsgDeviceChannel* channel) 
{
	if(bufferLength != 0)
	{
		channel->SetBufferLength(bufferLength);
		ReconstructBuffers();
	}
	channels.push_back(channel);
	return true;
}

void MasterChannel::ReconstructBuffers()
{
	//vector<short*> newBuffers(channels.size());
	short** newBuffers;
	newBuffers = new short*[channels.size()];
	for(int i = 0;i<channels.size();i++)
	{
		//newBuffers.push_back(channels[i]->GetBuffer());
		newBuffers[i] = channels[i]->GetBuffer();
	}
	buffers = newBuffers;
}

MasterChannel::~MasterChannel(void)
{
}
