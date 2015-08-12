#include "pch.h"
#include "CompositeSoundSource.h"
#include "CompositeSound.h"
#include "SoundEngine.h"
#include "Sound.h"
#include "AudioTools.h"

Draw8::Audio::CompositeSoundSource::CompositeSoundSource(Draw8::Audio::SoundEngine^ SoundEngine, const Platform::Array<Platform::String^>^ Filenames, const Platform::Array<LoopInfo^>^ LoopingParts)
{
	//Check
	Draw8::ThrowIf(Filenames->Length != LoopingParts->Length, "The length of Filenames and LoopingParts does not match. Both must have the same length.", E_INVALIDARG);
	Draw8::ThrowIf(Filenames->Length < 2, "There must be more than one part in the composite sound.", E_INVALIDARG);
	
	//Create vars
	auto Data = ref new Platform::Array<SoundData^>(Filenames->Length);
	int* SampleRates = new int[Filenames->Length];
	auto Channelss = new Draw8::Audio::Channels[Filenames->Length];

	//Load data
	for(uint i = 0 ; i != Filenames->Length; i++)
		Data[i] = SoundData::LoadFromFile(Filenames[i], &(SampleRates[i]), &(Channelss[i]));

	//If ALL the Sample rates are not equal
	auto sampleRateEqual = true;
	auto sampleRateModel = SampleRates[0];
	for (uint i = 0; i < Filenames->Length; i++)
		sampleRateEqual = sampleRateEqual && (SampleRates[i] == sampleRateModel);

	//If all the channels count are not equal
	auto channelsEqual = true;
	auto channelsModel = Channelss[0];
	for (uint i = 0; i < Filenames->Length; i++)
		channelsEqual = channelsEqual && (Channelss[i] == channelsModel);

	//Check
	Draw8::ThrowIf(!sampleRateEqual, "Discrepancy detected : not all sounds have the same sample rate.", E_INVALIDARG);
	Draw8::ThrowIf(!channelsEqual,   "Discrepancy detected : not all sounds have the same number of channels.", E_INVALIDARG);

	//Set data
	auto SampleRate = SampleRates[0];
	auto Channels = Channelss[0];

	//Check
	Draw8::ThrowIf(SampleRate < DRAW8_AUDIO_MIN_SAMPLE_RATE || SampleRate > DRAW8_AUDIO_MAX_SAMPLE_RATE, "Sample rate must be between " + DRAW8_AUDIO_MIN_SAMPLE_RATE + " and " + DRAW8_AUDIO_MAX_SAMPLE_RATE + ".", E_INVALIDARG);
	
	//Check if all buffers are the same type
	bool isSameType = true;
	SoundDataType type = Data[0]->Type;
	for (uint i = 0; i < Data->Length; i++)
	{
		isSameType = isSameType && (Data[i]->Type == type);
		Draw8::ThrowIf(Channels == Draw8::Audio::Channels::Stereo && Data[i]->GetDataSize() % 2 != 0, "Stereo SoundData's raw data must have a length multiple of two.", E_INVALIDARG);
	}
	Draw8::ThrowIf(!isSameType, "All buffers must have the same type (float/byte).", E_INVALIDARG);
	
	//Saves the data
	this->m_samplingRate = SampleRate;
	this->m_channels = Channels;
	this->m_engine = SoundEngine;
	this->m_data = ref new Platform::Array<SoundData^>(Data->Length);
	this->m_loopingSequence = const_cast<Platform::Array<LoopInfo^>^>(LoopingParts);
	this->m_disposed = false;
	for(int i = 0 ; i != (int)Data->Length ; i++)
		m_data[i] = Data[i];

	//Deleting useless data in the end
	delete[] SampleRates;
	delete[] Channelss;
}

Draw8::Audio::CompositeSoundSource::CompositeSoundSource(Draw8::Audio::SoundEngine^ SoundEngine, const Platform::Array<SoundData^>^ Data, const Platform::Array<LoopInfo^>^ LoopingParts, Draw8::Audio::Channels Channels, int SampleRate)
{
	//Check
	Draw8::ThrowIf(SampleRate < DRAW8_AUDIO_MIN_SAMPLE_RATE || SampleRate > DRAW8_AUDIO_MAX_SAMPLE_RATE, "Sample rate must be between " + DRAW8_AUDIO_MIN_SAMPLE_RATE + " and " + DRAW8_AUDIO_MAX_SAMPLE_RATE + ".", E_INVALIDARG);
	Draw8::ThrowIf(Data->Length != LoopingParts->Length, "The length of Data and LoopingParts does not match. Both must have the same length.", E_INVALIDARG);
	Draw8::ThrowIf(Data->Length < 2, "There must be more than one part in the composite sound.", E_INVALIDARG);
	
	//Check if all buffers are the same type
	bool isSameType = true;
	SoundDataType type = Data[0]->Type;
	for (uint i = 0; i < Data->Length; i++)
	{
		isSameType = isSameType && (Data[i]->Type == type);
		Draw8::ThrowIf(Channels == Draw8::Audio::Channels::Stereo && Data[i]->GetDataSize() % 2 != 0, "Stereo SoundData's raw data must have a length multiple of two.", E_INVALIDARG);
	}
	Draw8::ThrowIf(!isSameType, "All buffers must have the same type (float/byte).", E_INVALIDARG);
	
	//Saves the data
	this->m_samplingRate = SampleRate;
	this->m_channels = Channels;
	this->m_engine = SoundEngine;
	this->m_data = ref new Platform::Array<SoundData^>(Data->Length);
	this->m_loopingSequence = const_cast<Platform::Array<LoopInfo^>^>(LoopingParts);
	this->m_disposed = false;
	for(int i = 0 ; i != (int)Data->Length ; i++)
		m_data[i] = Data[i];
}

Draw8::Audio::CompositeSound^ Draw8::Audio::CompositeSoundSource::Instantiate(const Platform::Array<SoundTarget^>^ Targets)
{
	//Throws if disposed
	Draw8::ThrowIfDisposed(m_disposed);

	//Returns a new instance
	auto targets = Targets->Length == 0 ? nullptr : const_cast<Platform::Array<SoundTarget^>^>(Targets);
	return ref new CompositeSound(m_engine, this, targets);
}

Draw8::Audio::CompositeSoundSource::~CompositeSoundSource()
{
	//Disposes
	m_disposed = true;

	//Removes the engine
	m_engine = nullptr;
	
	//Removes the data
	m_data = nullptr;

	//Disposes of all instances
	for(int i = 0 ; i != m_instances.size() ; i++)
	{
		if(m_instances[i].Resolve<CompositeSound>() != nullptr)
			delete m_instances[i].Resolve<CompositeSound>();
	}
}