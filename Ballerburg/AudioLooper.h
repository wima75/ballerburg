#pragma once
#include <pch.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <BiquadFilterChannel.h>

class AudioLooper : public IXAudio2VoiceCallback
{
public:
	AudioLooper(int bufferSize);
	~AudioLooper(void);

	void SetChannel(BiquadFilterChannel* newChannel);
	bool IsActive();
	void Activate();
	void Start();

	void Init(AudioLooper* looper);

	void Stop();
		bool _ready;

private:
	int bufferSize;
	int _sampleBits;
	BiquadFilterChannel* channel;
	bool initialized;
	bool firstAudioEvent;

	XAUDIO2_BUFFER _buffer;

	IXAudio2* _audioEngine;
	IXAudio2MasteringVoice* _masterVoice;
	IXAudio2SourceVoice* _voice;
	float _bufferDatabyte[1024];

	void BufferEnded(UINT32 BytesRequired);

	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext)
	{
	}
	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 BytesRequired)
	{
		//Test(BytesRequired);
	}
	STDMETHOD_(void, OnStreamEnd)() { }; 
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() { } 
	STDMETHOD_(void, OnBufferEnd)(void * pBufferContext)
	{ 
		BufferEnded(0);
	}
	STDMETHOD_(void, OnLoopEnd)(void * pBufferContext) {    } 
	STDMETHOD_(void, OnVoiceError)(void * pBufferContext, HRESULT Error) { }
};
