#include "pch.h"
#include "SoundEngine.h"
#include "SoundSource.h"
#include <time.h>

Draw8::Audio::SoundEngine::SoundEngine()
{
	//Creates the engine
	Create(nullptr);
}

Draw8::Audio::SoundEngine::SoundEngine(IDevice^ OutputDevice)
{
	//Creates the engine
	Create(OutputDevice);
}

void Draw8::Audio::SoundEngine::Create(IDevice^ OutputDevice)
{
	//Set data
	m_outputDevice = OutputDevice;

	//We create the XAudio2 device
	ThrowIfFailed(XAudio2Create(&m_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));
	
	//If the device is null, and the ThrowIfFailed returned true, we stop the program
	if(m_xaudio2 == nullptr)
		throw ref new Platform::NullReferenceException("The XAudio2 Device was not created.");
	
	//We create a mastering voice
	ThrowIfFailed(m_xaudio2->CreateMasteringVoice(&m_masteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, OutputDevice == nullptr ? NULL : OutputDevice->DeviceID->Data(), 0, AudioCategory_GameEffects));

	//The engine is started
	IsStarted = true;

	//Srand is called
	srand((uint)time(NULL));
}

void Draw8::Audio::SoundEngine::Start()
{
	//If already started, we do nothing
	if(IsStarted)
		return;

	//We start the engine
	Draw8::ThrowIfFailed(m_xaudio2->StartEngine());

	//And we change the state
	IsStarted = true;
}

void Draw8::Audio::SoundEngine::Suspend()
{
	//If not started, we do nothing
	if(!IsStarted)
		return;

	//We stop the engine
	m_xaudio2->StopEngine();

	//And we change the state
	IsStarted = false;
}

float Draw8::Audio::SoundEngine::GlobalVolume::get()
{
	//If the voice ain't created
	if(m_masteringVoice == nullptr)
		return -1;

	//Returns the volume
	float volume;
	m_masteringVoice->GetVolume(&volume);
	return volume;
}

void Draw8::Audio::SoundEngine::GlobalVolume::set(float Value)
{
	//If the voice ain't created
	if(m_masteringVoice == nullptr)
		return;

	//Sets the volume
	m_masteringVoice->SetVolume(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_VOLUME, DRAW8_AUDIO_MAX_VOLUME));
}
