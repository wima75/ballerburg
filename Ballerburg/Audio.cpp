//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "Audio.h"

void AudioEngineCallbacks::Initialize(Audio* audio)
{
    m_audio = audio;
}

// Called when a critical system error causes XAudio2
// to be closed and restarted. The error code is given in Error.
void  _stdcall AudioEngineCallbacks::OnCriticalError(HRESULT Error)
{
    m_audio->SetEngineExperiencedCriticalError();
}

Audio::~Audio()
{
    SuspendAudio();
}

void Audio::Initialize()
{
	bufferSize = 512;

    m_isAudioStarted = false;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
	_flugEngine = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_musicSourceVoice = nullptr;
	_flugMasteringVoice = nullptr;
	_flugSourceVoice = nullptr;

    for (int i = 0; i < ARRAYSIZE(m_soundEffects); i++)
    {
        m_soundEffects[i].m_soundEffectBufferData = nullptr;
        m_soundEffects[i].m_soundEffectSourceVoice = nullptr;
        m_soundEffects[i].m_soundEffectStarted = false;
        ZeroMemory(&m_soundEffects[i].m_audioBuffer, sizeof(m_soundEffects[i].m_audioBuffer));
    }
    ZeroMemory(m_audioBuffers, sizeof(m_audioBuffers));
	ZeroMemory(_flugBufferDatabyte, sizeof(_flugBufferDatabyte));
}


void bufferEndCallback(Audio* audio)
{
	int i;
	audio->channel->Generate(audio->bufferSize * 2);
	short* lrIn = audio->channel->GetBuffer();
	for(i = 0;i<audio->bufferSize;i++) 
	{
		audio->_flugBufferDatabyte[i * 2 + 0] = lrIn[i * 2 + 0] / 32768.0;
		audio->_flugBufferDatabyte[i * 2 + 1] = lrIn[i * 2 + 1] / 32768.0;
	}
	DX::ThrowIfFailed(audio->_flugSourceVoice->SubmitSourceBuffer(&audio->_flugBuffer));
}

