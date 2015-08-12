#include "pch.h"

#ifndef DEF_SOUNDEFFECT
#define DEF_SOUNDEFFECT

#include "ISoundEffectProcessor.h"
#include "SoundEffectParameterCollection.h"
#include "AudioTools.h"

namespace Draw8
{
	namespace Audio
	{

		///<summary>A sound effect.</summary>
		public ref class SoundEffect sealed
		{
			private:
				//The params size
				uint m_paramsSize;
				//The params themselves
				void* m_paramsData;
				//The XAPO
				IUnknown* m_xapo;
				//The vector containing owners
				std::vector<Platform::WeakReference> m_owners;
				//The properties
				SoundEffectParameterCollection^ m_params;
				//The used params
				RawParameterDescriptor* m_usedParams;
				//The number of them
				uint m_usedParamsCount;

			public:
				///<summary>The effect parameters.</summary>
				property SoundEffectParameterCollection^ Parameters { SoundEffectParameterCollection^ get() { return m_params; } }

			public:
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Creates a prebuilt sound effect.</summary>
				SoundEffect(Platform::String^ EffectName);
				///<summary>Creates a custom sound effect.</summary>
				SoundEffect(ISoundEffectProcessor^ Processor);

			public:
				///<summary>Disposes of the effect.</summary>
				virtual ~SoundEffect();

			private:
				//When parameters changed
				void OnParamsChanged(Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ sender, Windows::Foundation::Collections::IMapChangedEventArgs<Platform::String^>^ args);

			internal:
				//Add an owner
				bool AddOwner(Platform::WeakReference Owner);
				//Remove an owner
				bool RemoveOwner(Platform::WeakReference Owner);
				//Returns data
				IUnknown* GetXAPO() { return m_xapo; }
				void* GetParams() { return m_paramsData; }
				uint GetParamsSize() { return m_paramsSize; }
		};
	}
}

#endif