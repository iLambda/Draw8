#include "pch.h"
#include <stdint.h>

#ifndef DEF_WAVREADER
#define DEF_WAVREADER

namespace Draw8
{
	namespace Audio
	{
		#pragma region Definitions
		#pragma pack(push,1)
		struct RIFFChunk
		{
			uint32_t tag;
			uint32_t size;
		};

		struct RIFFChunkHeader
		{
			uint32_t tag;
			uint32_t size;
			uint32_t riff;
		};

		#pragma pack(pop)
		#pragma endregion

		class WAVReader
		{
			public:
				static void ReadFromData(byte* RawData, uint RawDataSize, WAVEFORMATEX** WAVEFormat, byte** WAVEData, uint* WAVEDataSize);

			private:
				static const RIFFChunk* FindChunk(const byte* Data, size_t SizeBytes, uint Tag);
				
		};
	}
}

#endif