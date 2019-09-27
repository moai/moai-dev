// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOGICALDEVICEVKCLIENT_H
#define MOAILOGICALDEVICEVKCLIENT_H

#include <moai-gfx-vk/MOAILifecycleProviderVK.h>

class MOAILogicalDeviceVK;

//================================================================//
// MOAILogicalDeviceClientVK
//================================================================//
class MOAILogicalDeviceClientVK :
	public MOAIAbstractLifecycleClientVK < MOAILogicalDeviceVK > {
public:

	//----------------------------------------------------------------//
	MOAILogicalDeviceVK& GetLogicalDevice () {
	
		MOAILogicalDeviceVK* logicalDevice = this->GetLifecycleProvider ();
		assert ( logicalDevice );
		return *logicalDevice;
	}
};

#endif