void Audio::CreateResources()
{
    try
    {
        // Media Foundation is a convenient way to get both file I/O and format decode for
        // audio assets. You can replace the streamer in this sample with your own file I/O
        // and decode routines.
        m_musicStreamer.Initialize(L"Assets\\Audio\\background.wma");

        DX::ThrowIfFailed(
            XAudio2Create(&m_musicEngine)
            );

		DX::ThrowIfFailed(
            XAudio2Create(&_flugEngine)
            );

#if defined(_DEBUG)
        XAUDIO2_DEBUG_CONFIGURATION debugConfig = {0};
        debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
        debugConfig.TraceMask = XAUDIO2_LOG_ERRORS;
        m_musicEngine->SetDebugConfiguration(&debugConfig);
#endif

        m_musicEngineCallback.Initialize(this);
        m_musicEngine->RegisterForCallbacks(&m_musicEngineCallback);

		_flugEngineCallback.Initialize(this);
		_flugEngine->RegisterForCallbacks(&_flugEngineCallback);

        // This sample plays the equivalent of background music, which we tag on the
        // mastering voice as AudioCategory_GameMedia. In ordinary usage, if we were
        // playing the music track with no effects, we could route it entirely through
        // Media Foundation. Here we are using XAudio2 to apply a reverb effect to the
        // music, so we use Media Foundation to decode the data then we feed it through
        // the XAudio2 pipeline as a separate Mastering Voice, so that we can tag it
        // as Game Media. We default the mastering voice to 2 channels to simplify
        // the reverb logic.
        DX::ThrowIfFailed(
            m_musicEngine->CreateMasteringVoice(
                &m_musicMasteringVoice,
                2,
                44100,
                0,
                nullptr,
                nullptr,
                AudioCategory_GameMedia
                )
        );

		DX::ThrowIfFailed(
            _flugEngine->CreateMasteringVoice(
                &_flugMasteringVoice,
                2,
                44100,
                0,
                nullptr,
                nullptr,
                AudioCategory_GameMedia
                )
        );

        XAUDIO2_SEND_DESCRIPTOR descriptors[1];
        descriptors[0].pOutputVoice = m_musicMasteringVoice;
        descriptors[0].Flags = 0;
        XAUDIO2_VOICE_SENDS sends = {0};
        sends.SendCount = 1;
        sends.pSends = descriptors;
        WAVEFORMATEX& waveFormat = m_musicStreamer.GetOutputWaveFormatEx();

        DX::ThrowIfFailed(
            m_musicEngine->CreateSourceVoice(&m_musicSourceVoice, &waveFormat, 0, 1.0f, &m_voiceContext, &sends, nullptr)
            );

        DX::ThrowIfFailed(
            m_musicMasteringVoice->SetVolume(0.4f)
            );


		uint32 sampleRate = 44100;
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
		_flugContext.bufferEndCallback = &bufferEndCallback;
		_flugContext._audio = this;
		DX::ThrowIfFailed(_flugEngine->CreateSourceVoice(&_flugSourceVoice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO,&_flugContext, NULL, NULL));

		DX::ThrowIfFailed(
            _flugMasteringVoice->SetVolume(0.4f)
            );

        DX::ThrowIfFailed(
            _flugSourceVoice->SetVolume(0.4f)
            );


		_flugBuffer.Flags = 0;
		_flugBuffer.PlayBegin = 0;
		_flugBuffer.PlayLength = 0;    // play entire buffer
		_flugBuffer.LoopBegin = 0;
		_flugBuffer.LoopLength = 0;    // loop entire buffer
		_flugBuffer.LoopCount = 0;//XAUDIO2_LOOP_INFINITE;
		//_buffer.pAudioData = (BYTE *)&_bufferData;
		_flugBuffer.pAudioData = (BYTE *)&_flugBufferDatabyte;
		_flugBuffer.pContext = NULL;


        // Create a separate engine and mastering voice for sound effects in the sample
        // Games will use many voices in a complex graph for audio, mixing all effects down to a
        // single mastering voice.
        // We are creating an entirely new engine instance and mastering voice in order to tag
        // our sound effects with the audio category AudioCategory_GameEffects.
        DX::ThrowIfFailed(
            XAudio2Create(&m_soundEffectEngine)
            );

        m_soundEffectEngineCallback.Initialize(this);
        m_soundEffectEngine->RegisterForCallbacks(&m_soundEffectEngineCallback);

        // We default the mastering voice to 2 channels to simplify the reverb logic.
        DX::ThrowIfFailed(
            m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice, 2, 44100, 0, nullptr, nullptr, AudioCategory_GameEffects)
            );

        CreateSourceVoice(Baller0Event);
        CreateSourceVoice(Baller1Event);
        CreateSourceVoice(Baller2Event);
        CreateSourceVoice(Baller3Event);
        CreateSourceVoice(Baller4Event);
        CreateSourceVoice(Baller5Event);
        CreateSourceVoice(Baller6Event);
        CreateSourceVoice(Baller7Event);
        CreateSourceVoice(Baller8Event);
        CreateSourceVoice(Baller9Event);
        CreateSourceVoice(Baller10Event);
        CreateSourceVoice(Baller11Event);
        CreateSourceVoice(Baller12Event);
        CreateSourceVoice(Baller13Event);
        CreateSourceVoice(Baller14Event);
        CreateSourceVoice(Baller15Event);
        CreateSourceVoice(Baller16Event);
        CreateSourceVoice(Baller17Event);
        CreateSourceVoice(Baller18Event);
        CreateSourceVoice(Baller19Event);
        CreateSourceVoice(Baller20Event);
        CreateSourceVoice(Baller21Event);
        CreateSourceVoice(Baller22Event);
        CreateSourceVoice(Baller23Event);
        CreateSourceVoice(Baller24Event);
        CreateSourceVoice(Baller25Event);
        CreateSourceVoice(Baller26Event);
        CreateSourceVoice(Baller27Event);
        CreateSourceVoice(Baller28Event);
        CreateSourceVoice(Baller29Event);
        CreateSourceVoice(Baller30Event);
        CreateSourceVoice(Baller31Event);
		CreateSourceVoice(BingEvent);
		CreateSourceVoice(EndeEvent);
    }
    catch (...)
    {
        m_engineExperiencedCriticalError = true;
    }
}


