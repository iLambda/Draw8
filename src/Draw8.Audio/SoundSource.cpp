#include "pch.h"
#include "SoundSource.h"
#include "SoundEngine.h"
#include "Sound.h"

Draw8::Audio::SoundSource::SoundSource(Draw8::Audio::SoundEngine^ SoundEngine, Platform::String^ Filename)
{
	//Import data
	int SampleRate;
	Draw8::Audio::Channels Channels;

	//Loads data
	auto Data = SoundData::LoadFromFile(Filename, &SampleRate, &Channels);

	//Check
	Draw8::ThrowIf(SampleRate < DRAW8_AUDIO_MIN_SAMPLE_RATE || SampleRate > DRAW8_AUDIO_MAX_SAMPLE_RATE, "Sample rate must be between " + DRAW8_AUDIO_MIN_SAMPLE_RATE + " and " + DRAW8_AUDIO_MAX_SAMPLE_RATE + ".", E_INVALIDARG);

	//Tests data size
	if(Channels == Draw8::Audio::Channels::Stereo)
	{
		if(Data->GetDataSize() % 2 != 0)
		{
			throw ref new Platform::InvalidArgumentException("Stereo SoundData's raw data must have a length multiple of two."); 
		}
	}

	//Saves the data
	this->m_data = Data;
	this->m_samplingRate = SampleRate;
	this->m_channels = Channels;
	this->m_engine = SoundEngine;
}

Draw8::Audio::SoundSource::SoundSource(Draw8::Audio::SoundEngine^ SoundEngine, Draw8::Audio::SoundData^ Data, Draw8::Audio::Channels Channels, int SampleRate)
{
	//Check
	Draw8::ThrowIf(SampleRate < DRAW8_AUDIO_MIN_SAMPLE_RATE || SampleRate > DRAW8_AUDIO_MAX_SAMPLE_RATE, "Sample rate must be between " + DRAW8_AUDIO_MIN_SAMPLE_RATE + " and " + DRAW8_AUDIO_MAX_SAMPLE_RATE + ".", E_INVALIDARG);

	//Tests data size
	if(Channels == Draw8::Audio::Channels::Stereo)
	{
		if(Data->GetDataSize() % 2 != 0)
		{
			throw ref new Platform::InvalidArgumentException("Stereo SoundData's raw data must have a length multiple of two."); 
		}
	}

	//Saves the data
	this->m_data = Data;
	this->m_samplingRate = SampleRate;
	this->m_channels = Channels;
	this->m_engine = SoundEngine;
}

Draw8::Audio::Sound^ Draw8::Audio::SoundSource::Instantiate(LoopInfo^ LoopInfo, const Platform::Array<SoundTarget^>^ Targets)
{
	//Returns a new instance
	auto targets = Targets->Length == 0 ? nullptr : const_cast<Platform::Array<SoundTarget^>^>(Targets);
	return ref new Sound(m_engine, this, LoopInfo, targets);
}

Draw8::Audio::SoundSource::~SoundSource()
{
	//Removes the engine
	m_engine = nullptr;
	
	//Removes the data
	m_data = nullptr;
}