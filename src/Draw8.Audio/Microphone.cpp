#include "pch.h"
#include "Microphone.h"

Draw8::Audio::Microphone::Microphone(Windows::Foundation::TimeSpan MaxDuration)
{
	Create(MaxDuration, SoundDataType::Float, SoundDataFormat::IEEE_FLOAT, 22050, Audio::Channels::Mono, nullptr);
}

Draw8::Audio::Microphone::Microphone(Windows::Foundation::TimeSpan MaxDuration, IDevice^ InputDevice)
{
	Create(MaxDuration, SoundDataType::Float, SoundDataFormat::IEEE_FLOAT, 22050, Audio::Channels::Mono, InputDevice);
}

Draw8::Audio::Microphone::Microphone(Windows::Foundation::TimeSpan MaxDuration, SoundDataType Type, SoundDataFormat Format, int SampleRate, Draw8::Audio::Channels Channels, IDevice^ InputDevice)
{
	Create(MaxDuration, Type, Format, SampleRate, Channels, InputDevice);
}

void Draw8::Audio::Microphone::Create(Windows::Foundation::TimeSpan MaxDuration, SoundDataType Type, SoundDataFormat Format, int SampleRate, Draw8::Audio::Channels Channels, IDevice^ InputDevice)
{
	//Assert if bad params
	Draw8::ThrowIf(SampleRate <= 0, "The sample rate must be greater than 0.", E_INVALIDARG);
	Draw8::ThrowIf(Type == SoundDataType::Float && Format == SoundDataFormat::PCM, "The type and the format doesn't match.", E_INVALIDARG);

	//Set data
	m_type = Type;
	m_format = Format;
	m_isStarted = false;
	m_sampleRate = SampleRate;
	m_channels = Channels;
	m_inputDevice = InputDevice;
	m_activated = false;
	m_maxDuration = MaxDuration;

	//We create the capture device
	LPCWSTR CaptureDeviceID = NULL;

	#ifdef WIN_PHONE
		//We get the capture device name
		CaptureDeviceID = m_inputDevice == nullptr ? GetDefaultAudioCaptureId(AudioDeviceRole::Default) : m_inputDevice->DeviceID->Data();

		//We create the interface
		Draw8::ThrowIfFailed(ActivateAudioInterface(CaptureDeviceID, __uuidof(IAudioClient2), (void**)&m_defaultCaptureDevice));

		//Finish activation
		FinishActivation(this);
	#else
		//We get the capture device name
		CaptureDeviceID = m_inputDevice == nullptr ? Windows::Media::Devices::MediaDevice::GetDefaultAudioCaptureId(Windows::Media::Devices::AudioDeviceRole::Default)->Data() : m_inputDevice->DeviceID->Data();
		
		//We create the interface
		IActivateAudioInterfaceAsyncOperation * asyncOp;
		m_completeHandler = Microsoft::WRL::Make<MicrophoneActivateAudioInterfaceCompletionHandler>(this);
		Draw8::ThrowIfFailed(ActivateAudioInterfaceAsync(CaptureDeviceID, __uuidof(IAudioClient2), NULL, m_completeHandler.Get(), &asyncOp));
	#endif
}

