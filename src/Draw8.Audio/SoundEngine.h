#include "pch.h"

#ifndef DEF_SOUNDENGINE
#define DEF_SOUNDENGINE

namespace Draw8
{
	namespace Audio
	{
		///<summary>A class representing the Sound engine.</summary>
		public ref class SoundEngine sealed
		{
			private:
				//Contains the XAudio device
				Microsoft::WRL::ComPtr<IXAudio2> m_xaudio2;
				//Contains the XAudio mastering voice
				IXAudio2MasteringVoice* m_masteringVoice;
				//Contains the endpoint
				Draw8::IDevice^ m_outputDevice;
			
			public:
				///<summary>Is the Sound Engine started ?</summary>
				property bool IsStarted;
				///<summary>The actual sound volume presented like a multiplier : 0 is silence, 1 is no attenuation</summary>
				property float GlobalVolume { float get(); void set(float value); }	
				///<summary>The audio output device. Null is default device (usually speakers/headphones).</summary>
				property IDevice^ OutputDevice { IDevice^ get() { return m_outputDevice; } }

			internal:
				//Returns the mastering voice
				IXAudio2MasteringVoice* GetMasteringVoice() { return m_masteringVoice; }
				//Returns the engine
				Microsoft::WRL::ComPtr<IXAudio2> GetXAudioEngine() { return m_xaudio2; }
			public:
				///<summary>Starts the engine. When created, the engine is automatically started.</summary>
				void Start();
				///<summary>Stops the engine.
				///			Call this method to save battery on the user's device if you're not willing to play background music. 
				///			When restarted, the engine will act as if it has never been paused.
				///			Do not call this method as a way to pause songs.</summary>
				void Suspend();

			public:
				///<summary>Creates the Sound engine. The engine is created started : there is no need to call Start(). The output of the sound will be the default output.</summary>
				SoundEngine();
				///<summary>Creates the Sound engine. The engine is created started : there is no need to call Start().</summary>
				///<param name="OutputDevice">The audio output device. Use null for default device (usually speakers/headphones).</param>
				SoundEngine(IDevice^ OutputDevice);

			private:
				//Constructor delocated
				void Create(IDevice^ OutputDevice);

		};
	}
}

#endif
