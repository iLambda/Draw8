#include "pch.h"
#include "CompositeSound.h"
#include "SoundData.h"
#include "CompositeSoundSource.h"
#include "SoundEngine.h"

float Draw8::Audio::CompositeSound::Volume::get()
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return -1;

	//Returns the volume
	float volume;
	m_sourceVoice->GetVolume(&volume);
	return volume;
}

void Draw8::Audio::CompositeSound::Volume::set(float Value)
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return;

	//Sets the volume
	m_sourceVoice->SetVolume(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_VOLUME, DRAW8_AUDIO_MAX_VOLUME));
}

float Draw8::Audio::CompositeSound::FrequencyRatio::get()
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return -1;

	//Returns the freq ration
	float frequencyRatio;
	m_sourceVoice->GetFrequencyRatio(&frequencyRatio);
	return frequencyRatio;
}

int Draw8::Audio::CompositeSound::Part::get()
{
	//Gets the state
	XAUDIO2_VOICE_STATE voiceState;
	m_sourceVoice->GetState(&voiceState, XAUDIO2_VOICE_NOSAMPLESPLAYED);

	//Returns the part
	return m_source->Data->Length - voiceState.BuffersQueued;
}

void Draw8::Audio::CompositeSound::FrequencyRatio::set(float Value)
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return;

	//Sets the freq ratio
	m_sourceVoice->SetFrequencyRatio(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_FREQ_RATIO, DRAW8_AUDIO_MAX_FREQ_RATIO));
}

float Draw8::Audio::CompositeSound::Progression::get()
{
	//Gets the state
	XAUDIO2_VOICE_STATE voiceState;
	m_sourceVoice->GetState(&voiceState);

	//Computes size
	auto data = m_source->Data;
	auto len = 0;
	for (uint i = 0; i < data->Length; i++)
		len += data[i]->GetDataSize();

	//Returns the progression
	return ((float)voiceState.SamplesPlayed / (float)len);
}

Draw8::Audio::ISoundSource^ Draw8::Audio::CompositeSound::Source::get()
{
	//Returns the source
	return m_source;
}

Draw8::Audio::CompositeSound::CompositeSound(SoundEngine^ Engine, CompositeSoundSource^ Source, Platform::Array<SoundTarget^>^ Targets)
{
	//Saves the source and the data
	this->m_source = Source;
	this->m_playing = false;
	this->m_filter = nullptr;
	this->m_targets = Targets;

	//Gets some useful info
	auto looped = m_source->GetLoopingSequence();
	auto data = m_source->Data;
	auto sampleBits = Draw8::Audio::GetSampleBits(data[0]->Type);
	auto channels = (int)m_source->Channels;
	auto samplingRate = m_source->SampleRate;
	auto format = Draw8::Audio::GetFormat(data[0]->Format);

	//Fills the headed
    m_waveHeader.wBitsPerSample = sampleBits;
    m_waveHeader.nAvgBytesPerSec = (samplingRate * channels * sampleBits) / 8;
    m_waveHeader.nChannels = channels;
    m_waveHeader.nBlockAlign = channels * sampleBits / 8;
    m_waveHeader.wFormatTag = format; 
    m_waveHeader.nSamplesPerSec = (DWORD)samplingRate;
    m_waveHeader.cbSize = 0;

	//Creating the voices
	m_callback.SetEmitter(Platform::WeakReference(this));
	Draw8::ThrowIfFailed(Engine->GetXAudioEngine()->CreateSourceVoice(&m_sourceVoice, &m_waveHeader, XAUDIO2_VOICE_USEFILTER, DRAW8_AUDIO_MAX_FREQ_RATIO, &m_callback, Targets == nullptr ? NULL : &Draw8::Audio::GetDescriptor(Targets), NULL));

	//Setting buffers
	for (int i = 0; i < (int)data->Length; i++)
	{
		//Gathering data
		LoopInfo^ loopInf;
		if(looped[i] == nullptr)
			loopInf = ref new LoopInfo();
		else
			loopInf = looped[i];

		auto isLooped = loopInf->Times > 1;
		auto samples = (int)floor(((float)loopInf->Start / 1000.f) * samplingRate);
		auto begin = Draw8::Clamp(samples, 0, data[i]->GetDataSize());

		//Creating the sound buffer
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = data[i]->GetDataSize() * sampleBits / 8;
		buffer.pAudioData = (byte*)data[i]->GetData();
		buffer.Flags = (i == data->Length - 1) ? XAUDIO2_END_OF_STREAM : 0;
		buffer.pContext = NULL;
		buffer.PlayBegin = isLooped ? 0 : begin;

		//If looped
		if(isLooped)
		{
			buffer.PlayLength = 0; 
			buffer.LoopBegin = begin;
			buffer.LoopLength = 0;
			buffer.LoopCount = loopInf->Times;
		}

		//Sending the sound buffer
		Draw8::ThrowIfFailed(m_sourceVoice->SubmitSourceBuffer(&buffer));
	}

	//Sets chain
	m_chain = ref new SoundEffectChain((IXAudio2Voice**)(&m_sourceVoice), Channels);
}

