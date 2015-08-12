#include "pch.h"

#ifndef DEF_TOOLS
#define DEF_TOOLS

#include <ppl.h>
#include <ppltasks.h>

namespace Draw8
{
	//Definitions
	#define PI_F 3.14159265f
	#define PI_D 3.14159265
	#define PI PI_F
	#define PHI 1.61803398874989484820458683436563811
	
	#define DRAW8_32BIT_ALLOCATION

	#define WIN_PHONE_8 8
	#define WIN_PHONE_VER_0 0

	//Macro
	#define COALESCE(a, b) (a != NULL ? a : b)
	#define COALESCE_CLI(a, b) (a != nullptr ? a : b)
	#define CLAMP(x, m, M) (x > m ? (x < M ? x : M) : m)
	#define CALLIFNOTNULL(funcPtsr) if(funcPtr != NULL) { funcPtr(); }
	#define SQUARE(x) (x*x)

	//Typedefs
	typedef unsigned int uint;		//Unsigned int
	typedef unsigned long ulong;	//Unsigned long
	typedef unsigned char uchar;	//Unsigned char
	typedef unsigned char byte;		//Unsigned char
	typedef unsigned short ushort; //Unsigned short

	//Inlines
	//Outputs something
	inline void Output(Platform::Object^ Object)
	{
		OutputDebugString(Object->ToString()->Data());
	}

	//Throws exception if disposed
	inline void ThrowIfDisposed(bool IsDisposed)
	{
		if(IsDisposed)
			throw ref new Platform::DisconnectedException("The object has been disposed.");
	}

	//Throws exception with message if true
	inline void ThrowIf(bool Condition, Platform::String^ Message, HRESULT hr = E_FAIL)
	{
		if(Condition)
			throw ref new Platform::COMException(hr, Message);
	}
	//Throws an exception corresponding to the HRESULT passed in parameter.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
			throw ref new Platform::COMException(hr);//Platform::Exception::CreateException(hr);
	}
	//Clamps the value between m and M
	template <typename T>
	inline T Clamp(T x, T m, T M)
	{
		return (x > m ? (x < M ? x : M) : m);
	}

	//Cast from type
	inline void Cast(Platform::Object^ Object, void* Pointer)
	{
		switch (Platform::Type::GetTypeCode(Object->GetType()))
		{
			case Platform::TypeCode::Boolean: (*((bool*)Pointer)) = (bool)Object; return;
			case Platform::TypeCode::Char16:  (*((wchar_t*)Pointer)) = (wchar_t)Object; return;
			case Platform::TypeCode::Double:  (*((double*)Pointer)) = (double)Object; return;
			case Platform::TypeCode::Int16:   (*((short*)Pointer)) = (short)Object; return;
			case Platform::TypeCode::Int32:   (*((int*)Pointer)) = (int)Object; return;
			case Platform::TypeCode::Int64:   (*((long long*)Pointer)) = (long long)Object; return;
			//case Platform::TypeCode::Int8:    (*((char*)Pointer)) = (char)Object; return;
			case Platform::TypeCode::Single:  (*((float*)Pointer)) = (float)Object; return;	
			case Platform::TypeCode::String:  (*((Platform::String^*)Pointer)) = (Platform::String^)Object; return;
			case Platform::TypeCode::UInt16:  (*((ushort*)Pointer)) = (ushort)Object; return;
			case Platform::TypeCode::UInt32:  (*((uint*)Pointer)) = (uint)Object; return;
			case Platform::TypeCode::UInt64:  (*((unsigned long long*)Pointer)) = (unsigned long long)Object; return;
			case Platform::TypeCode::UInt8:   (*((uchar*)Pointer)) = (uchar)Object; return;
			default: return;
		}
	}

	//Functions
	//Loads a byte array from file synchronously. Data must be an empty pointer, or memory leaks will ensue.
	inline void LoadBufferFromFile(Platform::String^ Filename, byte** Data, LARGE_INTEGER* FileSize = NULL, DWORD* BytesRead = NULL)
	{
		#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
			//Get installed location path
			auto path = (Windows::ApplicationModel::Package::Current->InstalledLocation->Path) + "\\" + Filename;
			auto basePath = path->Data();
		#else
			auto basePath = Filename->Data();
		#endif

		//Gets a handle on the file
		#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
			auto fileHandle = CreateFile2(basePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);
		#else
			auto fileHandle = CreateFileW(basePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		#endif

		//Tests if error
		if(fileHandle == INVALID_HANDLE_VALUE)
			Draw8::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));

		//Gets file size to allocate memory
		LARGE_INTEGER fileSize;
		#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
			FILE_STANDARD_INFO fileInfo;
			if (!GetFileInformationByHandleEx(fileHandle, FileStandardInfo, &fileInfo, sizeof(fileInfo)))
				Draw8::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
			fileSize = fileInfo.EndOfFile;
		#else
			GetFileSizeEx(fileHandle, &fileSize);
		#endif

		//If 32-bit allocation
		#ifdef DRAW8_32BIT_ALLOCATION
			Draw8::ThrowIf(fileSize.HighPart > 0, "The file is too big for 32-bit allocation", E_FAIL);
		#endif
			
		//Allocate memory
		*Data = new byte[fileSize.LowPart];
		DWORD bytesRead = 0;
		
		//Reads bytes
		if(!ReadFile(fileHandle, *Data, fileSize.LowPart, &bytesRead, nullptr))
		{
			delete[] *Data;
			Data = NULL;
			Draw8::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		//Tests if file loading is ok
		if((bytesRead < fileSize.LowPart))
		{
			delete[] *Data;
			Data = NULL;
			Draw8::ThrowIf(true, "An error occured during file reading.", E_FAIL);
		}

		//Returns stuff
		if(FileSize != NULL)
			*FileSize = fileSize;
		if(BytesRead != NULL)
			*BytesRead = bytesRead;

		//Releases the handle
		CloseHandle(fileHandle);
	}	

	//Loads a byte array asynchronously from file
	inline Concurrency::task<Platform::Array<byte>^> LoadBufferFromFileAsync(Platform::String^ filename)
	{
		//Some usings
		using namespace Windows::Storage;
		using namespace Concurrency;
		
		//Opens the local folder
		auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
		
		//Create a task to return file
		return create_task(folder->GetFileAsync(filename)).then([] (StorageFile^ file) 
		{
			//Opends the file
			return file->OpenReadAsync();
		}).then([] (Streams::IRandomAccessStreamWithContentType^ stream)
		{
			//Read the stream
			unsigned int bufferSize = static_cast<unsigned int>(stream->Size);
			auto fileBuffer = ref new Streams::Buffer(bufferSize);
			return stream->ReadAsync(fileBuffer, bufferSize, Streams::InputStreamOptions::None);
		}).then([] (Streams::IBuffer^ fileBuffer) -> Platform::Array<byte>^ 
		{
			//Returns data<
			auto fileData = ref new Platform::Array<byte>(fileBuffer->Length);
			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(fileData);
			return fileData;
		});
	}
}

#endif