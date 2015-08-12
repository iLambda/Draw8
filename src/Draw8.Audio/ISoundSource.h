#include "pch.h"

#ifndef DEF_ISOUNDSOURCE
#define DEF_ISOUNDSOURCE

#include "SoundData.h"
#include "SoundEngine.h"

namespace Draw8
{
	namespace Audio
	{
		interface class ISound;

		///<summary>The source of a sound. Can instanciate sounds.</summary>
		public interface class ISoundSource
		{
			public:
				///<summary>Defines if the sound is Mono or Stereo. Default is Mono.</summary>
				property Draw8::Audio::Channels Channels { Draw8::Audio::Channels get(); }
				///<summary>Contains the sampling rate (the number of samples played in one second).</summary>
				property int SampleRate { int get(); }
		};
	}
}

#endif