#ifdef WIN_PHONE
void Draw8::Audio::Microphone::FinishActivation(Microphone^ Microphone)
#else
void Draw8::Audio::Microphone::FinishActivation(Microphone^ Microphone, IActivateAudioInterfaceAsyncOperation* ActivateOperation)
#endif
{

#ifndef WIN_PHONE
	//Get the activation result
	HRESULT hr;
	IUnknown* unknown;
	Draw8::ThrowIfFailed(ActivateOperation->GetActivateResult(&hr, &unknown));
										
	//Convert it
	Draw8::ThrowIfFailed(unknown->QueryInterface(IID_PPV_ARGS(&Microphone->m_defaultCaptureDevice)));
#endif

	//Set the category
	AudioClientProperties properties = {};
	properties.cbSize = sizeof(AudioClientProperties);
	properties.eCategory = _AUDIO_STREAM_CATEGORY::AudioCategory_Other;

	//And sends the properties
	Draw8::ThrowIfFailed(Microphone->m_defaultCaptureDevice->SetClientProperties(&properties));
	
	//Filling structure
	Microphone->m_waveFormatEx.wFormatTag = Draw8::Audio::GetFormat(Microphone->m_format);
	Microphone->m_waveFormatEx.nChannels = (int)Microphone->m_channels;
	Microphone->m_waveFormatEx.nSamplesPerSec = Microphone->m_sampleRate;
	Microphone->m_waveFormatEx.wBitsPerSample = Draw8::Audio::GetSampleBits(Microphone->m_type);
	Microphone->m_waveFormatEx.nBlockAlign = Microphone->m_waveFormatEx.nChannels * (Microphone->m_waveFormatEx.wBitsPerSample / 8);
	Microphone->m_waveFormatEx.nAvgBytesPerSec = Microphone->m_waveFormatEx.nSamplesPerSec * Microphone->m_waveFormatEx.nBlockAlign;
	Microphone->m_waveFormatEx.cbSize = 0;

	//Initialize engine
	Draw8::ThrowIfFailed(Microphone->m_defaultCaptureDevice->Initialize(AUDCLNT_SHAREMODE_SHARED, 0x88000000, Microphone->MaxDuration.Duration, 0, &Microphone->m_waveFormatEx, NULL));

	//Get the capture client
	Draw8::ThrowIfFailed(Microphone->m_defaultCaptureDevice->GetService(__uuidof(IAudioCaptureClient), (void**)&Microphone->m_captureClient));

	//Says it's activated
	Microphone->m_activated = true;
	Microphone->Activated(Microphone, nullptr);
}

void Draw8::Audio::Microphone::StartCapture()
{
	//If started, we do nothing
	if(IsStarted)
		return;

	//Start capturing
	Draw8::ThrowIfFailed(m_defaultCaptureDevice->Start());

	//Set as capturing
	m_isStarted = true;
}

void Draw8::Audio::Microphone::StopCapture()
{
	//If started, we do nothing
	if(!IsStarted)
		return;

	//Start capturing
	Draw8::ThrowIfFailed(m_defaultCaptureDevice->Stop());
	
	//Set as capturing
	m_isStarted = false;
}

Draw8::Audio::SoundData^ Draw8::Audio::Microphone::ReadData()
{
	//If the microphone is not started, this does nothing
	if(IsStarted)
		return nullptr;

	//We initialize data
	std::vector<byte> data;
	uint packetSize = 0;
	int index = 0;

	//We gather the packet size
	Draw8::ThrowIfFailed(m_captureClient->GetNextPacketSize(&packetSize));

	//While the packet size is not zero
	while (packetSize > 0)
	{
		//Create some temp vars
		BYTE* packetData = NULL;
		UINT32 frameCount = 0;
		DWORD flags = 0;

		//Resize the vector		
		Draw8::ThrowIfFailed(m_captureClient->GetBuffer(&packetData, &frameCount, &flags, nullptr, nullptr));
		uint incomingBufferSize = frameCount * ((m_waveFormatEx.wBitsPerSample / 8) * m_waveFormatEx.nChannels);

		//Data insertion
		data.insert(data.end(), packetData, packetData + incomingBufferSize);
		
		//Releaase buffers
		Draw8::ThrowIfFailed(m_captureClient->ReleaseBuffer(frameCount));
		Draw8::ThrowIfFailed(m_captureClient->GetNextPacketSize(&packetSize));
	}
	
	//Creates the buffer
	byte* buffer = new byte[data.size()];
	stdext::checked_array_iterator<byte*> it(buffer, data.size());
	std::copy(data.begin(), data.end(), it);

	//Returns data
	return ref new SoundData(buffer, data.size()* 8 / m_waveFormatEx.wBitsPerSample, m_type, m_format);
}

void Draw8::Audio::Microphone::ClearAllBuffers()
{
	//If the microphone is not started, this does nothing
	if(IsStarted)
		return;

	//We initialize data
	uint packetSize = 0;

	//We gather the packet size
	Draw8::ThrowIfFailed(m_captureClient->GetNextPacketSize(&packetSize));

	//While the packet size is not zero
	while (packetSize > 0)
	{
		BYTE* packetData = NULL;
		UINT32 frameCount = 0;
		DWORD flags = 0;

		Draw8::ThrowIfFailed(m_captureClient->GetBuffer(&packetData, &frameCount, &flags, nullptr, nullptr));
		Draw8::ThrowIfFailed(m_captureClient->ReleaseBuffer(frameCount));
		Draw8::ThrowIfFailed(m_captureClient->GetNextPacketSize(&packetSize));
	}
}