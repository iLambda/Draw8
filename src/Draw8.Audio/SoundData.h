#include "pch.h"

#ifndef DEF_SOUNDDATA
#define DEF_SOUNDDATA

#include "AudioTools.h"

namespace Draw8
{
	namespace Audio
	{
		///<summary>The data composing a sound.</summary>
		public ref class SoundData sealed 
		{		
			private:
				//Contains the raw data
				void* m_data;
				//Contains the data size
				int m_size;
				//Defines the buffer type
				SoundDataType m_type;
				//Defines sound format
				SoundDataFormat m_format;
				//Disposed
				bool m_disposed;

			public:
				///<summary>The data type.</summary>
				property SoundDataType Type { SoundDataType get() { return m_type; } }
				///<summary>The data format.</summary>
				property SoundDataFormat Format { SoundDataFormat get() { return m_format; } }

			public:
				///<summary>Creates the sound data in its entierty.</summary>
				///<param name="Data">Contains the raw data.</param>
				SoundData(const Platform::Array<byte>^ Data);
				///<summary>Creates the sound data in its entierty.</summary>
				///<param name="Data">Contains the raw data.</param>
				[Windows::Foundation::Metadata::DefaultOverloadAttribute]
				SoundData(const Platform::Array<ushort>^ Data);
				///<summary>Creates the sound data in its entierty.</summary>
				///<param name="Data">Contains the raw data.</param>
				SoundData(const Platform::Array<int>^ Data);
				///<summary>Creates the sound data in its entierty.</summary>
				///<param name="Data">Contains the raw data.</param>
				SoundData(const Platform::Array<float>^ Data);
				
			internal:
				//Creates a sound data in its entierty
				SoundData(void* Data, int Length, SoundDataType Type, SoundDataFormat Format);

			public:
				///<summary>Disposes of data.</summary>
				virtual ~SoundData();

			public:
				///<summary>Loads sound data from a file in float data.</summary>
				///<param name="Filename">The file path.</param>
				///<param name="SampleRate">The sampling rate (in Hertz) defined in the file.</param>
				///<param name="Channels">The channels as defined in the file.</param>
				///<returns>The sound data.</returns>
				static SoundData^ LoadFromFile(Platform::String^ Filename, int* SampleRate, Draw8::Audio::Channels *Channels);

				///<summary>Generate a sine wave data in float data.</summary>
				///<param name="Frequency">The wave frequency (in Hertz).</param>
				///<param name="SampleRate">The sampling rate (in Hertz). Generally, 44100 Hz is fine.</param>
				///<returns>The sound data.</returns>
				static SoundData^ GenerateSineWave(float Frequency, int SampleRate);

				///<summary>Creates a square wave data in float data.</summary>
				///<param name="Frequency">The wave frequency (in Hertz).</param>
				///<param name="SampleRate">The sampling rate (in Hertz). Generally, 44100 Hz is fine.</param>
				///<returns>The sound data.</returns>
				static SoundData^ GenerateSquareWave(float Frequency, int SampleRate);

				///<summary>Generate white noise data in float data.</summary>
				///<param name="Duration">The noise duration in seconds.</param>
				///<param name="Level">The noise level. Must be between 0 and 1.</param>
				///<param name="SampleRate">The sampling rate (in Hertz). Generally, 44100 Hz is fine.</param>
				///<returns>The sound data.</returns>
				static SoundData^ GenerateWhiteNoise(float Duration, float Level, int SampleRate);

				///<summary>Creates a silent buffer.</summary>
				///<param name="Duration">The noise duration in seconds.</param>
				///<param name="SampleRate">The sampling rate (in Hertz). Generally, 44100 Hz is fine.</param>
				///<returns>The sound data.</returns>
				static SoundData^ GenerateSilence(float Duration, int SampleRate);

			private:
				//Sets the data
				template <typename T>
				void SetRawData(const Platform::Array<T>^ Data);
				//Is disposed
				bool IsDisposed() { return m_disposed; }

			internal:
				//Returns the data
				void* GetData() { Draw8::ThrowIfDisposed(m_disposed); return m_data; }
				//Returns the size
				int GetDataSize() { Draw8::ThrowIfDisposed(m_disposed); return m_size; }

		};
	}
}

#endif