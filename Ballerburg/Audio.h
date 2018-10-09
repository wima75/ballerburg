//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "MediaStreamer.h"

static const int STREAMING_BUFFER_SIZE = 65536;
static const int MAX_BUFFER_COUNT = 3;

enum SoundEvent
{
    Baller0Event = 0,
    Baller1Event = 1,
    Baller2Event = 2,
    Baller3Event = 3,
    Baller4Event = 4,
    Baller5Event = 5,
    Baller6Event = 6,
    Baller7Event = 7,
    Baller8Event = 8,
    Baller9Event = 9,
    Baller10Event = 10,
    Baller11Event = 11,
    Baller12Event = 12,
    Baller13Event = 13,
    Baller14Event = 14,
    Baller15Event = 15,
    Baller16Event = 16,
    Baller17Event = 17,
    Baller18Event = 18,
    Baller19Event = 19,
    Baller20Event = 20,
    Baller21Event = 21,
    Baller22Event = 22,
    Baller23Event = 23,
    Baller24Event = 24,
    Baller25Event = 25,
    Baller26Event = 26,
    Baller27Event = 27,
    Baller28Event = 28,
    Baller29Event = 29,
    Baller30Event = 30,
    Baller31Event = 31,
	BingEvent = 32,
	EndeEvent = 33,
    LastSoundEvent
};

// Make sure this matches the number of entries in the SoundEvent enum above
static const int SOUND_EVENTS = LastSoundEvent;

struct SoundEffectData
{
    SoundEvent                  m_soundEventType;
    IXAudio2SourceVoice*        m_soundEffectSourceVoice;
    XAUDIO2_BUFFER              m_audioBuffer;
    byte*                       m_soundEffectBufferData;
    uint32                      m_soundEffectBufferLength;
    uint32                      m_soundEffectSampleRate;
    bool                        m_soundEffectStarted;
};

struct StreamingVoiceContext : public IXAudio2VoiceCallback
{
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32){}
    STDMETHOD_(void, OnVoiceProcessingPassEnd)(){}
    STDMETHOD_(void, OnStreamEnd)(){}
    STDMETHOD_(void, OnBufferStart)(void*)
    {
        ResetEvent(hBufferEndEvent);
    }
    STDMETHOD_(void, OnBufferEnd)(void* pContext)
    {
        // Trigger the event for the music stream.
        if (pContext == 0) {
            SetEvent(hBufferEndEvent);
        }
    }
    STDMETHOD_(void, OnLoopEnd)(void*){}
    STDMETHOD_(void, OnVoiceError)(void*, HRESULT){}

    HANDLE hBufferEndEvent;
    StreamingVoiceContext() : hBufferEndEvent(CreateEventEx(NULL, FALSE, FALSE, NULL))
    {
    }
    virtual ~StreamingVoiceContext()
    {
        CloseHandle(hBufferEndEvent);
    }
};

class Audio;
struct FlugContext : public IXAudio2VoiceCallback
{
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32){}
    STDMETHOD_(void, OnVoiceProcessingPassEnd)(){}
    STDMETHOD_(void, OnStreamEnd)(){}
    STDMETHOD_(void, OnBufferStart)(void*)
    {
        //ResetEvent(hBufferEndEvent);
    }
	void (*bufferEndCallback)(Audio* audio);
	Audio* _audio;
    STDMETHOD_(void, OnBufferEnd)(void* pContext)
    {
		bufferEndCallback(_audio);
    }
    STDMETHOD_(void, OnLoopEnd)(void*){}
    STDMETHOD_(void, OnVoiceError)(void*, HRESULT){}

    /*HANDLE hBufferEndEvent;
    FlugContext() : hBufferEndEvent(CreateEventEx(NULL, FALSE, FALSE, NULL))
    {
    }*/
    virtual ~FlugContext()
    {
        //CloseHandle(hBufferEndEvent);
    }
};

class Audio;
class AudioEngineCallbacks: public IXAudio2EngineCallback
{
private:
    Audio* m_audio;

public :
    AudioEngineCallbacks(){};
    void Initialize(Audio* audio);

    // Called by XAudio2 just before an audio processing pass begins.
    void _stdcall OnProcessingPassStart(){};

    // Called just after an audio processing pass ends.
    void  _stdcall OnProcessingPassEnd(){};

    // Called when a critical system error causes XAudio2
    // to be closed and restarted. The error code is given in Error.
    void  _stdcall OnCriticalError(HRESULT Error);
};

class Audio
{
public:
	int bufferSize;
	int _sampleBits;
	XAUDIO2_BUFFER _flugBuffer;
	float _flugBufferDatabyte[1024];
	BiquadFilterChannel* channel;
	IXAudio2SourceVoice*        _flugSourceVoice;

private:
    IXAudio2*                   m_musicEngine;
    IXAudio2*                   m_soundEffectEngine;
	IXAudio2*					_flugEngine;
    IXAudio2MasteringVoice*     m_musicMasteringVoice;
    IXAudio2MasteringVoice*     m_soundEffectMasteringVoice;
	IXAudio2MasteringVoice*     _flugMasteringVoice;
    IXAudio2SourceVoice*        m_musicSourceVoice;
	
    StreamingVoiceContext       m_voiceContext;
	FlugContext					_flugContext;
    byte                        m_audioBuffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];
    MediaStreamer               m_musicStreamer;
    uint32                      m_currentBuffer;
    SoundEffectData             m_soundEffects[SOUND_EVENTS];
    bool                        m_engineExperiencedCriticalError;
    AudioEngineCallbacks        m_musicEngineCallback;
    AudioEngineCallbacks        m_soundEffectEngineCallback;
	AudioEngineCallbacks        _flugEngineCallback;

    void CreateSourceVoice(SoundEvent);
	void FlugBufferEnded();

public:
    bool m_isAudioStarted;

    ~Audio();
    void Initialize();
    void CreateResources();
    void ReleaseResources();
    void Start();
    void Render();
    void SuspendAudio();
    void ResumeAudio();

	void SetChannel(BiquadFilterChannel* newChannel);

    // This flag can be used to tell when the audio system
    // is experiencing critial errors.
    // XAudio2 gives a critical error when the user unplugs
    // the headphones and a new speaker configuration is generated.
    void SetEngineExperiencedCriticalError()
    {
        m_engineExperiencedCriticalError = true;
    }

    bool HasEngineExperiencedCriticalError()
    {
        return m_engineExperiencedCriticalError;
    }

    void PlaySoundEffect(SoundEvent sound);
    bool IsSoundEffectStarted(SoundEvent sound);
    void StopSoundEffect(SoundEvent sound);
    void SetSoundEffectVolume(SoundEvent sound, float volume);
    void SetSoundEffectPitch(SoundEvent sound, float pitch);
    void SetSoundEffectFilter(SoundEvent sound, float frequency, float oneOverQ);
};