void Audio::CreateSourceVoice(SoundEvent sound)
{
    // Load all data for each sound effect into a single in-memory buffer
    MediaStreamer soundEffectStream;
    switch (sound)
    {
        case Baller0Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller0.wav"); break;
		case Baller1Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller1.wav"); break;
		case Baller2Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller2.wav"); break;
		case Baller3Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller3.wav"); break;
		case Baller4Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller4.wav"); break;
		case Baller5Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller5.wav"); break;
		case Baller6Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller6.wav"); break;
		case Baller7Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller7.wav"); break;
		case Baller8Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller8.wav"); break;
		case Baller9Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller9.wav"); break;
		case Baller10Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller10.wav"); break;
		case Baller11Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller11.wav"); break;
		case Baller12Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller12.wav"); break;
		case Baller13Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller13.wav"); break;
		case Baller14Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller14.wav"); break;
		case Baller15Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller15.wav"); break;
		case Baller16Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller16.wav"); break;
		case Baller17Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller17.wav"); break;
		case Baller18Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller18.wav"); break;
		case Baller19Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller19.wav"); break;
		case Baller20Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller20.wav"); break;
		case Baller21Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller21.wav"); break;
		case Baller22Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller22.wav"); break;
		case Baller23Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller23.wav"); break;
		case Baller24Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller24.wav"); break;
		case Baller25Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller25.wav"); break;
		case Baller26Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller26.wav"); break;
		case Baller27Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller27.wav"); break;
		case Baller28Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller28.wav"); break;
		case Baller29Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller29.wav"); break;
		case Baller30Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller30.wav"); break;
		case Baller31Event: soundEffectStream.Initialize(L"Assets\\Audio\\baller31.wav"); break;
		case BingEvent: soundEffectStream.Initialize(L"Assets\\Audio\\bing.wav"); break;
		case EndeEvent: soundEffectStream.Initialize(L"Assets\\Audio\\ende.wav"); break;
    }
    m_soundEffects[sound].m_soundEventType = sound;

    uint32 bufferLength = soundEffectStream.GetMaxStreamLengthInBytes();
    m_soundEffects[sound].m_soundEffectBufferData = new byte[bufferLength];
    soundEffectStream.ReadAll(m_soundEffects[sound].m_soundEffectBufferData, bufferLength, &m_soundEffects[sound].m_soundEffectBufferLength);

    XAUDIO2_SEND_DESCRIPTOR descriptors[1];
    descriptors[0].pOutputVoice = m_soundEffectMasteringVoice;
    descriptors[0].Flags = 0;
    XAUDIO2_VOICE_SENDS sends = {0};
    sends.SendCount = 1;
    sends.pSends = descriptors;


        DX::ThrowIfFailed(
            m_soundEffectEngine->CreateSourceVoice(
                &m_soundEffects[sound].m_soundEffectSourceVoice,
                &(soundEffectStream.GetOutputWaveFormatEx()),
                0,
                1.0f,
                &m_voiceContext,
                &sends)
            );
    
    m_soundEffects[sound].m_soundEffectSampleRate = soundEffectStream.GetOutputWaveFormatEx().nSamplesPerSec;

    // Queue in-memory buffer for playback
    ZeroMemory(&m_soundEffects[sound].m_audioBuffer, sizeof(m_soundEffects[sound].m_audioBuffer));

    m_soundEffects[sound].m_audioBuffer.AudioBytes = m_soundEffects[sound].m_soundEffectBufferLength;
    m_soundEffects[sound].m_audioBuffer.pAudioData = m_soundEffects[sound].m_soundEffectBufferData;
    m_soundEffects[sound].m_audioBuffer.pContext = &m_soundEffects[sound];
    m_soundEffects[sound].m_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

    m_soundEffects[sound].m_audioBuffer.LoopCount = 0;
    

    DX::ThrowIfFailed(
        m_soundEffects[sound].m_soundEffectSourceVoice->SubmitSourceBuffer(&m_soundEffects[sound].m_audioBuffer)
        );
}

