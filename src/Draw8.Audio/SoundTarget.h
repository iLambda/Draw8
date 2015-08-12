#include "pch.h"

#ifndef DEF_SOUNDTARGET
#define DEF_SOUNDTARGET

#include "SoundEngine.h"
#include "ISoundEmitter.h"
#include "AudioTools.h"
#include "SoundEffectChain.h"

namespace Draw8
{
	namespace Audio
	{
		///<summary>A target for the output of a sound.</summary>
		public ref class SoundTarget sealed : public ISoundEmitter 
		{
			private:
				//The filter in itself
				SoundFilter^ m_filter;
				//The submix voice
				IXAudio2SubmixVoice* m_submixVoice;
				//Contains the channels
				Draw8::Audio::Channels m_channels;
				//Contains the sampling rate
				int m_samplingRate;
				//Contains the outputs
				Platform::Array<SoundTarget^>^ m_targets;
				//The effect chain
				SoundEffectChain^ m_chain;

			public:
				///<summary>A sound filter that will be applied on sound.</summary>
				property SoundFilter^ Filter { virtual SoundFilter^ get() { return m_filter; } virtual void set(SoundFilter^ val); }
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { virtual Draw8::Audio::Channels get() { return m_channels; } }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { virtual int get() { return m_samplingRate; } }
				///<summary>The actual sound volume presented like a multiplier : 0 is silence, 1 is no attenuation</summary>
				property float Volume { virtual float get(); virtual void set(float value); }
				///<summary>This emitter output(s). If null, the output is the speaker itself.</summary>
				property Platform::Array<SoundTarget^>^ Outputs { Platform::Array<SoundTarget^>^ get() { return m_targets; } void set(const Platform::Array<SoundTarget^>^); }
				///<summary>The effects applied to the sound output.</summary>
				property SoundEffectChain^ Effects { virtual SoundEffectChain^ get() { return m_chain; } }

			public:
				///<summary>Creates an output target.</summary>
				///<param name="Engine">The Sound engine which'll be able to play the sound.</param>
				///<param name="Channels">Defines if the sound is Mono (1 channel) or Stereo (2 channels).</param>
				///<param name="SampleRate">The number of samples played in one second. Value in Hertz. Usually 44100.</param>
				///<param name="Targets">The output target(s). Set to null if the output must directly be the speakers.</param>
				SoundTarget(SoundEngine^ Engine, Draw8::Audio::Channels Channels, int SampleRate, const Platform::Array<SoundTarget^>^ Targets);

			private:
				//Removes the sound
				~SoundTarget();

			internal:
				//Returns the voice
				IXAudio2SubmixVoice* GetVoice() { return m_submixVoice; }
		};

		//Gives a correctly made sendlist
		XAUDIO2_VOICE_SENDS GetDescriptor(const Platform::Array<Audio::SoundTarget^>^ Targets);
	}
}

#endif