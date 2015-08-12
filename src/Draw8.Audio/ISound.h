#include "pch.h"

#ifndef DEF_ISOUND
#define DEF_ISOUND

#include "ISoundSource.h"
#include "ISoundEmitter.h"

namespace Draw8
{
	namespace Audio
	{
		///<summary>Represents the method called at the end of a looping sequence.</summary>
		///<param name="Sender">The sender of the event.</param>
		///<param name="Args">The arguments. Always null.</param>
		public delegate void LoopEndEventHandler(ISound^ Sender, Platform::Object^ Args);

		///<summary>Represents the method called at the end of a sound part.</summary>
		///<param name="Sender">The sender of the event.</param>
		///<param name="Args">The arguments. Always null.</param>
		public delegate void PartEndEventHandler(ISound^ Sender, Platform::Object^ Args);

		///<summary>Represents the method called at the beginning of a sound part.</summary>
		///<param name="Sender">The sender of the event.</param>
		///<param name="Args">The arguments. Always null.</param>
		public delegate void PartBeginEventHandler(ISound^ Sender, Platform::Object^ Args);

		///<summary>A sound instantiated from a source played with XAudio2.</summary>
		public interface class ISound : public ISoundEmitter
		{
			public:
				///<summary>Returns the current reading progression. 0 is beginning, and 1 is end. 
				///If the sound is on loop mode, (Progression modulo 1) = fracPart(Progression) is the current progression, 
				///nd intPart(Progression) is the number of time it has been played.</summary>
				property float Progression { float get(); }
				///<summary>The frequency ratio.</summary>
				property float FrequencyRatio;		
				///<summary>The source that created the sound.</summary>
				property Draw8::Audio::ISoundSource^ Source { Draw8::Audio::ISoundSource^ get(); }
				///<summary>True if the sound is going to loop at its end. False otherwise.</summary>
				property bool Looped { bool get(); }
				///<summary>True if the sound is currently playing. False otherwise.</summary>
				property bool IsPlaying { bool get(); }
				///<summary>A tag that can be set by the user</summary>
				property Object^ Tag;

			public:
				///<summary>Starts the sound.</summary>
				void Play();
				///<summary>Suspends the sound.</summary>
				///<param name="ProcessEffects">True if you want to process the tails of the effects. False elsewise.</param>
				void Suspend(bool ProcessEffects);

			public:
				///<summary>Called when a loop ends and then restarts after.</summary>
				event LoopEndEventHandler^ LoopEnd
				{
					Windows::Foundation::EventRegistrationToken add(LoopEndEventHandler^ Handler);
					void remove(Windows::Foundation::EventRegistrationToken Token);
					void raise(ISound^ Sender, Platform::Object^ Args);
				}
				///<summary>Called when a part of the sound begin. If the sound is monopart (Sound), this event
				///is invoked when the sound begin. If the sound is multipart (CompositeSound), it is invoked when a part of the sound
				///starts.</summary>
				event PartBeginEventHandler^ PartBegin
				{
					Windows::Foundation::EventRegistrationToken add(PartBeginEventHandler^ Handler);
					void remove(Windows::Foundation::EventRegistrationToken Token);
					void raise(ISound^ Sender, Platform::Object^ Args);
				}
				///<summary>Called when a part of the sound ends. If the sound is monopart (Sound), this event
				///is invoked when the sound ends. If the sound is multipart (CompositeSound), it is invoked when a part of the sound
				///ends.</summary>
				event PartEndEventHandler^ PartEnd
				{
					Windows::Foundation::EventRegistrationToken add(PartEndEventHandler^ Handler);
					void remove(Windows::Foundation::EventRegistrationToken Token);
					void raise(ISound^ Sender, Platform::Object^ Args);
				}
		};
	}
}

#endif