void Audio::ReleaseResources()
{
    if (m_musicSourceVoice != nullptr)
    {
        m_musicSourceVoice->DestroyVoice();
    }
	if(_flugSourceVoice != nullptr)
	{
		_flugSourceVoice->DestroyVoice();
	}
    for (int i = 0; i < ARRAYSIZE(m_soundEffects); i++)
    {
        if (m_soundEffects[i].m_soundEffectSourceVoice != nullptr)
        {
            m_soundEffects[i].m_soundEffectSourceVoice->DestroyVoice();
        }
        m_soundEffects[i].m_soundEffectSourceVoice = nullptr;
    }
    if (m_musicMasteringVoice != nullptr)
    {
            m_musicMasteringVoice->DestroyVoice();
    }
	if (_flugMasteringVoice != nullptr)
    {
            _flugMasteringVoice->DestroyVoice();
    }
    if (m_soundEffectMasteringVoice != nullptr)
    {
        m_soundEffectMasteringVoice->DestroyVoice();
    }

    m_musicSourceVoice = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
	_flugSourceVoice = nullptr;
	_flugMasteringVoice = nullptr;
	_flugEngine = nullptr;
}

void Audio::Start()
{
    if (m_engineExperiencedCriticalError)
    {
        return;
    }

	for(int i = 0;i<bufferSize * 2;i++) {
		_flugBufferDatabyte[i] = 0;
	}
	_flugBuffer.AudioBytes = bufferSize * 2 * _sampleBits / 8;
	DX::ThrowIfFailed(_flugSourceVoice->SubmitSourceBuffer(&_flugBuffer));
	DX::ThrowIfFailed(_flugSourceVoice->Start(0));
	
	// Musik starten
    /*HRESULT hr = m_musicSourceVoice->Start(0);

    if SUCCEEDED(hr) {
        m_isAudioStarted = true;
    }
    else
    {
        m_engineExperiencedCriticalError = true;
    }*/
}

// This sample processes audio buffers during the render cycle of the application.
// As long as the sample maintains a high-enough frame rate, this approach should
// not glitch audio. In game code, it is best for audio buffers to be processed
// on a separate thread that is not synced to the main render loop of the game.
void Audio::Render()
{
    if (m_engineExperiencedCriticalError)
    {
        m_engineExperiencedCriticalError = false;
        ReleaseResources();
        Initialize();
        CreateResources();
        Start();
        if (m_engineExperiencedCriticalError)
        {
            return;
        }
    }

    try
    {
        bool streamComplete;
        XAUDIO2_VOICE_STATE state;
        uint32 bufferLength;
        XAUDIO2_BUFFER buf = {0};

        // Use MediaStreamer to stream the buffers.
        m_musicSourceVoice->GetState(&state);
        while (state.BuffersQueued <= MAX_BUFFER_COUNT - 1)
        {
            streamComplete = m_musicStreamer.GetNextBuffer(
                m_audioBuffers[m_currentBuffer],
                STREAMING_BUFFER_SIZE,
                &bufferLength
                );

            if (bufferLength > 0)
            {
                buf.AudioBytes = bufferLength;
                buf.pAudioData = m_audioBuffers[m_currentBuffer];
                buf.Flags = (streamComplete) ? XAUDIO2_END_OF_STREAM : 0;
                buf.pContext = 0;
                DX::ThrowIfFailed(
                    m_musicSourceVoice->SubmitSourceBuffer(&buf)
                    );

                m_currentBuffer++;
                m_currentBuffer %= MAX_BUFFER_COUNT;
            }

            if (streamComplete)
            {
                // Loop the stream.
                m_musicStreamer.Restart();
                break;
            }

            m_musicSourceVoice->GetState(&state);
        }
    }
    catch (...)
    {
        m_engineExperiencedCriticalError = true;
    }
}

