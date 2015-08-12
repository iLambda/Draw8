#include "pch.h"

#ifndef DEF_SOUNDEFFECTCHAIN
#define DEF_SOUNDEFFECTCHAIN

#include "AudioTools.h"
#include "SoundEffect.h"

namespace Draw8
{
	namespace Audio
	{
		///<summary>An iterator for the class SoundEffectChain.</summary>
		public ref class SoundEffectChainIterator sealed : public Windows::Foundation::Collections::IIterator<SoundEffect^>
		{
			private:
				//The iterator
				std::vector<SoundEffect^>::iterator m_iterator;
				//The owner
				std::vector<SoundEffect^>* m_owner;

			public:
				///<summary>Returns the current element.</summary>
				property SoundEffect^ Current { virtual SoundEffect^ get() { return *m_iterator; } }
				///<summary>Returns the validity of the iterator</summary>
				property bool HasCurrent { virtual bool get() { return m_iterator != m_owner->end(); } }

			internal:
				//Creates an iterator
				SoundEffectChainIterator(std::vector<SoundEffect^>* Owner) { m_owner = Owner; m_iterator = m_owner->begin(); }

			public:
				///<summary>Returns all the elements of the collection.</summary>
				///<param name="Items">The whole collection.</param>
				///<returns>The collection count.</returns>
				virtual uint GetMany(Platform::WriteOnlyArray<SoundEffect^>^ Items) 
				{  
					//Copies into array
					for(int i = 0 ; i != m_owner->size() ; i++)
						Items->set(i, m_owner->at(i));

					//Returns the size
					return m_owner->size(); 
				}
				///<summary>Returns the next iterator.</summary>
				///<returns>True if valid, false elsewise.</returns>
				virtual bool MoveNext() { return ++m_iterator != m_owner->end(); }

		};

		///<summary>A chain containing the effects applied to a sound. Whenever you have the choice, always prefer using SoundEffectChain methods instead of LINQ (e.g. At() instead of ElementAt())
		///: the methods SoundEffectChain has are a lot more optimized than LINQ's ones. </summary>
		public ref class SoundEffectChain sealed : public Windows::Foundation::Collections::IIterable<SoundEffect^>
		{
			private:
				//The underlying vector
				std::vector<SoundEffect^> m_chain;
				//The reference
				IXAudio2Voice** m_voice;
				//The channels number
				Channels m_channels;

			public:
				///<summary>The length of the collection.</summary>
				property uint Length { uint get() { return m_chain.size(); } }
				///<summary>Is the collection empty ?</summary>
				property bool IsEmpty { bool get() { return m_chain.size() == 0; } }

			internal:
				//The constructor. Must link to the owner
				SoundEffectChain(IXAudio2Voice** Voice, Channels Channels);
			private:
				//The destructor
				~SoundEffectChain();

			public:
				///<summary>Returns an iterator to the first element.</summary>
				///<returns>The iterator.</returns>
				virtual Windows::Foundation::Collections::IIterator<SoundEffect^>^ First() { return ref new SoundEffectChainIterator(&m_chain); }

				///<summary>Adds an effect at the end of the chain. Returns the index.</summary>
				///<param name="Effect">The effect added.</param>
				///<returns>The index.</returns>
				int Add(SoundEffect^ Effect);

				///<summary>Adds an effect at the given index. Returns the index.</summary>
				///<param name="Index">The position of the inserted effect.</param>
				///<param name="Effect">The effect added.</param>
				///<returns>The index.</returns>
				int Insert(int Index, SoundEffect^ Effect);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Adds an effect at the position of another. Returns the index.</summary>
				///<param name="Effect">The effect that will give its position.</param>
				///<param name="InsertedEffect">The effect added.</param>
				///<returns>The index.</returns>
				int Insert(SoundEffect^ Effect, SoundEffect^ InsertedEffect);

				///<summary>Returns the index of an effect.</summary>
				///<param name="Effect">The effect searched.</param>
				///<returns>The index.</returns>
				int IndexOf(SoundEffect^ Effect);

				///<summary>True if the effect belongs to the chain. False elsewise.</summary>
				///<param name="Effect">The effect searched.</param>
				///<returns>A boolean indicating if Effect belongs to the chain.</returns>
				bool Contains(SoundEffect^ Effect);

				///<summary>Removes the effect at this index from the chain.</summary>
				///<param name="Index">The index of the effect searched.</param>
				void Remove(int Index);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Removes the effect from the chain.</summary>
				///<param name="Effect">The effect searched.</param>
				void Remove(SoundEffect^ Effect);

				///<summary>Replaces the effect at this index with another.</summary>
				///<param name="Index">The index of the effect which'll be replaced.</param>
				///<param name="Effect">The new effect at index.</param>
				void Replace(int Index, SoundEffect^ Effect);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Replaces the effect with another.</summary>
				///<param name="ReplacedEffect">The effect which'll be replaced.</param>
				///<param name="ReplacingEffect">The new effect at index.</param>
				void Replace(SoundEffect^ ReplacedEffect, SoundEffect^ ReplacingEffect);

				///<summary>Swaps effects.</summary>
				///<param name="FirstIndex">The index of the effect.</param>
				///<param name="SecondIndex">The index of the other effect.</param>
				void Swap(int FirstIndex, int SecondIndex);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Swaps effects.</summary>
				///<param name="FirstEffect">The effect.</param>
				///<param name="SecondEffect">The other effect.</param>
				void Swap(SoundEffect^ FirstEffect, SoundEffect^ SecondEffect);

				///<summary>Returns the effect at the given index.</summary>
				///<param name="Index">The index.</param>
				///<returns>The effect at the index.</returns>
				SoundEffect^ At(int Index);

				///<summary>Clears the chain.</summary>
				void Clear();

				///<summary>Activate an effect.</summary>
				///<param name="Index">The index.</param>
				void Activate(int Index);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Activate an effect.</summary>
				///<param name="Effect">The effect.</param>
				void Activate(SoundEffect^ Effect);

				///<summary>Deactivate an effect.</summary>
				///<param name="Index">The index.</param>
				void Deactivate(int Index);
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				///<summary>Deactivate an effect.</summary>
				///<param name="Effect">The effect.</param>
				void Deactivate(SoundEffect^ Effect);

			private:
				//Called when collection changed
				void CollectionChanged();

			internal:
				//Called when effect need update
				void EffectUpdate(SoundEffect^ EffectDescriptor);
		};
	}
}

#endif