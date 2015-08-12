#include "pch.h"

#ifndef DEF_COMPOSITESOUND
#define DEF_COMPOSITESOUND

#include "ISound.h"
#include "SoundCallback.h"
#include "SoundTarget.h"

namespace Draw8
{
	namespace Audio
	{
		ref class CompositeSoundSource;

		///<summary>A composite sound instantiated from a composite sound source.</summary>
		public ref class CompositeSound sealed : public ISound
		{
			private:
				//Contains the buffer
				IXAudio2SourceVoice* m_sourceVoice;
				//Contains the WAVE header
				WAVEFORMATEX m_waveHeader;
				//Contains the data type
				WORD m_dataFormat;
				//Contains the source
				CompositeSoundSource^ m_source;
				//Contains the played state
				bool m_playing;
				//Contains the current phase playing
				int m_phase;
				//Contains the sound callback manager
				SoundCallback m_callback;
				//Private delegates
				event LoopEndEventHandler^ m_loopEndEventHandler;
				event PartBeginEventHandler^ m_partBeginEventHandler;
				event PartEndEventHandler^ m_partEndEventHandler;
				//The filter in question
				SoundFilter^ m_filter;
				//The outputs
				Platform::Array<SoundTarget^>^ m_targets;
				//The effect chain
				SoundEffectChain^ m_chain;
				
			internal:
				//Creates an instance
				CompositeSound(SoundEngine^ Engine, CompositeSoundSource^ Source, Platform::Array<SoundTarget^>^ Targets);
				
			public:
				///<summary>Disposes of the sound</summary>
				virtual ~CompositeSound();

			public:
				///<summary>Returns the current reading progression. 0 is beginning, and 1 is end. 
				///If the sound is on loop mode, (Progression modulo 1) = fracPart(Progression) is the current progression, 
				///nd intPart(Progression) is the number of time it has been played.</summary>
				property float Progression { virtual float get(); }
				///<summary>The actual sound volume presented like a multiplier : 0 is silence, 1 is no attenuation</summary>
				property float Volume { virtual float get(); virtual void set(float value); }
				///<summary>The frequency ratio.</summary>
				property float FrequencyRatio { virtual float get(); virtual void set(float value); }
				///<summary>The source that created the sound.</summary>
				property ISoundSource^ Source { virtual ISoundSource^ get(); }
				///<summary>True if the sound is going to loop at its end. False otherwise.</summary>
				property bool Looped { virtual bool get() { return false; } }
				///<summary>True if the sound is currently playing. False otherwise.</summary>
				property bool IsPlaying { virtual bool get() { return m_playing; } }
				///<summary>Returns the number qualifying the sound this compite sound is actually playing (0 is first one, 1 is second one, ...).</summary>
				property int Part { int get(); }
				///<summary>A tag that can be set by the user</summary>
				property virtual Object^ Tag;
				///<summary>A sound filter that will be applied on sound.</summary>
				property SoundFilter^ Filter { virtual SoundFilter^ get() { return m_filter; } virtual void set(SoundFilter^ val); }
				///<summary>This emitter output(s). If null, the output is the speaker itself.</summary>
				property Platform::Array<SoundTarget^>^ Outputs { Platform::Array<SoundTarget^>^ get() { return m_targets; } void set(const Platform::Array<SoundTarget^>^); }
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { virtual Draw8::Audio::Channels get() { return Source->Channels; } }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { virtual int get() { return Source->SampleRate; } }
				///<summary>The effects applied to the sound output.</summary>
				property SoundEffectChain^ Effects { virtual SoundEffectChain^ get() { return m_chain; } }

			public:
				///<summary>Starts the sound.</summary>
				virtual void Play();
				///<summary>Suspends the sound.</summary>
				///<param name="ProcessEffects">True if you want to process the tails of the effects. False elsewise.</param>
				virtual void Suspend(bool ProcessEffects);
				///<summary>Goes to the next region.</summary>
				void Next();

			public:
				///<summary>Called when a loop ends and then restarts after.</summary>
				event LoopEndEventHandler^ LoopEnd
				{
					virtual Windows::Foundation::EventRegistrationToken add(LoopEndEventHandler^ Handler) { return m_loopEndEventHandler += Handler; }
					virtual void remove(Windows::Foundation::EventRegistrationToken Token) { m_loopEndEventHandler -= Token; }
					virtual void raise(ISound^ Sound, Platform::Object^ Args) { m_loopEndEventHandler(Sound, Args); }
				}
				///<summary>Called when a part of the sound begin. If the sound is monopart (Sound), this event
				///is invoked when the sound begin. If the sound is multipart (CompositeSound), it is invoked when a part of the sound
				///starts.</summary>
				event PartBeginEventHandler^ PartBegin
				{
					virtual Windows::Foundation::EventRegistrationToken add(PartBeginEventHandler^ Handler) { return m_partBeginEventHandler += Handler; }
					virtual void remove(Windows::Foundation::EventRegistrationToken Token) { m_partBeginEventHandler -= Token; }
					virtual void raise(ISound^ Sender, Platform::Object^ Args) { m_partBeginEventHandler(Sender, Args); }
				}
				///<summary>Called when a part of the sound ends. If the sound is monopart (Sound), this event
				///is invoked when the sound ends. If the sound is multipart (CompositeSound), it is invoked when a part of the sound
				///ends.</summary>
				event PartEndEventHandler^ PartEnd
				{
					virtual Windows::Foundation::EventRegistrationToken add(PartEndEventHandler^ Handler) { return m_partEndEventHandler += Handler; }
					virtual void remove(Windows::Foundation::EventRegistrationToken Token) { m_partEndEventHandler -= Token; }
					virtual void raise(ISound^ Sender, Platform::Object^ Args) { m_partEndEventHandler(Sender, Args); }
				}

		};
	}
}

#endif