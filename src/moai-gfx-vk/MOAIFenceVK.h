// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFENCEVK_H
#define MOAIFENCEVK_H

#include <moai-gfx-vk/MOAILogicalDeviceClientVK.h>

//================================================================//
// MOAIFenceVK
//================================================================//
class MOAIFenceVK :
	public MOAILogicalDeviceClientVK {
private:
	
	//----------------------------------------------------------------//
	void			MOAIAbstractLifecycleClientVK_Finalize		();
	
public:

	VkFence			mFence;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mFence != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkFence () {
	
		return this->mFence;
	}
	
	//----------------------------------------------------------------//
	void				Initialize					( MOAILogicalDeviceVK& logicalDevice, bool signaled = false );
						MOAIFenceVK 				();
						~MOAIFenceVK				();
	void				Wait						();
};

#endif
