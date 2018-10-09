#include <pch.h>
#include <AudioLooper.h>

AudioLooper::AudioLooper(int bufferSize)
{
	this->bufferSize = bufferSize;
	channel = NULL;
	initialized = true;
	firstAudioEvent = false;
}

void AudioLooper::Init(AudioLooper* looper)
{

	uint32 sampleRate = 44100;
	DX::ThrowIfFailed(XAudio2Create(&_audioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR));
	DX::ThrowIfFailed(_audioEngine->CreateMasteringVoice(&_masterVoice, XAUDIO2_DEFAULT_CHANNELS, sampleRate));

	int channels = 2;
	_sampleBits = 32;
 
    // set up wave format using my good friend WAVEFORMATEX
    WAVEFORMATEX wfx;
    wfx.wBitsPerSample = _sampleBits;
    wfx.nAvgBytesPerSec = sampleRate * channels * _sampleBits / 8;
    wfx.nChannels = channels;
    wfx.nBlockAlign = channels * _sampleBits / 8;
    wfx.wFormatTag = WAVE_FORMAT_PCM; //WAVE_FORMAT_IEEE_FLOAT; // or could use WAVE_FORMAT_PCM
    wfx.nSamplesPerSec = sampleRate;
    wfx.cbSize = 0;    // set to zero for PCM or IEEE float

	DX::ThrowIfFailed(_audioEngine->CreateSourceVoice(&_voice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO,looper, NULL, NULL));

	//_buffer.AudioBytes = bufferSize;// VOICE_BUFFER_SAMPLE_COUNT * _sampleBits / 8;
	_buffer.Flags = 0;
    _buffer.PlayBegin = 0;
    _buffer.PlayLength = 0;    // play entire buffer
    _buffer.LoopBegin = 0;
    _buffer.LoopLength = 0;    // loop entire buffer
    _buffer.LoopCount = 0;//XAUDIO2_LOOP_INFINITE;
    //_buffer.pAudioData = (BYTE *)&_bufferData;
	_buffer.pAudioData = (BYTE *)&_bufferDatabyte;
    _buffer.pContext = NULL;

	//_callback->_buffer = &_buffer;
	//_callback->buffer = _bufferDatabyte;
	//_callback->_voice = _voice;

	//DX::ThrowIfFailed(_voice->FlushSourceBuffers());
	_voice->SetVolume(0.1);

	//channel->Generate(bufferSize * 2);
	//channel->GetBuffer();

	//DX::ThrowIfFailed(_voice->SubmitSourceBuffer(&_buffer));
	//DX::ThrowIfFailed(_voice->Start(0));
}

void AudioLooper::Start() 
{
	for(int i = 0;i<bufferSize * 2;i++) {
		_bufferDatabyte[i] = 0;
	}
	_buffer.AudioBytes = bufferSize * 2 * _sampleBits / 8;
	DX::ThrowIfFailed(_voice->SubmitSourceBuffer(&_buffer));
	DX::ThrowIfFailed(_voice->Start(0));
}

void AudioLooper::Stop()
{
	DX::ThrowIfFailed(_voice->FlushSourceBuffers());
	DX::ThrowIfFailed(_voice->Stop(0));
}

void AudioLooper::BufferEnded(UINT32 BytesRequired) 
{
	int i;
	channel->Generate(bufferSize * 2);
	short* lrIn = channel->GetBuffer();
	for(i = 0;i<bufferSize;i++) 
	{
		_bufferDatabyte[i * 2 + 0] = lrIn[i * 2 + 0] / 32768.0;
		_bufferDatabyte[i * 2 + 1] = lrIn[i * 2 + 1] / 32768.0;
	}
	DX::ThrowIfFailed(_voice->SubmitSourceBuffer(&_buffer));
	_ready = true;
}

void AudioLooper::SetChannel(BiquadFilterChannel* newChannel)
{ 
	newChannel->SetBufferLength(bufferSize * 2);
	channel = newChannel;
}

bool AudioLooper::IsActive()
{
	return false; // TODO
}

void AudioLooper::Activate()
{
	if(IsActive()) return;
	// TODO
}

AudioLooper::~AudioLooper(void)
{
}
