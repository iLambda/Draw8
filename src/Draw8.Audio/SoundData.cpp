#include "pch.h"
#include "SoundData.h"
#include "AudioTools.h"
#include "WAVReader.h"

Draw8::Audio::SoundData::SoundData(const Platform::Array<float>^ Data)
{
	//We finally set the data
	SetRawData(Data);
	m_type = SoundDataType::Float;
	m_format = SoundDataFormat::IEEE_FLOAT;
	m_disposed = false;
}

Draw8::Audio::SoundData::SoundData(const Platform::Array<byte>^ Data)
{
	//We finally set the data
	SetRawData(Data);
	m_type = SoundDataType::Byte;
	m_format = SoundDataFormat::PCM;
	m_disposed = false;
}

Draw8::Audio::SoundData::SoundData(const Platform::Array<ushort>^ Data)
{
	//We finally set the data
	SetRawData(Data);
	m_type = SoundDataType::Short;
	m_format = SoundDataFormat::PCM;
	m_disposed = false;
}

Draw8::Audio::SoundData::SoundData(const Platform::Array<int>^ Data)
{
	//We finally set the data
	SetRawData(Data);
	m_type = SoundDataType::Int; 
	m_format = SoundDataFormat::PCM;
	m_disposed = false;
}

Draw8::Audio::SoundData::SoundData(void* Data, int Length, SoundDataType Type, SoundDataFormat Format)
{
	//We finally set the data
	m_data = Data;
	m_size = Length;
	m_type = Type;
	m_format = Format;
	m_disposed = false;
}

Draw8::Audio::SoundData::~SoundData()
{
	//If not disposed
	if(m_disposed)
		return;

	//Removing the voice
	delete m_data;		
	m_data = NULL;
	m_disposed = true;
	m_size = 0;
}

template <typename T>
void Draw8::Audio::SoundData::SetRawData(const Platform::Array<T>^ Data)
{
	//We get data length
	m_size = (int)Data->Length;
	
	//We just allocate memory for it
	m_data = new T[m_size];
	
	//We copy the data
	for(int i = 0 ; i != m_size ; i++)
		((T*)m_data)[i] = Data[i];

	//We set the data format accurately (float by default)
	m_type = SoundDataType::Float;
}


Draw8::Audio::SoundData^ Draw8::Audio::SoundData::GenerateSineWave(float Frequency, int SampleRate)
{
	//We generate the data array
	auto data = new float[SampleRate];

	//We fill it
	for(int i = 0 ; i != SampleRate ; i++)
		data[i] = sin(i * 2 * PI * Frequency / SampleRate);

	//And we return the data
	return ref new SoundData((void*)data, SampleRate, SoundDataType::Float, SoundDataFormat::IEEE_FLOAT);
}

Draw8::Audio::SoundData^ Draw8::Audio::SoundData::GenerateSquareWave(float Frequency, int SampleRate)
{
	//We generate the data array
	auto data = new float[SampleRate];

	//We fill it
	for(int i = 0 ; i != SampleRate ; i++)
		data[i] = ((float)((int)floor(i * Frequency / SampleRate) % 2) * 2) - 1;

	//And we return the data
	return ref new SoundData((void*)data, SampleRate, SoundDataType::Float, SoundDataFormat::IEEE_FLOAT);

}

Draw8::Audio::SoundData^ Draw8::Audio::SoundData::GenerateWhiteNoise(float Duration, float Level, int SampleRate)
{
	//We generate the data array
	auto bufferSize = (int)(SampleRate * Duration);
	auto data = new float[bufferSize];

	//We fill it
	int x1 = 866292096 + (rand() % 866292096);
	int x2 = 2011616708 + ((rand() + 1588) % 2011616708);
	auto level = Draw8::Clamp(Level, 0.f, 1.f) * (float)(2.0f / 0xffffffff);

	for(int i = 0 ; i != bufferSize ; i++)
	{
		x1 ^= x2;
		data[i] = x2 * level;
		x2 += x1;
	}

	//And we return the data
	return ref new SoundData((void*)data, (int)(SampleRate * Duration), SoundDataType::Float, SoundDataFormat::IEEE_FLOAT);
}

Draw8::Audio::SoundData^ Draw8::Audio::SoundData::LoadFromFile(Platform::String^ Filename, int* SampleRate, Draw8::Audio::Channels *Channels)
{
	//Loads the data from file
	byte* rawData;
	DWORD rawDataSize;
	Draw8::LoadBufferFromFile(Filename, &rawData, NULL, &rawDataSize);
	
	//Decodes it
	WAVEFORMATEX* wFormat;
	byte* data;
	uint dataSize;
	WAVReader::ReadFromData(rawData, rawDataSize, &wFormat, &data, &dataSize);

	//Gets the format
 	WAVEFORMATEX wfx = wFormat[0];
	*SampleRate = (int)wfx.nSamplesPerSec;

	//And we return the data
	auto sd = ref new SoundData(data, dataSize * 8 / wfx.wBitsPerSample, SoundDataType::Byte, SoundDataFormat::PCM);
	
	//Parse type
	Draw8::Audio::ParseWaveHeader(wfx.wFormatTag, wfx.wBitsPerSample, &sd->m_type, &sd->m_format);

	//Parse channels
	if(wfx.nChannels == 1 || wfx.nChannels == 2)
		*Channels = (Draw8::Audio::Channels)wfx.nChannels;
	else
		throw ref new Platform::NotImplementedException("Wave type not supported (Stereo and Mono supported only, cannot be " + wfx.nChannels + " channels).");

	//Return the data
	return sd;
}

Draw8::Audio::SoundData^ Draw8::Audio::SoundData::GenerateSilence(float Duration, int SampleRate)
{
	//We generate the data array
	auto data = new float[(int)(SampleRate * Duration)];

	//We fill it
	for(int i = 0 ; i != (int)(SampleRate * Duration) ; i++)
		data[i] = 0.f;

	//And we return the data
	return ref new SoundData((void*)data, (int)(SampleRate * Duration), SoundDataType::Float, SoundDataFormat::IEEE_FLOAT);
}