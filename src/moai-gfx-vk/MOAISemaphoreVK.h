// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISEMAPHOREVK_H
#define MOAISEMAPHOREVK_H

#include <moai-gfx-vk/MOAILogicalDeviceClientVK.h>

//================================================================//
// MOAISemaphoreVK
//================================================================//
class MOAISemaphoreVK :
	public MOAILogicalDeviceClientVK {
private:
	
	//----------------------------------------------------------------//
	void			MOAIAbstractLifecycleClientVK_Finalize		();
	
public:

	VkSemaphore		mSemaphore;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mSemaphore != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkSemaphore () {
	
		return this->mSemaphore;
	}
	
	//----------------------------------------------------------------//
	void				Initialize					( MOAILogicalDeviceVK& logicalDevice );
						MOAISemaphoreVK				();
						~MOAISemaphoreVK			();
};

#endif
