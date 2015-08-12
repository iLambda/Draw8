#include "pch.h"

#ifndef DEF_SOUNDEFFECTPARAMETERS
#define DEF_SOUNDEFFECTPARAMETERS

namespace Draw8
{
	namespace Audio
	{
		///<summary>A sound effect parameter.</summary>
		public ref class SoundEffectParameter sealed : public Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>
		{
			private:
				//The key
				Platform::String^ m_key;
				//The value
				Platform::Object^ m_value;

			public:
				///<summary>The key.</summary>
				property Platform::String^ Key { virtual Platform::String^ get() { return m_key; } } 
				///<summary>The value.</summary>
				property Platform::Object^ Value { virtual Platform::Object^ get() { return m_value; } } 

			public:
				///<summary>Creates a parameter</summary>
				SoundEffectParameter(Platform::String^ Key, Platform::Object^ Value) : m_key(Key), m_value(Value) {}

			internal:
				//Creates a parameter
				SoundEffectParameter(std::map<Platform::String^, Platform::Object^>::iterator it) : m_key((*it).first), m_value((*it).second) {}
				//Creates a parameter
				SoundEffectParameter(std::map<Platform::String^, Platform::Object^>::value_type it) : m_key(it.first), m_value(it.second) {}
		};

		///<summary>An iterator for the class SoundEffectParameterCollection.</summary>
		public ref class SoundEffectParameterCollectionIterator sealed : public Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^>
		{
			private:
				//The iterator
				std::map<Platform::String^, Platform::Object^>::iterator m_iterator;
				//The owner
				std::map<Platform::String^, Platform::Object^>* m_owner;

			public:
				///<summary>Returns the current element.</summary>
				property Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^ Current { virtual Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^ get() { return ref new SoundEffectParameter(m_iterator); } }
				///<summary>Returns the validity of the iterator</summary>
				property bool HasCurrent { virtual bool get() { return m_iterator != m_owner->end(); } }

			internal:
				//Creates an iterator
				SoundEffectParameterCollectionIterator(std::map<Platform::String^, Platform::Object^>* Owner) { m_owner = Owner; m_iterator = m_owner->begin(); }

			public:
				///<summary>Returns all the elements of the collection.</summary>
				///<param name="Items">The whole collection.</param>
				///<returns>The collection count.</returns>
				virtual uint GetMany(Platform::WriteOnlyArray<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^>^ Items) 
				{  
					//Copies into array
					int i = 0;
					for(auto it = m_owner->begin() ; it != m_owner->end() ; ++it)
						Items->set(i++, ref new SoundEffectParameter(it));

					//Returns the size
					return m_owner->size(); 
				}
				///<summary>Returns the next iterator.</summary>
				///<returns>True if valid, false elsewise.</returns>
				virtual bool MoveNext() { return ++m_iterator != m_owner->end(); }

		};

		///<summary>An immutable view of the parameters of a sound effect.</summary>
		public ref class SoundEffectParameterCollectionView sealed : Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>
		{
			private:
				//The underlying map
				std::map<Platform::String^, Platform::Object^> m_params;

			public:
				///<summary>The number of parameters.</summary>
				property unsigned int Size { virtual unsigned int get() { return (uint)m_params.size(); } }
			
			internal:
				//Creates the view
				SoundEffectParameterCollectionView(std::map<Platform::String^, Platform::Object^> Params) : m_params(Params) {}

			public:
				///<summary>Returns an iterator to the first element.</summary>
				///<returns>The iterator.</returns>
				virtual Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^>^ First() { return ref new SoundEffectParameterCollectionIterator(&m_params); }
				///<summary>Indicates if the parameters exists.</summary>
				///<returns>True if present, false if not.</returns>
				virtual bool HasKey(Platform::String^ Key) { return m_params.find(Key) != m_params.end(); }
				///<summary>Retries an object with its key.</summary>
				///<param name="Key">The key.</param>
				///<returns>The element.</returns>
				virtual Platform::Object^ Lookup(Platform::String^ Key);
				///<summary>Splits the immutable map.</summary>
				///<param name="First">The first immutable map.</param>
				///<param name="Second">The second immutable map.</param>
				virtual void Split(Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^* First, Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^* Second);
		};

		///<summary>The parameters of a sound effect.</summary>
		public ref class SoundEffectParameterCollectionChangedEventArgs sealed : Windows::Foundation::Collections::IMapChangedEventArgs<Platform::String^>
		{
			private:
				//The key
				Platform::String^ m_key;
				//The given change
				Windows::Foundation::Collections::CollectionChange m_change;

			public:
				///<summary>The dictionary key where the change occured.</summary>
				property Platform::String^ Key { virtual Platform::String^ get() { return m_key; } }
				///<summary>The kind of change that occured.</summary>
				property Windows::Foundation::Collections::CollectionChange CollectionChange { virtual Windows::Foundation::Collections::CollectionChange get() { return m_change; } }

			internal:
				//Creates the args
				SoundEffectParameterCollectionChangedEventArgs(Platform::String^ Key, Windows::Foundation::Collections::CollectionChange CollectionChange) : m_key(Key), m_change(CollectionChange) {}
		};

		///<summary>The parameters of a sound effect.</summary>
		public ref class SoundEffectParameterCollection sealed : Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>
		{
			private:
				//The underlying map
				std::map<Platform::String^, Platform::Object^> m_params;

			public:
				///<summary>The number of parameters.</summary>
				property unsigned int Size { virtual unsigned int get() { return (uint)m_params.size(); } }

			public:
				///<summary>Returns an iterator to the first element.</summary>
				///<returns>The iterator.</returns>
				virtual Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IKeyValuePair<Platform::String^, Platform::Object^>^>^ First();
				///<summary>Clears the parameters.</summary>
				virtual void Clear();
				///<summary>Returns an immutable view of the map.</summary>
				///<returns>The immutable view.</returns>
				virtual Windows::Foundation::Collections::IMapView<Platform::String^, Platform::Object^>^ GetView();
				///<summary>Indicates if the parameters exists.</summary>
				///<returns>True if present, false if not.</returns>
				virtual bool HasKey(Platform::String^ Key);
				///<summary>Inserts a parameter. If the key already exists, the element will be replaced.</summary>
				///<param name="Key">The key.</param>
				///<param name="Value">The value to insert.</param>
				///<returns>True if the insertion replaced an element. False elsewise.</returns>
				virtual bool Insert(Platform::String^ Key, Platform::Object^ Value);
				///<summary>Retries an object with its key.</summary>
				///<param name="Key">The key.</param>
				///<returns>The element.</returns>
				virtual Platform::Object^ Lookup(Platform::String^ Key);
				///<summary>Removes an object with its key.</summary>
				///<param name="Key">The key.</param>
				virtual void Remove(Platform::String^ Key);

			internal:
				bool InsertWithoutRaise(Platform::String^ Key, Platform::Object^ Value);

			public:
				///<summary>Called when map event changes.</summary>
				virtual event Windows::Foundation::Collections::MapChangedEventHandler<Platform::String^, Platform::Object^>^ MapChanged;

		};
	}
}

#endif