Draw8::Audio::CompositeSound::~CompositeSound()
{
	//Removes chain
	//m_chain->Dispose();
	m_chain = nullptr;

	//Removing the voice
	m_sourceVoice->DestroyVoice();
	m_sourceVoice = NULL;

	//Removing filter
	m_filter = nullptr;
		
	//Removing targets
	m_targets = nullptr;

	//Removing the source
	m_source = nullptr;
}

void Draw8::Audio::CompositeSound::Play()
{
	//If null, nothing
	if(m_sourceVoice == nullptr)
		return;

	//Play the sound
	Draw8::ThrowIfFailed(m_sourceVoice->Start());
	m_playing = true;
} 

void Draw8::Audio::CompositeSound::Suspend(bool ProcessEffects)
{
	//If null, nothing
	if(m_sourceVoice == nullptr)
		return;

	//Pauses the sound
	Draw8::ThrowIfFailed(m_sourceVoice->Stop(ProcessEffects ? XAUDIO2_PLAY_TAILS : 0));
	m_playing = false;
}

void Draw8::Audio::CompositeSound::Next()
{
	//If null, nothing
	if(m_sourceVoice == nullptr)
		return;

	//We exit the loop
	Draw8::ThrowIfFailed(m_sourceVoice->ExitLoop());
}

void Draw8::Audio::CompositeSound::Filter::set(Draw8::Audio::SoundFilter^ Filter)
{
	//Set the filter
	m_filter = Filter;
	XAUDIO2_FILTER_PARAMETERS desc;

	//We get the descriptor
	if(Filter != nullptr)
		desc = Filter->GetDescriptor((float)m_source->SampleRate);
	else
	{
		desc.Frequency = 1.0f;    
		desc.OneOverQ = 1.0f;
		desc.Type = (XAUDIO2_FILTER_TYPE)4;
	}

	//We set it
	Draw8::ThrowIfFailed(m_sourceVoice->SetFilterParameters(&desc));
}

void Draw8::Audio::CompositeSound::Outputs::set(const Platform::Array<SoundTarget^>^ Targets)
{
	//Nulling currents targets
	m_targets = nullptr;
		
	//Test if the target is null
	auto targets = Targets == nullptr ? nullptr : (Targets->Length == 0 ? nullptr : Targets);
	
	//Sets data
	if(targets != nullptr)
		m_sourceVoice->SetOutputVoices(&GetDescriptor(Targets));
	else
		m_sourceVoice->SetOutputVoices(NULL);

	//Copies the target
	m_targets = targets != nullptr ? const_cast<Platform::Array<SoundTarget^>^>(targets) : nullptr;
}