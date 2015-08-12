#include "pch.h"
#include "Sound.h"
#include "SoundData.h"
#include "SoundSource.h"
#include "SoundEngine.h"
#include "SoundTarget.h"
#include "SoundEffectChain.h"


float Draw8::Audio::Sound::Volume::get()
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return -1;

	//Returns the volume
	float volume;
	m_sourceVoice->GetVolume(&volume);
	return volume;
}

void Draw8::Audio::Sound::Volume::set(float Value)
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return;

	//Sets the volume
	m_sourceVoice->SetVolume(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_VOLUME, DRAW8_AUDIO_MAX_VOLUME));
}

float Draw8::Audio::Sound::FrequencyRatio::get()
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return -1;

	//Returns the freq ration
	float frequencyRatio;
	m_sourceVoice->GetFrequencyRatio(&frequencyRatio);
	return frequencyRatio;
}

void Draw8::Audio::Sound::FrequencyRatio::set(float Value)
{
	//If the voice ain't created
	if(m_sourceVoice == nullptr)
		return;

	//Sets the freq ratio
	m_sourceVoice->SetFrequencyRatio(Draw8::Clamp(Value, DRAW8_AUDIO_MIN_FREQ_RATIO, DRAW8_AUDIO_MAX_FREQ_RATIO));
}

float Draw8::Audio::Sound::Progression::get()
{
	//Gets the state
	XAUDIO2_VOICE_STATE voiceState;
	m_sourceVoice->GetState(&voiceState);

	//Returns the progression
	return ((float)voiceState.SamplesPlayed / (float)((SoundSource^)Source)->Data->GetDataSize());
}

Draw8::Audio::ISoundSource^ Draw8::Audio::Sound::Source::get()
{
	//Returns the source
	return m_source;
}

Draw8::Audio::Sound::Sound(SoundEngine^ Engine, SoundSource^ Source, LoopInfo^ LoopInfo, Platform::Array<SoundTarget^>^ Targets)
{
	//Saves the source and the data
	this->m_source = Source;
	this->m_playing = false;
	this->m_filter = nullptr;
	this->m_targets = Targets;

	//Gets some useful info
	auto sampleBits = Draw8::Audio::GetSampleBits(m_source->Data->Type);
	auto channels = (int)m_source->Channels;
	auto samplingRate = m_source->SampleRate;
	auto format = Draw8::Audio::GetFormat(m_source->Data->Format);
	auto length = m_source->Data->GetDataSize();
	
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

	//Gathering data
	Draw8::Audio::LoopInfo^ loopInf;
	if(LoopInfo == nullptr)
		loopInf = ref new Draw8::Audio::LoopInfo();
	else
		loopInf = LoopInfo;

	//Saving state
	this->m_looped = loopInf;

	//Preparing data for buffers
	auto isLooped = loopInf->Times > 1;
	auto samples = (int)floor(((float)loopInf->Start / 1000.f) * samplingRate);
	auto begin = Draw8::Clamp(samples, 0, m_source->Data->GetDataSize());
		
	//Creating the sound buffer
	XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = length * sampleBits / 8;
    buffer.pAudioData = (byte*)m_source->Data->GetData();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.pContext = NULL;
	buffer.PlayBegin = isLooped ? 0 : begin;;

	//If looped
	if(Looped)
	{
		buffer.PlayLength = 0; 
		buffer.LoopBegin = begin;
		buffer.LoopLength = 0;
		buffer.LoopCount = loopInf->Times;
	}

	//Sending the sound buffer
    Draw8::ThrowIfFailed(m_sourceVoice->SubmitSourceBuffer(&buffer));

	//Sets chain
	m_chain = ref new SoundEffectChain((IXAudio2Voice**)(&m_sourceVoice), Channels);
}

Draw8::Audio::Sound::~Sound()
{
	//Removes chain
	m_chain = nullptr;

	//Removing the voice
	m_sourceVoice->DestroyVoice();
	m_sourceVoice = NULL;

	//Removing targets
	m_targets = nullptr;

	//Removing the source
	m_source = nullptr;
}

void Draw8::Audio::Sound::Play()
{
	//If null, nothing
	if(m_sourceVoice == nullptr)
		return;

	//Play the sound
	Draw8::ThrowIfFailed(m_sourceVoice->Start());
	m_playing = true;
} 

void Draw8::Audio::Sound::Suspend(bool ProcessEffects)
{
	//If null, nothing
	if(m_sourceVoice == nullptr)
		return;

	//Pauses the sound
	Draw8::ThrowIfFailed(m_sourceVoice->Stop(ProcessEffects ? XAUDIO2_PLAY_TAILS : 0));
	m_playing = false;
}

void Draw8::Audio::Sound::Filter::set(Draw8::Audio::SoundFilter^ Filter)
{
	//We get the descriptor
	auto desc = Filter->GetDescriptor((float)m_source->SampleRate);
	m_filter = Filter;

	//We set it
	Draw8::ThrowIfFailed(m_sourceVoice->SetFilterParameters(&desc));
}

void Draw8::Audio::Sound::Outputs::set(const Platform::Array<SoundTarget^>^ Targets)
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