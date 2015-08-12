#include "pch.h"

#ifndef DEF_ISOUNDEFFECTPROCESSOR
#define DEF_ISOUNDEFFECTPROCESSOR

#include "SoundEffectParameterCollection.h"

namespace Draw8
{
	namespace Audio
	{
		///<summary>A sound effect.</summary>
		public interface class ISoundEffectProcessor
		{
			public:
				///<summary>The default parameters of the filter.</summary>
				property Platform::Array<SoundEffectParameter^>^ Parameters { Platform::Array<SoundEffectParameter^>^ get(); }
				
			public:
				///<summary>The function that treats data</summary>
				void Process();
		};
	}
}

#endif