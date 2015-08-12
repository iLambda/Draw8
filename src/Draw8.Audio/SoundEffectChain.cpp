#include "pch.h"
#include "SoundEffectChain.h"
#include "SoundEffect.h"
#include "ISoundEmitter.h"

Draw8::Audio::SoundEffectChain::SoundEffectChain(IXAudio2Voice** Voice, Channels Channels) { m_voice = Voice; m_channels = Channels; }

Draw8::Audio::SoundEffectChain::~SoundEffectChain()
{
	//Removes the references stored
	for (uint i = 0; i < Length; i++)
		m_chain[i] = nullptr;
}

int Draw8::Audio::SoundEffectChain::Add(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null", E_INVALIDARG);

	//Adds the effect
	m_chain.push_back(Effect);

	//Collection changed
	CollectionChanged();

	//We notify the effect
	Effect->AddOwner(Platform::WeakReference(this));

	//Returns the ID
	return Length - 1;
}

int Draw8::Audio::SoundEffectChain::Insert(int Index, Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);

	//If index is superior to length
	if(m_chain.begin() + Index > m_chain.end())
		throw ref new Platform::OutOfBoundsException("The index was out of the SoundEffectChain range (length is " + Length + ")");

	//Adds the effect
	m_chain.insert(m_chain.begin() + Index, Effect);

	//Collection changed
	CollectionChanged();

	//We notify the effect
	Effect->AddOwner(Platform::WeakReference(this));

	//Returns the ID
	return Index;
}

int Draw8::Audio::SoundEffectChain::Insert(Draw8::Audio::SoundEffect^ Effect, Draw8::Audio::SoundEffect^ InsertedEffect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);
	Draw8::ThrowIf(InsertedEffect == nullptr, "InsertedEffect is null.", E_INVALIDARG);

	//Gets the iterator
	auto iterator = std::find(m_chain.begin(), m_chain.end(), Effect);

	//If not present
	if(iterator == m_chain.end())
		throw ref new Platform::InvalidArgumentException("The given effect was not present in the SoundEffectChain.");

	//Computes pos
	auto pos = iterator - m_chain.begin();

	//Adds the effect
	m_chain.insert(iterator, InsertedEffect);

	//Collection changed
	CollectionChanged();

	//We notify the effect
	InsertedEffect->AddOwner(Platform::WeakReference(this));

	//Returns the ID
	return pos;
}

bool Draw8::Audio::SoundEffectChain::Contains(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);

	//Returns
	return (std::find(m_chain.begin(), m_chain.end(), Effect) != m_chain.end());
}

int Draw8::Audio::SoundEffectChain::IndexOf(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);

	//Gets the iterator
	auto iterator = std::find(m_chain.begin(), m_chain.end(), Effect);

	//If not present
	if(iterator == m_chain.end())
		throw ref new Platform::InvalidArgumentException("The given effect was not present in the SoundEffectChain.");

	//Returns the ID
	return iterator - m_chain.begin();
}

void Draw8::Audio::SoundEffectChain::Remove(int Index)
{
	//If index is superior to length
	if(m_chain.begin() + Index >= m_chain.end())
		throw ref new Platform::OutOfBoundsException("The index was out of the SoundEffectChain range (length is " + Length + ")");

	//We notify the effect
	(*(m_chain.begin() + Index))->RemoveOwner(Platform::WeakReference(this));

	//Removes
	m_chain.erase(m_chain.begin() + Index);

	//Collection changed
	CollectionChanged();
}

void Draw8::Audio::SoundEffectChain::Remove(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);

	//Gets the iterator
	auto iterator = std::find(m_chain.begin(), m_chain.end(), Effect);

	//If not present
	if(iterator == m_chain.end())
		throw ref new Platform::InvalidArgumentException("The given effect was not present in the SoundEffectChain.");

	//We notify the effect
	Effect->RemoveOwner(Platform::WeakReference(this));

	//Removes
	m_chain.erase(iterator);

	//Collection changed
	CollectionChanged();
}

void Draw8::Audio::SoundEffectChain::Replace(int Index, Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "Effect is null.", E_INVALIDARG);

	//Throw for errors
	if(Index >= (int)Length)
		throw ref new Platform::OutOfBoundsException("FirstIndex was out of the SoundEffectChain range (length is " + Length + ")");

	//We notify the effect
	m_chain[Index]->RemoveOwner(Platform::WeakReference(this));

	//Replaces
	m_chain[Index] = Effect;

	//Collection changed
	CollectionChanged();

	//We notify the effect
	Effect->AddOwner(Platform::WeakReference(this));
}

void Draw8::Audio::SoundEffectChain::Replace(Draw8::Audio::SoundEffect^ ReplacedEffect, Draw8::Audio::SoundEffect^ ReplacingEffect)
{
	//Assert if effect is null
	Draw8::ThrowIf(ReplacedEffect == nullptr, "ReplacedEffect is null.", E_INVALIDARG);
	Draw8::ThrowIf(ReplacingEffect == nullptr, "ReplacingEffect is null.", E_INVALIDARG);

	//Gets iterator
	auto iterator = std::find(m_chain.begin(), m_chain.end(), ReplacedEffect);
	
	//Throw for errors
	if(iterator == m_chain.end())
		throw ref new Platform::OutOfBoundsException("FirstEffect was not present in the SoundEffectChain.");

	//We notify the effect
	ReplacedEffect->RemoveOwner(Platform::WeakReference(this));

	//Replaces
	m_chain[iterator - m_chain.begin()] = ReplacingEffect;

	//Collection changed
	CollectionChanged();

	//We notify the effect
	ReplacingEffect->AddOwner(Platform::WeakReference(this));
}

