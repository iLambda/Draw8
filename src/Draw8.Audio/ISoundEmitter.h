#include "pch.h"

#ifndef DEF_ISOUNDEMITTER
#define DEF_ISOUNDEMITTER

#include "AudioTools.h"

namespace Draw8
{
	namespace Audio
	{
		ref class SoundEffectChain;

		///<summary>An object that produces sound.</summary>
		public interface class ISoundEmitter
		{
			public:
				///<summary>The actual sound volume presented like a multiplier : 0 is silence, 1 is no attenuation.</summary>
				property float Volume;  
				///<summary>The sound filter</summary>
				property SoundFilter^ Filter;
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { Draw8::Audio::Channels get(); }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { int get(); }
				///<summary>The effects applied to the sound output.</summary>
				property SoundEffectChain^ Effects { SoundEffectChain^ get(); }
		};
	}
}

#endif