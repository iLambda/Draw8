#include "pch.h"
#include "SoundCallback.h"

void Draw8::Audio::SoundCallback::OnBufferEnd(void * pBufferContext)
{
	if(m_emitter.Resolve<ISound>() != nullptr)
		m_emitter.Resolve<ISound>()->PartEnd(m_emitter.Resolve<ISound>(), nullptr);
}

void Draw8::Audio::SoundCallback::OnBufferStart(void * pBufferContext)
{
	if(m_emitter.Resolve<ISound>() != nullptr)
		m_emitter.Resolve<ISound>()->PartBegin(m_emitter.Resolve<ISound>(), nullptr);
}

void Draw8::Audio::SoundCallback::OnLoopEnd(void * pBufferContext)
{
	if(m_emitter.Resolve<ISound>() != nullptr)
		m_emitter.Resolve<ISound>()->LoopEnd(m_emitter.Resolve<ISound>(), nullptr);
}		
				
 
void Draw8::Audio::SoundCallback::OnStreamEnd() {}
void Draw8::Audio::SoundCallback::OnVoiceProcessingPassEnd() {}
void Draw8::Audio::SoundCallback::OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
void Draw8::Audio::SoundCallback::OnVoiceError(void * pBufferContext, HRESULT Error) {}