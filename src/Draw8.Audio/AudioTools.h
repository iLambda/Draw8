#include "pch.h"

#ifndef DEF_AUDIOTOOLS
#define DEF_AUDIOTOOLS

namespace Draw8
{
	namespace Audio
	{
		//Definitions
		#define DRAW8_AUDIO_MIN_FREQ_RATIO			XAUDIO2_MIN_FREQ_RATIO
		#define DRAW8_AUDIO_MAX_FREQ_RATIO			XAUDIO2_MAX_FREQ_RATIO
		
		#define DRAW8_AUDIO_MIN_SAMPLE_RATE			XAUDIO2_MIN_SAMPLE_RATE
		#define DRAW8_AUDIO_MAX_SAMPLE_RATE			XAUDIO2_MAX_SAMPLE_RATE

		#define DRAW8_AUDIO_MAX_LOOP_COUNT			XAUDIO2_MAX_LOOP_COUNT
		#define DRAW8_AUDIO_INFINITE_LOOP_COUNT		XAUDIO2_LOOP_INFINITE
				
		#define DRAW8_AUDIO_MAX_VOLUME				XAUDIO2_MAX_VOLUME_LEVEL
		#define DRAW8_AUDIO_MIN_VOLUME				(-DRAW8_AUDIO_MAX_VOLUME)

		#define DRAW8_AUDIO_MAX_FILTER_ONEOVERQ     XAUDIO2_MAX_FILTER_ONEOVERQ
		#define DRAW8_AUDIO_MIN_FILTER_ONEOVERQ     0.f

		#define DRAW8_AUDIO_MAX_FILTER_FREQUENCY	XAUDIO2_MAX_FILTER_FREQUENCY
		#define DRAW8_AUDIO_MIN_FILTER_FREQUENCY	0.f

		//Note : if not on Xbox, these strings must be low-endian.
		//Will look at it later when my Xbox One SDK arrives (id@xbox much)
		//(iLambda, 26/02/14, 17:50, France)
		#define DRAW8_FOURCC_RIFF_TAG       'FFIR'
		#define DRAW8_FOURCC_FORMAT_TAG     ' tmf'
		#define DRAW8_FOURCC_DATA_TAG       'atad'
		#define DRAW8_FOURCC_WAVE_FILE_TAG  'EVAW'
		#define DRAW8_FOURCC_XWMA_FILE_TAG  'AMWX'
		#define DRAW8_FOURCC_DLS_SAMPLE     'pmsw'
		#define DRAW8_FOURCC_MIDI_SAMPLE    'lpms'
		#define DRAW8_FOURCC_XWMA_DPDS      'sdpd'
		#define DRAW8_FOURCC_XMA_SEEK       'kees'

		//Enums
		///<summary>Defines the number of channels of a sound.</summary>
		public enum class Channels { Mono = 1, Stereo = 2 };
		///<summary>Defines the sound data type</summary>
		public enum class SoundDataType { Byte, Short, Int, Float };
		///<summary>Defines the format of the sound.</summary>
		public enum class SoundDataFormat 
		{ 
			PCM = WAVE_FORMAT_PCM,
			IEEE_FLOAT = WAVE_FORMAT_IEEE_FLOAT
		};
		///<summary>The filter type.</summary>
		public enum class SoundFilterType 
		{
			LowPass = LowPassFilter,
			BandPass = BandPassFilter,
			HighPass = HighPassFilter,
			Notch = NotchFilter
		};

		//Structs
		//A raw parameter descriptor
		struct RawParameterDescriptor
		{
			LPCWSTR Name;
			uint Size;
		};

		//Classes
		///<summary>Represents a basic sound filter (e.g. Low pass filter, notch filter, ...).</summary>
		public ref class SoundFilter sealed
		{
			private:
				//The filter type.
				SoundFilterType m_type;
				//The Q-factor (quality factor) of the filter.
				float m_qualityFactor;
				//The cutoff frequency.
				float m_cutoffFrequency;

			public:
				///<summary>The filter type.</summary>
				property SoundFilterType Type { SoundFilterType get() { return m_type; } }
				///<summary>The Q-factor (quality factor) of the filter.</summary>
				property float QualityFactor  { float get() { return m_qualityFactor; } }
				///<summary>The cutoff frequency. Must be between 0 and (SampleRate / 6). If not, the frequency will be clamped.</summary>
				property float CutoffFrequency { float get() { return m_cutoffFrequency; } }

			public:
				///<summary>Initializes a sound filter.</summary>
				///<param name="Type">The filter type.</param>
				///<param name="QualityFactor">The Q-factor (quality factor) of the filter.</param>
				///<param name="CutoffFrequency">The cutoff frequency. Must be between 0 and (SampleRate / 6). If not, the frequency will be clamped.</param>
				SoundFilter(SoundFilterType Type, float QualityFactor, float CutoffFrequency)
				{
					//Initalizes
					m_type = Type;
					m_qualityFactor = QualityFactor;
					m_cutoffFrequency = CutoffFrequency;
				}

			internal:
				//Returns the descriptor
				XAUDIO2_FILTER_PARAMETERS GetDescriptor(float SampleRate)
				{
					//We clamp the values
					XAUDIO2_FILTER_TYPE type = (XAUDIO2_FILTER_TYPE)m_type;
					float oneOverQ = Draw8::Clamp(1.f / m_qualityFactor, DRAW8_AUDIO_MIN_FILTER_ONEOVERQ, DRAW8_AUDIO_MAX_FILTER_ONEOVERQ);
					float cutoffFrequency = 2 * sin(PI_F * (Draw8::Clamp(m_cutoffFrequency, 0.f, SampleRate / 6.f) / SampleRate));

					//We create the descriptor
					XAUDIO2_FILTER_PARAMETERS descriptor;
					descriptor.Frequency = cutoffFrequency;
					descriptor.OneOverQ = oneOverQ;
					descriptor.Type = type;

					return descriptor;
				}
		};