void Draw8::Audio::SoundEffectChain::Swap(int FirstIndex, int SecondIndex)
{
	//Throw for errors
	if((uint)FirstIndex >= Length)
		throw ref new Platform::OutOfBoundsException("FirstIndex was out of the SoundEffectChain range (length is " + Length + ")");
	if((uint)SecondIndex >= Length)
		throw ref new Platform::OutOfBoundsException("SecondIndex was out of the SoundEffectChain range (length is " + Length + ")");

	//Swap items
	std::iter_swap(m_chain.begin() + FirstIndex, m_chain.begin() + SecondIndex);

	//Collection changed
	CollectionChanged();

	//Swap manually all params
	if(*m_voice != NULL)
	{
		//Swap params
		(*m_voice)->SetEffectParameters(FirstIndex, m_chain.at(FirstIndex)->GetParams(), m_chain.at(FirstIndex)->GetParamsSize());
		(*m_voice)->SetEffectParameters(SecondIndex, m_chain.at(SecondIndex)->GetParams(), m_chain.at(SecondIndex)->GetParamsSize());
	}
}

void Draw8::Audio::SoundEffectChain::Swap(Draw8::Audio::SoundEffect^ FirstEffect, Draw8::Audio::SoundEffect^ SecondEffect)
{
	//Assert if effect is null
	Draw8::ThrowIf(FirstEffect == nullptr, "FirstEffect is null.", E_INVALIDARG);
	Draw8::ThrowIf(SecondEffect == nullptr, "SecondEffect is null.", E_INVALIDARG);

	//Gets the iterator
	auto iterator1 = std::find(m_chain.begin(), m_chain.end(), FirstEffect);
	auto iterator2 = std::find(m_chain.begin(), m_chain.end(), SecondEffect);

	//Throw for errors
	if(iterator1 == m_chain.end())
		throw ref new Platform::OutOfBoundsException("FirstEffect was not present in the SoundEffectChain.");
	if(iterator2 == m_chain.end())
		throw ref new Platform::OutOfBoundsException("SecondEffect was not present in the SoundEffectChain.");

	//Swap items
	std::iter_swap(iterator1, iterator2);

	//Collection changed
	CollectionChanged();

	//Swap manually all params
	if(*m_voice != NULL)
	{
		//Swap params
		(*m_voice)->SetEffectParameters(IndexOf(FirstEffect), FirstEffect->GetParams(), FirstEffect->GetParamsSize());
		(*m_voice)->SetEffectParameters(IndexOf(SecondEffect), SecondEffect->GetParams(), SecondEffect->GetParamsSize());
	}
}

void Draw8::Audio::SoundEffectChain::Activate(int Index)
{
	//Activate
	if(*m_voice != NULL)
		(*m_voice)->EnableEffect(Index);
}

void Draw8::Audio::SoundEffectChain::Activate(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "FirstEffect is null.", E_INVALIDARG);

	//Activate
	if(*m_voice != NULL)
		(*m_voice)->EnableEffect(IndexOf(Effect));
}
			
void Draw8::Audio::SoundEffectChain::Deactivate(int Index)
{
	//Test bounds
	if(!(Index >= 0 && (uint)Index < Length))
		throw ref new Platform::OutOfBoundsException("Index is out of bounds.");

	//Activate
	if(*m_voice != NULL)
		(*m_voice)->DisableEffect(Index);
}

void Draw8::Audio::SoundEffectChain::Deactivate(Draw8::Audio::SoundEffect^ Effect)
{
	//Assert if effect is null
	Draw8::ThrowIf(Effect == nullptr, "FirstEffect is null.", E_INVALIDARG);

	//Activate
	if(*m_voice != NULL)
		(*m_voice)->DisableEffect(IndexOf(Effect));
}

Draw8::Audio::SoundEffect^ Draw8::Audio::SoundEffectChain::At(int Index)
{
	//Test bounds
	if(!(Index >= 0 && (uint)Index < Length))
		throw ref new Platform::OutOfBoundsException("Index is out of bounds.");

	//Returns the element
	return m_chain.at(Index);
}

void Draw8::Audio::SoundEffectChain::Clear()
{
	//Clear
	m_chain.clear();

	//Collection changed
	CollectionChanged();
}

void Draw8::Audio::SoundEffectChain::CollectionChanged()
{
	//If voice is dead
	if(*m_voice == NULL)
		return;

	//If length is now null
	if(Length != 0)
	{
		//Creates the effects
		std::vector<XAUDIO2_EFFECT_DESCRIPTOR> effects;
		for(int i = 0 ; i != Length ; i++)
		{
			XAUDIO2_EFFECT_DESCRIPTOR desc;
			desc.InitialState = TRUE;
			desc.OutputChannels = (int)m_channels;
			desc.pEffect = m_chain.at(i)->GetXAPO();
			effects.push_back(desc);
		}

		//The descriptor
		XAUDIO2_EFFECT_CHAIN desc = { Length, &effects[0] };

		//Set the chain
		(*m_voice)->SetEffectChain(&desc);
	}	
	else
		(*m_voice)->SetEffectChain(NULL);
}

void Draw8::Audio::SoundEffectChain::EffectUpdate(SoundEffect^ EffectUpdated)
{
	//Checks voice
	if(*m_voice == NULL)
		return;

	//Get index and update
	(*m_voice)->SetEffectParameters((uint)IndexOf(EffectUpdated), EffectUpdated->GetParams(), EffectUpdated->GetParamsSize());
}

