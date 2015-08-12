#include "pch.h"

#ifndef DEF_IDEVICE
#define DEF_IDEVICE

namespace Draw8
{
	///<summary>A device.</summary>
	public interface class IDevice
	{
		public:
			///<summary>The audio endpoint device identifier.</summary>
			property Platform::String^ DeviceID { Platform::String^ get(); }
			///<summary>The display name.</summary>
			property Platform::String^ DisplayName { Platform::String^ get(); }
	};
}

#endif