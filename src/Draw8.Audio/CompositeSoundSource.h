#include "pch.h"

#ifndef DEF_COMPOSITESOUNDSOURCE
#define DEF_COMPOSITESOUNDSOURCE

#include "SoundData.h"
#include "ISoundSource.h"
#include "SoundTarget.h"

namespace Draw8
{
	namespace Audio
	{
		//Forward declaration
		ref class SoundEngine;
		
		//Forward declaration	
		ref class CompositeSound;

		///<summary>A playable composite sound with multiple parts that can be used with a correctly initialized SoundEngine (<see cref="SoundEngine"/>). </summary>
		public ref class CompositeSoundSource sealed : public ISoundSource 
		{
			private:
				//Contains the data
				Platform::Array<SoundData^>^ m_data;
				//Contains the looping sequence
				Platform::Array<LoopInfo^>^ m_loopingSequence;
				//Contains the channels
				Draw8::Audio::Channels m_channels;
				//Contains the sampling rate
				int m_samplingRate;
				//Contains the data type
				WORD m_dataFormat;
				//Contains a copy of the engine pointer
				SoundEngine^ m_engine;
				//The instances
				std::vector<Platform::WeakReference> m_instances;
				//Is disposed
				bool m_disposed;

			public:
				///<summary>Contains the data representing the sound.</summary>
				property Platform::Array<SoundData^>^ Data { Platform::Array<SoundData^>^ get() { return m_data; } } 
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { virtual Draw8::Audio::Channels get() { return m_channels; } }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { virtual int get() { return m_samplingRate; } }

			public:
				///<summary>Loads a composite sound from files.</summary>
				///<param name="SoundEngine">The Sound engine which'll be able to play the sound.</param>
				///<param name="Filenames">The filenames of the parts you want to load.</param>
				///<param name="LoopingParts">The information about looping each part.</param>
				CompositeSoundSource(Draw8::Audio::SoundEngine^ SoundEngine, const Platform::Array<Platform::String^>^ Filenames, const Platform::Array<LoopInfo^>^ LoopingParts);
				///<summary>Loads a sound from a SoundData object.</summary>
				///<param name="SoundEngine">The Sound engine which'll be able to play the sound.</param>
				///<param name="Data">The array of data representing the parts of your sound.</param>
				///<param name="LoopingParts">The information about looping each part.</param>
				///<param name="Channels">Defines if the sound is Mono (1 channel) or Stereo (2 channels).</param>
				///<param name="SampleRate">The number of samples played in one second. Value in Hertz. Usually 44100.</param>
				CompositeSoundSource(Draw8::Audio::SoundEngine^ SoundEngine, const Platform::Array<SoundData^>^ Data, const Platform::Array<LoopInfo^>^ LoopingParts, Draw8::Audio::Channels Channels, int SampleRate);

			public:
				///<summary>Disposes of source and all instances</summary>
				virtual ~CompositeSoundSource();

			public:
				///<summary>Create an instance that can be played.</summary>
				///<param name="Targets">The sound output targets. If this parameter is null, the output will be the speaker itself.</param>
				///<returns>A new instance of this sound ready to be played.</returns>
				CompositeSound^ Instantiate(const Platform::Array<SoundTarget^>^ Targets);

			internal:
				//Returns the looping sequence
				Platform::Array<LoopInfo^>^ GetLoopingSequence() { return m_loopingSequence; }
		};
	}
}

#endif