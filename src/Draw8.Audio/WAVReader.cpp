#include "pch.h"
#include "AudioTools.h"
#include "WAVReader.h"

void Draw8::Audio::WAVReader::ReadFromData(byte* RawData, uint RawDataSize, WAVEFORMATEX** WAVEFormat, byte** WAVEData, uint* WAVEDataSize)
{
	//Check input
	Draw8::ThrowIf(!RawData, "The WAVE raw data given is null.", E_INVALIDARG);

	//Check if the data is at least the size of a minimal wav
	Draw8::ThrowIf((RawDataSize < (sizeof(RIFFChunk)*2 + sizeof(DWORD) + sizeof(WAVEFORMAT))), "The data is to small to contain a correct WAV file", E_INVALIDARG);
	
	//Computes the end of data
	const byte* wavEnd = RawData + RawDataSize;
	
	//Locate a chunk
	auto riffChunk = FindChunk(RawData, RawDataSize, DRAW8_FOURCC_RIFF_TAG);
    Draw8::ThrowIf(!riffChunk || riffChunk->size < 4, "The chunk found in the wav data is not valid.", E_FAIL);

	//Conert it to a header
	auto riffHeader = reinterpret_cast<const RIFFChunkHeader*>(riffChunk);
    Draw8::ThrowIf(riffHeader->riff != DRAW8_FOURCC_WAVE_FILE_TAG && riffHeader->riff != DRAW8_FOURCC_XWMA_FILE_TAG, "The chunk found couldn't be converted to a valid header.", E_FAIL);

	//We search for 'fmt'
	auto ptr = reinterpret_cast<const byte*>(riffHeader) + sizeof(RIFFChunkHeader);
	Draw8::ThrowIf(((ptr + sizeof(RIFFChunk)) > wavEnd), "End of file was reached before the routine was able to find suitable data.", HRESULT_FROM_WIN32(ERROR_HANDLE_EOF));
    
	//Find the fmt chunk
    auto fmtChunk = FindChunk(ptr, riffHeader->size, DRAW8_FOURCC_FORMAT_TAG);
    Draw8::ThrowIf(!fmtChunk || fmtChunk->size < sizeof(PCMWAVEFORMAT), "The FMT chunk was not found. Data might be corrputed.", E_FAIL);

	//Convert the chunk to byte data
    ptr = reinterpret_cast<const byte*>(fmtChunk) + sizeof(RIFFChunk);
    Draw8::ThrowIf(ptr + fmtChunk->size > wavEnd, "End of file was reached before the routine was able to find suitable data.", HRESULT_FROM_WIN32(ERROR_HANDLE_EOF));
	
	//Finally get wave format
    auto wf = reinterpret_cast<const WAVEFORMAT*>(ptr);

	//Validate WAVEFORMAT pointer
    switch( wf->wFormatTag )
    {
		//Those two formats doesn't need validation
		case WAVE_FORMAT_PCM:
		case WAVE_FORMAT_IEEE_FLOAT:
			break;

		default:
        {
			//Check
			Draw8::ThrowIf(fmtChunk->size < sizeof(WAVEFORMATEX), "The chunk is invalid", E_FAIL);
            
			//Casting the current data (ptr) into a WAVEFORMATEX header
            auto wfx = reinterpret_cast<const WAVEFORMATEX*>(ptr);

			//Check
			Draw8::ThrowIf(fmtChunk->size < (sizeof(WAVEFORMATEX) + wfx->cbSize), "The chunk is invalid", E_FAIL);
            
			//Now we treat the format here
			if(wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
			{
				//Check
				Draw8::ThrowIf(( fmtChunk->size < sizeof(WAVEFORMATEXTENSIBLE) ) || ( wfx->cbSize < ( sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) ) ), "The chunk is invalid", E_FAIL);
            
				//The GUID used for memory comparaison
				static const GUID wfexGUID = {0x00000000, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};
				
				//The wave format extensible pointer
                auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>( ptr );

				//Check
				Draw8::ThrowIf(memcmp(reinterpret_cast<const byte*>(&wfex->SubFormat) + sizeof(DWORD), reinterpret_cast<const byte*>(&wfexGUID) + sizeof(DWORD), sizeof(GUID) - sizeof(DWORD) ) != 0, 
							   "An error parsing the wave file occured. The type of file is not supported.", 
							   E_NOTIMPL);

				//Checking the subformat
                switch( wfex->SubFormat.Data1 )
                {
					//Those formats are supported
					case WAVE_FORMAT_PCM:
					case WAVE_FORMAT_IEEE_FLOAT:
						//Good to go
						break;

					default:
						throw ref new Platform::NotImplementedException("This wave format type is not supported now");
                }
			}
			else
				throw ref new Platform::NotImplementedException("This wave format type is not supported now");
        }
    }

    //Locate 'data' tag
    ptr = reinterpret_cast<const byte*>(riffHeader) + sizeof(RIFFChunkHeader);
	Draw8::ThrowIf((ptr + sizeof(RIFFChunk)) > wavEnd, "End of file was reached.", HRESULT_FROM_WIN32(ERROR_HANDLE_EOF));
    
	//We find the chunk
    auto dataChunk = FindChunk(ptr, riffChunk->size, DRAW8_FOURCC_DATA_TAG);
	Draw8::ThrowIf(!dataChunk || !dataChunk->size, "WAV data is invalid.", HRESULT_FROM_WIN32(ERROR_INVALID_DATA));
    
	//We cast it into data
    ptr = reinterpret_cast<const byte*>(dataChunk) + sizeof(RIFFChunk);
	Draw8::ThrowIf(ptr + dataChunk->size > wavEnd, "End of file was reached.", HRESULT_FROM_WIN32(ERROR_HANDLE_EOF));

    //Returns data
    *WAVEFormat = const_cast<WAVEFORMATEX*>(reinterpret_cast<const WAVEFORMATEX*>(wf));
	*WAVEData = const_cast<byte*>(ptr);
    *WAVEDataSize = (uint)dataChunk->size;
}

const Draw8::Audio::RIFFChunk* Draw8::Audio::WAVReader::FindChunk(const byte* Data, size_t SizeBytes, uint Tag)
{
	//Check input
    if (!Data)
        return nullptr;

	//Compute some useful values (end and current)
    const byte* ptr = Data;
    const byte* end = Data + SizeBytes;

	//Check into data for the good chunk
    while (end > (ptr + sizeof(RIFFChunk)))
    {
		//If we found it, we return the chunk
        auto header = reinterpret_cast<const RIFFChunk*>(ptr);
        if (header->tag == Tag)
            return header;

		//Else we continue searching
		ptrdiff_t offset = header->size + sizeof(RIFFChunk);
        ptr += offset;
    }

	//Nothing found
    return nullptr;
}