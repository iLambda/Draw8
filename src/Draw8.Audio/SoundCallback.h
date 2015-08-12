#include "pch.h"
#include "ISound.h"

#ifndef DEF_SOUNDCALLBACK
#define DEF_SOUNDCALLBACK

namespace Draw8
{
	namespace Audio
	{
		class SoundCallback : public IXAudio2VoiceCallback
		{
			private:
				//The emitter
				Platform::WeakReference m_emitter;

			public:
				//The default ctor
				SoundCallback() { }
				~SoundCallback() { m_emitter = nullptr; }

			public:
				//Set emitter
				void SetEmitter(Platform::WeakReference Emitter) { m_emitter = Emitter; }
				
			public:
				void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext);							//Used
				void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext);						//Used
				void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext);						    //Used
				 
				void STDMETHODCALLTYPE OnStreamEnd();												//Unused
				void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();									//Unused
				void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired);			//Unused
				void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT E11rror);		//Unused
		};
	}
}

#endif