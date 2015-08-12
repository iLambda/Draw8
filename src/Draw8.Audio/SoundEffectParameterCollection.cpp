#include "pch.h"
#include "SoundEffectParameterCollection.h"

Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^>^ Draw8::Audio::SoundEffectParameterCollection::First()
{
	//Returns an iterator
	return ref new SoundEffectParameterCollectionIterator(&m_params);
}

void Draw8::Audio::SoundEffectParameterCollection::Clear()
{
	//Clears
	m_params.clear();
	MapChanged(this, ref new SoundEffectParameterCollectionChangedEventArgs(nullptr, Windows::Foundation::Collections::CollectionChange::Reset));
}

Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ Draw8::Audio::SoundEffectParameterCollection::GetView()
{
	//Returns map
	return ref new Draw8::Audio::SoundEffectParameterCollectionView(m_params);
}

bool Draw8::Audio::SoundEffectParameterCollection::HasKey(Platform::String^ Key)
{
	//Tells
	return m_params.find(Key) != m_params.end();
}

bool Draw8::Audio::SoundEffectParameterCollection::Insert(Platform::String^ Key, Platform::Object^ Value)
{
	//Insert value
	auto tmp = m_params.find(Key) != m_params.end();
	m_params[Key] = Value;
	MapChanged(this, ref new SoundEffectParameterCollectionChangedEventArgs(Key, tmp ? Windows::Foundation::Collections::CollectionChange::ItemChanged : Windows::Foundation::Collections::CollectionChange::ItemInserted));
	return tmp;
}

bool Draw8::Audio::SoundEffectParameterCollection::InsertWithoutRaise(Platform::String^ Key, Platform::Object^ Value)
{
	//Insert value
	auto tmp = m_params.find(Key) != m_params.end();
	m_params[Key] = Value;
	return tmp;
}

Platform::Object^ Draw8::Audio::SoundEffectParameterCollection::Lookup(Platform::String^ Key)
{
	//Returns obj
	if(!HasKey(Key))
		throw ref new Platform::OutOfBoundsException("The given key was not found in the dictionary");
	return m_params[Key];
}

void Draw8::Audio::SoundEffectParameterCollection::Remove(Platform::String^ Key)
{
	//Removes key
	if(!HasKey(Key))
		throw ref new Platform::OutOfBoundsException("The given key was not found in the dictionary");
	MapChanged(this, ref new SoundEffectParameterCollectionChangedEventArgs(Key, Windows::Foundation::Collections::CollectionChange::ItemRemoved));
	m_params.erase(Key);
}

Platform::Object^ Draw8::Audio::SoundEffectParameterCollectionView::Lookup(Platform::String^ Key)
{
	//Returns obj
	if(!HasKey(Key))
		throw ref new Platform::OutOfBoundsException("The given key was not found in the dictionary");
	return m_params[Key];
}

void Draw8::Audio::SoundEffectParameterCollectionView::Split(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^* First, Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^* Second)
{
	//Creates maps
	auto map1 = std::map<Platform::String^, Platform::Object^>();
	auto map2 = std::map<Platform::String^, Platform::Object^>();
	auto split = (int)floor(Size / 2.f);

	//Fills them
	int i = 0;
	for (auto it = m_params.begin() ; it != m_params.end() ; ++it)
		(i++ >= split ? map2 : map1)[it->first] = it->second;

	//Returns them
	*First = ref new SoundEffectParameterCollectionView(map1);
	*Second = ref new SoundEffectParameterCollectionView(map2);
	return;
}