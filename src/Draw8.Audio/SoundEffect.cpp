#include "pch.h"
#include "SoundEffect.h"
#include "SoundEffectChain.h"
#include "AudioTools.h"

Draw8::Audio::SoundEffect::SoundEffect(Platform::String^ EffectName)
{
	//We subscribe
	m_params = ref new SoundEffectParameterCollection();
	m_params->MapChanged += ref new Windows::Foundation::Collections::MapChangedEventHandler<Platform::String^, Platform::Object^>(this, &Draw8::Audio::SoundEffect::OnParamsChanged);

	//Creating the guid
	GUID uuid;

	//Chooses the type
	if(EffectName == "Reverb")
	{
		//Set type
		uuid = __uuidof(FXReverb);

		//Set list of used params
		m_usedParams = new RawParameterDescriptor[2];
		m_usedParams[0].Name = L"Diffusion";
		m_usedParams[0].Size = sizeof(float);
		m_usedParams[1].Name = L"RoomSize";
		m_usedParams[1].Size = sizeof(float);
		m_usedParamsCount = 2;
		
		//Set params size
		m_paramsSize = 2*sizeof(float);

		//Set default params
		m_params->InsertWithoutRaise("Diffusion", FXREVERB_DEFAULT_DIFFUSION);
		m_params->InsertWithoutRaise("RoomSize",  FXREVERB_DEFAULT_ROOMSIZE);
	}
	else if(EffectName == "Echo")
	{
		//Set type
		uuid = __uuidof(FXEcho);

		//Set list of used params
		m_usedParams = new RawParameterDescriptor[3];
		m_usedParams[0].Name = L"Delay";
		m_usedParams[0].Size = sizeof(float);
		m_usedParams[1].Name = L"Feedback";
		m_usedParams[1].Size = sizeof(float);
		m_usedParams[2].Name = L"WetDryMix";
		m_usedParams[2].Size = sizeof(float);
		m_usedParamsCount = 3;

		//Set params size
		m_paramsSize = 3*sizeof(float);

		//Set default params
		m_params->InsertWithoutRaise("Delay", FXECHO_DEFAULT_DELAY);
		m_params->InsertWithoutRaise("Feedback",  FXECHO_DEFAULT_FEEDBACK);
		m_params->InsertWithoutRaise("WetDryMix",  FXECHO_DEFAULT_WETDRYMIX);
	}
	else
		throw ref new Platform::InvalidArgumentException("This effect is not a correct prebuilt effect name.");

	//Create XAPO
	CreateFX(uuid, &m_xapo);

	//Creates parameters
	m_paramsData = new byte[m_paramsSize];
	OnParamsChanged(nullptr, nullptr);
}
				
Draw8::Audio::SoundEffect::SoundEffect(ISoundEffectProcessor^ Processor)
{
	//Not implemented now
	throw ref new Platform::NotImplementedException("Custom user effects are not supported at this time.");
}

Draw8::Audio::SoundEffect::~SoundEffect()
{
	//Delete strings
	for(int i = 0 ; i != m_usedParamsCount ; i++)
		delete m_usedParams[i].Name;

	//Delete array of params
	delete[] m_usedParams;
	delete[] m_paramsData;
}

bool Draw8::Audio::SoundEffect::AddOwner(Platform::WeakReference Reference)
{
	//If null, nothing happens
	if(Reference.Resolve<SoundEffectChain>() == nullptr)
		return false;

	//Check if in list
	auto it = std::find(m_owners.begin(), m_owners.end(), Reference);

	//If in list
	if(it != m_owners.end())
		return false;

	//Else
	m_owners.push_back(Reference);

	//Need update on voice
	Reference.Resolve<SoundEffectChain>()->EffectUpdate(this);

	//Success
	return true;
}

bool Draw8::Audio::SoundEffect::RemoveOwner(Platform::WeakReference Reference)
{
	//If null, nothing happens
	if(Reference.Resolve<SoundEffectChain>() == nullptr)
		return false;

	//Check if in list
	auto it = std::find(m_owners.begin(), m_owners.end(), Reference);

	//If not in list
	if(it == m_owners.end())
		return false;

	//Else
	m_owners.erase(it);

	//Success
	return true;
}

void Draw8::Audio::SoundEffect::OnParamsChanged(Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ sender, Windows::Foundation::Collections::IMapChangedEventArgs<Platform::String^>^ args)
{
	//Tests if selected param
	if(args != nullptr)
	{
		auto key = args->Key->Data();
		if(!(std::find_if(m_usedParams, m_usedParams + m_paramsSize, [key](RawParameterDescriptor Desc) { return wcscmp(Desc.Name, key) == 0; }) != (m_usedParams + m_paramsSize)))
			return;
	}

	//Fills data structure
	int i = 0;
	int j = 0;
	while(i != m_paramsSize)
	{
		//Adds data
		auto key = ref new Platform::String(m_usedParams[j].Name);
		if(m_params->HasKey(key))
		{
			auto obj = m_params->Lookup(key);
			Draw8::Cast(obj, (((byte*)m_paramsData) + i));
		}
		else
		{
			for(int k = 0 ; k != m_usedParams[j].Size ; k++)
				(*(((byte*)m_paramsData) + i + k)) = 0;
		}
		
		//Increment indexes ; will fail if m_usedParams is badly initialized
		i += m_usedParams[j].Size;
		j++;
	}

	//Calls all methods
	for (auto it = m_owners.begin(); it != m_owners.end(); ++it) 
	{
		//We call methods
		if((*it).Resolve<SoundEffectChain>() != nullptr)
			(*it).Resolve<SoundEffectChain>()->EffectUpdate(this);
	}
}