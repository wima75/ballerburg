#pragma once
#include <pch.h>
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <AudioLooper.h>
#include <BiquadFilterChannel.h>

ref class Sound sealed {
public:
	Sound();
	void Stop();
	void Test(float f);
	void Test2();
	void Test3(float f);
	void sound1();
	void sound2();
	void Start();
	bool Ready();

	void Write(int address, int value);
	
private:
	int _sampleBits;
	IXAudio2MasteringVoice* _masterVoice;    // cannot use ComPtr for this
	IXAudio2* _audioEngine;

	//IXAudio2SourceVoice* _voice;
    XAUDIO2_BUFFER _buffer;
    //float _bufferData[VOICE_BUFFER_SAMPLE_COUNT];
	//BYTE _bufferDatabyte[VOICE_BUFFER_SAMPLE_COUNT];

	bool _started;

	UINT32 _pos;

	//Callback1* _callback;
	//VoiceCallback* _callback;

	AudioLooper* looper;
	BiquadFilterChannel* filter;

	PsgDeviceChannel* _psgDevice;




};