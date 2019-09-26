// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOGICALDEVICECLIENTVK_H
#define MOAILOGICALDEVICECLIENTVK_H

#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// MOAILogicalDeviceClientVK
//================================================================//
class MOAILogicalDeviceClientVK {
private:

	MOAILogicalDeviceVK*		mLogicalDevice;

public:
	
	//----------------------------------------------------------------//
	MOAILogicalDeviceVK& GetLogicalDevice () {
		assert ( this->mLogicalDevice );
		return *this->mLogicalDevice;
	}
	
	//----------------------------------------------------------------//
	MOAILogicalDeviceClientVK () :
		mLogicalDevice ( NULL ) {
	}
	
	//----------------------------------------------------------------//
	~MOAILogicalDeviceClientVK () {
	}
	
	//----------------------------------------------------------------//
	void SetLogicalDevice ( MOAILogicalDeviceVK& logicalDevice ) {
		this->mLogicalDevice = &logicalDevice;
	}
};

#endif