void Audio::PlaySoundEffect(SoundEvent sound)
{
    XAUDIO2_BUFFER buf = {0};
    XAUDIO2_VOICE_STATE state = {0};

	for(int i = 0;i<SOUND_EVENTS;i++) {
		if(m_soundEffects[i].m_soundEffectStarted) {
			HRESULT hr = m_soundEffects[i].m_soundEffectSourceVoice->Stop();
			m_soundEffects[i].m_soundEffectStarted = false;
		}
	}

    if (m_engineExperiencedCriticalError) {
        // If there's an error, then we'll recreate the engine on the next
        // render pass.
        return;
    }

    SoundEffectData* soundEffect = &m_soundEffects[sound];
    HRESULT hr = soundEffect->m_soundEffectSourceVoice->Start();
    if FAILED(hr)
    {
        m_engineExperiencedCriticalError = true;
        return;
    }

	{

        XAUDIO2_VOICE_STATE state = {0};
        soundEffect->m_soundEffectSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
        if (state.BuffersQueued == 0)
        {
            soundEffect->m_soundEffectSourceVoice->SubmitSourceBuffer(&soundEffect->m_audioBuffer);
        }
        else if (state.BuffersQueued < 1 /*&& sound == Baller0Event*/)
        {
            soundEffect->m_soundEffectSourceVoice->SubmitSourceBuffer(&soundEffect->m_audioBuffer);
        }


    }

    m_soundEffects[sound].m_soundEffectStarted = true;
}

void Audio::StopSoundEffect(SoundEvent sound)
{
    if (m_engineExperiencedCriticalError) {
        return;
    }

    HRESULT hr = m_soundEffects[sound].m_soundEffectSourceVoice->Stop();
    if FAILED(hr)
    {
        // If there's an error, then we'll recreate the engine on the next render pass
        m_engineExperiencedCriticalError = true;
        return;
    }

    m_soundEffects[sound].m_soundEffectStarted = false;
}

bool Audio::IsSoundEffectStarted(SoundEvent sound)
{
    return m_soundEffects[sound].m_soundEffectStarted;
}

void Audio::SetSoundEffectVolume(SoundEvent sound, float volume)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        m_soundEffects[sound].m_soundEffectSourceVoice->SetVolume(volume);
    }
}

void Audio::SetSoundEffectPitch(SoundEvent sound, float pitch)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        m_soundEffects[sound].m_soundEffectSourceVoice->SetFrequencyRatio(pitch);
    }
}

void Audio::SetSoundEffectFilter(SoundEvent sound, float frequency, float oneOverQ)
{
    if (m_soundEffects[sound].m_soundEffectSourceVoice != nullptr) {
        if (oneOverQ <= 0.1f)
        {
            oneOverQ = 0.1f;
        }
        if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ)
        {
            oneOverQ = XAUDIO2_MAX_FILTER_ONEOVERQ;
        }
        XAUDIO2_FILTER_PARAMETERS params = {LowPassFilter, XAudio2CutoffFrequencyToRadians(frequency, m_soundEffects[sound].m_soundEffectSampleRate), oneOverQ};

        m_soundEffects[sound].m_soundEffectSourceVoice->SetFilterParameters(&params);
    }
}

// Uses the IXAudio2::StopEngine method to stop all audio immediately.
// It leaves the audio graph untouched, which preserves all effect parameters
// and effect histories (like reverb effects) voice states, pending buffers,
// cursor positions and so on.
// When the engines are restarted, the resulting audio will sound as if it had
// never been stopped except for the period of silence.
void Audio::SuspendAudio()
{
    if (m_engineExperiencedCriticalError)
    {
        return;
    }

    if (m_isAudioStarted)
    {
        m_musicEngine->StopEngine();
        m_soundEffectEngine->StopEngine();
		_flugEngine->StopEngine();
    }
    m_isAudioStarted = false;
}

// Restarts the audio streams. A call to this method must match a previous call
// to SuspendAudio. This method causes audio to continue where it left off.
// If there is a problem with the restart, the m_engineExperiencedCriticalError
// flag is set. The next call to Render will recreate all the resources and
// reset the audio pipeline.
void Audio::ResumeAudio()
{
    if (m_engineExperiencedCriticalError)
    {
        return;
    }

    HRESULT hr = m_musicEngine->StartEngine();
    HRESULT hr2 = m_soundEffectEngine->StartEngine();
	HRESULT hr3 = _flugEngine->StartEngine();

    if (FAILED(hr) || FAILED(hr2) || FAILED(hr3))
    {
        m_engineExperiencedCriticalError = true;
    }
}

void Audio::SetChannel(BiquadFilterChannel* newChannel)
{ 
	newChannel->SetBufferLength(bufferSize * 2);
	channel = newChannel;
}