		///<summary>Describes a loop region.</summary>
		public ref class LoopInfo sealed
		{
			private:
				//The number of times
				uint m_times;

			public:
				///<summary>Number of times the sound must be looped. It must be between 1 and LoopInfo.InfiniteLoop. 
				///If the value is superior, the sound will loop an infinite number of times.</summary>
				property uint Times { uint get() { return m_times; } void set(uint val) { m_times = Draw8::Clamp(val, 1u, InfiniteLoop); } }
				///<summary>The loop starting time in milliseconds. If Start represents a duration superior to the sound duration, it will be clamped.
				///If Times == 1, then Start is the duration at which the sound will begin.
				///If Times > 1, then the sound will play for the first time normally, and then loop starting at the moment Start represents.</summary>
				property uint Start;

			public:
				///<summary>The number of times a sound must loop to loop infinitely.</summary>
				static property uint InfiniteLoop { uint get() { return DRAW8_AUDIO_INFINITE_LOOP_COUNT; } } 
				///<summary>The number of times a sound can loop before looping infinitely.</summary>
				static property uint MaxLoop { uint get() { return DRAW8_AUDIO_MAX_LOOP_COUNT; } } 
			
			public:
				///<summary>Creates a LoopInfo</summary>
				LoopInfo() { Times = 1; Start = 0; }
				///<summary>Creates a LoopInfo</summary>
				///<param name="Times">Number of times the sound must be looped. It must be between 1 and LoopInfo.InfiniteLoop. 
				///If the value is superior, the sound will loop an infinite number of times.</param>
				LoopInfo(uint Times) { this->Times = Times; Start = 0; }
				///<summary>Creates a LoopInfo</summary>
				///<param name="Times">Number of times the sound must be looped. It must be between 1 and LoopInfo.InfiniteLoop. 
				///If the value is superior, the sound will loop an infinite number of times.</param>
				///<param name="Start">The loop starting time in milliseconds. If Start represents a duration superior to the sound duration, it will be clamped.
				///If Times == 1, then Start is the duration at which the sound will begin.
				///If Times > 1, then the sound will play for the first time normally, and then loop starting at the moment Start represents.</param>
				LoopInfo(uint Times, uint Start) { this->Times = Times; this->Start = Start; }
		};

		//Functions
		//Convert the enum to the corresponding ID
		inline WORD GetSampleBits(SoundDataType Type)
		{
			switch (Type)
			{
				case SoundDataType::Byte:
					return 8;
				case SoundDataType::Short:
					return 16;
				case SoundDataType::Int:
				case SoundDataType::Float:
					return 32;
				default:
					return 0;
			}
		}
		
		//Convert the enum to the corresponding ID
		inline WORD GetFormat(SoundDataFormat Format)
		{
			return (uint)Format;
		}
		
		//Convert the IDs to the corresponding enums
		inline void ParseWaveHeader(WORD wFormatTag, WORD wBitsPerSample, SoundDataType* Type, SoundDataFormat* Format)
		{
			switch (wFormatTag)
			{
				case WAVE_FORMAT_IEEE_FLOAT:
					*Format = SoundDataFormat::IEEE_FLOAT;

					if(wBitsPerSample == 32)
						*Type = SoundDataType::Float;
					else
						throw ref new Platform::NotImplementedException("Wave format unsupported. (Bits per sample cannot be " + wBitsPerSample + " in " + wFormatTag + " format.)");

					break;

				case WAVE_FORMAT_PCM:
					*Format = SoundDataFormat::PCM;

					if(wBitsPerSample == 8)
						*Type = SoundDataType::Byte;
					else if(wBitsPerSample == 16)
						*Type = SoundDataType::Short;
					else if(wBitsPerSample == 32)
						*Type = SoundDataType::Int;
					else
						throw ref new Platform::NotImplementedException("Wave format unsupported. (Bits per sample cannot be " + wBitsPerSample + " in " + wFormatTag + " format.)");

					break;

				default:
					throw ref new Platform::NotImplementedException("Wave format unsupported. (Format must be PCM or IEEE Float.)");
			}
		}
	}
}

#endif