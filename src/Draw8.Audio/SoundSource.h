#include "pch.h"

#ifndef DEF_SOUNDSOURCE
#define DEF_SOUNDSOURCE

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
		ref class Sound;

		///<summary>A playable sound that can be used with a correctly initialized SoundEngine (<see cref="SoundEngine"/>).</summary>
		public ref class SoundSource sealed : public ISoundSource 
		{
			private:
				//Contains the data
				SoundData^ m_data;
				//Contains the channels
				Draw8::Audio::Channels m_channels;
				//Contains the sampling rate
				int m_samplingRate;
				//Contains the data type
				WORD m_dataFormat;
				//Contains a copy of the engine pointer
				SoundEngine^ m_engine;

			public:
				///<summary>Contains the data representing the sound.</summary>
				property SoundData^ Data { SoundData^ get() { return m_data; } } 
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { virtual Draw8::Audio::Channels get() { return m_channels; } }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { virtual int get() { return m_samplingRate; } }

			public:
				///<summary>Loads a sound from a file</summary>
				///<param name="SoundEngine">The Sound engine which'll be able to play the sound.</param>
				///<param name="Filename">The name of the file that'll be loaded.</param>
				SoundSource(Draw8::Audio::SoundEngine^ SoundEngine, Platform::String^ Filename);
				///<summary>Loads a sound from a SoundData object.</summary>
				///<param name="SoundEngine">The Sound engine which'll be able to play the sound.</param>
				///<param name="Data">The name of the file that'll be loaded.</param>
				///<param name="Channels">Defines if the sound is Mono (1 channel) or Stereo (2 channels).</param>
				///<param name="SampleRate">The number of samples played in one second. Value in Hertz. Usually 44100.</param>
				SoundSource(Draw8::Audio::SoundEngine^ SoundEngine, Draw8::Audio::SoundData^ Data, Draw8::Audio::Channels Channels, int SampleRate);

			private:
				//Disposes source
				~SoundSource();

			public:
				///<summary>Create an instance that can be played.</summary>
				///<param name="LoopInfo">The definition of the sound looping section.</param>
				///<param name="Targets">The sound output targets. If this parameter is null, the output will be the speaker itself.</param>
				///<returns>A new instance of this sound ready to be played.</returns>
				Sound^ Instantiate(LoopInfo^ LoopInfo, const Platform::Array<SoundTarget^>^ Targets);
		};
	}
}

#endif