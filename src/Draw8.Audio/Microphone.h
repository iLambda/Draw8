#include "pch.h"

#ifndef DEF_MICROPHONE
#define DEF_MICROPHONE

#include "SoundData.h"
#include "AudioTools.h"
#include <wrl\implements.h>

namespace Draw8
{
	namespace Audio
	{
		ref class Microphone;

		///<summary>The delegate holding the function called when the microphone is activated.</summary>
		///<param name="Sender">The sender of the event.</param>
		///<param name="Args">The arguments. Always null.</param>
		public delegate void MicrophoneActivatedEventHandler(Microphone^ Sender, Platform::Object^ Args);

		///<summary>An input peripheral that captures audio.</summary>
		public ref class Microphone sealed
		{

#ifndef WIN_PHONE
			//Friend the MicrophoneActivateAudioInterfaceCompletionHandler class
			friend class MicrophoneActivateAudioInterfaceCompletionHandler;
#endif

			private:
				//The default capture device
				IAudioClient2* m_defaultCaptureDevice;
				//The capture client
				IAudioCaptureClient* m_captureClient;
				//The output format
				WAVEFORMATEX m_waveFormatEx;
				//Is microphone started
				bool m_isStarted;
				//The format
				SoundDataFormat m_format;
				//The data type
				SoundDataType m_type;
				//The sample rate
				int m_sampleRate;
				//The channels number
				Channels m_channels;
				//Contains the input device
				IDevice^ m_inputDevice;
				//The activated indicator
				bool m_activated;
				//The max duration
				Windows::Foundation::TimeSpan m_maxDuration;
#ifndef WIN_PHONE
				//The completion handler. Used only when activation is async.
				Microsoft::WRL::ComPtr<MicrophoneActivateAudioInterfaceCompletionHandler> m_completeHandler;
#endif
				
			public:
				///<summary>True if microphone is recording. False elsewise.</summary>
				property bool IsStarted { bool get() { return m_isStarted; } }
				///<summary>The format of the data returned by this microphone.</summary>
				property SoundDataFormat Format { SoundDataFormat get() { return m_format; } }
				///<summary>The type of the data returned by this microphone.</summary>
				property SoundDataType Type { SoundDataType get() { return m_type; } }
				///<summary>The sample rate of the data returned by this microphone.</summary>
				property int SampleRate { int get() { return m_sampleRate; } }
				///<summary>The channels of the data returned by this microphone.</summary>
				property Draw8::Audio::Channels Channels { Draw8::Audio::Channels get() { return m_channels; } }
				///<summary>The audio input device. Null is default device (usually default mic).</summary>
				property IDevice^ InputDevice { IDevice^ get() { return m_inputDevice; } }
				///<summary>True if the microphone is correctly activated. False elsewise.</summary>
				property bool IsActivated { bool get() { return m_activated; } }
				///<summary>The recoring maximum duration.</summary>
				property Windows::Foundation::TimeSpan MaxDuration { Windows::Foundation::TimeSpan get() { return m_maxDuration; } }

			public:
				///<summary>Initializes the microphone. Default parameters is mono 32-bit float data, with a sample rate of 22050Hz. The input device is the default mic. Do not forget to add the Microphone capability.</summary>
				///<param name="MaxDuration">The recoring maximum duration.</param>
				Microphone(Windows::Foundation::TimeSpan MaxDuration);
				///<summary>Initializes the microphone. Default parameters is mono 32-bit float data, with a sample rate of 22050Hz. Do not forget to add the Microphone capability.</summary>
				///<param name="InputDevice">The audio input device. Use null for default device (usually default microphone).</param>
				///<param name="MaxDuration">The recoring maximum duration.</param>
				Microphone(Windows::Foundation::TimeSpan MaxDuration, IDevice^ InputDevice);
				///<summary>Initializes the microphone. Do not forget to add the Microphone capability.</summary>
				///<param name="Type">The type of the data returned by this microphone.</param>
				///<param name="Format">The format of the data returned by this microphone.</param>
				///<param name="SampleRate">The sample rate of the data returned by this microphone.</param>
				///<param name="Channels">The channels of the data returned by this microphone.</param>
				///<param name="InputDevice">The audio input device. Use null for default device (usually default microphone).</param>
				///<param name="MaxDuration">The recoring maximum duration.</param>
				Microphone(Windows::Foundation::TimeSpan MaxDuration, SoundDataType Type, SoundDataFormat Format, int SampleRate, Draw8::Audio::Channels Channels, IDevice^ InputDevice);

			public:
				///<summary>Triggered when the microphone is activated. Out of the UI thread.</summary>
				event MicrophoneActivatedEventHandler^ Activated;

			private:
				//Creates the mic
				void Create(Windows::Foundation::TimeSpan MaxDuration, SoundDataType Type, SoundDataFormat Format, int SampleRate, Draw8::Audio::Channels Channels, IDevice^ InputDevice);
				//Releases all buffers
				void ClearAllBuffers();

			internal:
#ifdef WIN_PHONE
				//Finish activation
				static void FinishActivation(Microphone^ Microphone);
#else
				//Finish activation
				static void FinishActivation(Microphone^ Microphone, IActivateAudioInterfaceAsyncOperation* ActivateOperation);
#endif


			public:
				///<summary>Captures data from the microphone.</summary>
				SoundData^ ReadData();
				///<summary>Start capturing data from the microphone.</summary>
				void StartCapture();
				///<summary>Stop capturing data from the microphone.</summary>
				void StopCapture();
		};

#ifndef WIN_PHONE
		//The async operation
		class MicrophoneActivateAudioInterfaceCompletionHandler : public Microsoft::WRL::RuntimeClass< Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::ClassicCom >, Microsoft::WRL::FtmBase, IActivateAudioInterfaceCompletionHandler >
		{
			private:
				//The owner
				Microphone^ m_microphone;
				
			public:
				//Ctor
				MicrophoneActivateAudioInterfaceCompletionHandler(Microphone^ Microphone) { m_microphone = Microphone; }

			public:
				//The overload
				virtual HRESULT STDMETHODCALLTYPE ActivateCompleted(IActivateAudioInterfaceAsyncOperation *ActivateOperation)
				{
					//Finish
					Microphone::FinishActivation(m_microphone, ActivateOperation);

					//Does not failed
					return S_OK;
				}
		};
#endif

	}
}

#endif