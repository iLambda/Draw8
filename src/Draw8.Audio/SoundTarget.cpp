#include "pch.h"
#include "SoundTarget.h"
#include "SoundEffectChain.h"

Draw8::Audio::SoundTarget::SoundTarget(Draw8::Audio::SoundEngine^ Engine, Draw8::Audio::Channels Channels, int SampleRate, const Platform::Array<SoundTarget^>^ Targets)
{
	//Sets data
	m_channels = Channels;
	m_samplingRate = SampleRate;

	//Creates submix voice
	auto targets = Targets == nullptr ? nullptr : (Targets->Length == 0 ? nullptr : Targets);
	Engine->GetXAudioEngine()->CreateSubmixVoice(&m_submixVoice, (int)Channels, (uint)SampleRate, XAUDIO2_VOICE_USEFILTER, 0, (targets == nullptr) ? NULL : &Draw8::Audio::GetDescriptor(Targets), 0);	

	//Copies the target
	m_targets = targets != nullptr ? const_cast<Platform::Array<SoundTarget^>^>(targets) : nullptr;

	//Sets chain
	m_chain = ref new SoundEffectChain((IXAudio2Voice**)(&m_submixVoice), Channels);
}

Draw8::Audio::SoundTarget::~SoundTarget()
{
	//Removes chain
	m_chain = nullptr;

	//Removes voice
	m_submixVoice->DestroyVoice();
	m_submixVoice = NULL;
}

void Draw8::Audio::SoundTarget::Filter::set(Draw8::Audio::SoundFilter^ Filter)
{
	//Set the filter
	m_filter = Filter;
	XAUDIO2_FILTER_PARAMETERS desc;

	//We get the descriptor
	if(Filter != nullptr)
		desc = Filter->GetDescriptor((float)SampleRate);
	else
	{
		desc.Frequency = 1.0f;    
		desc.OneOverQ = 1.0f;
		desc.Type = (XAUDIO2_FILTER_TYPE)4;
	}

	//We set it
	Draw8::ThrowIfFailed(m_submixVoice->SetFilterParameters(&desc));
}

float Draw8::Audio::SoundTarget::Volume::get()
{
	//If the voice ain't created
	if(m_submixVoice == nullptr)
		return -1;

	//Returns the volume
	float volume;
	m_submixVoice->GetVolume(&volume);
	return volume;
}

void Draw8::Audio::SoundTarget::Volume::set(float Value)
{
	//If the voice ain't created
	if(m_submixVoice == nullptr)
		return;

	//Sets the volume
	m_submixVoice->SetVolume(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_VOLUME, DRAW8_AUDIO_MAX_VOLUME));
}

XAUDIO2_VOICE_SENDS Draw8::Audio::GetDescriptor(const Platform::Array<Audio::SoundTarget^>^ Targets)
{
	//Creates the sendlist
	XAUDIO2_SEND_DESCRIPTOR* sendList = NULL;
	XAUDIO2_VOICE_SENDS send;

	//Fills it if needed
	sendList = new XAUDIO2_SEND_DESCRIPTOR[Targets->Length];
	for(int i = 0 ; i != Targets->Length ; i++)
	{
		sendList[i].Flags = 0;
		if(Targets[i] != nullptr)
			sendList[i].pOutputVoice = Targets[i]->GetVoice();
		else
			throw ref new Platform::NullReferenceException("The SoundTarget given at the index " + i + " was null.");
	}

	send.SendCount = Targets->Length;
	send.pSends = sendList;

	return send;
}

void Draw8::Audio::SoundTarget::Outputs::set(const Platform::Array<SoundTarget^>^ Targets)
{
	//Nulling currents targets
	m_targets = nullptr;
		
	//Test if the target is null
	auto targets = Targets == nullptr ? nullptr : (Targets->Length == 0 ? nullptr : Targets);
	
	//Sets data
	if(targets != nullptr)
		m_submixVoice->SetOutputVoices(&GetDescriptor(Targets));
	else
		m_submixVoice->SetOutputVoices(NULL);

	//Copies the target
	m_targets = targets != nullptr ? const_cast<Platform::Array<SoundTarget^>^>(targets) : nullptr;
}