// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFENCEVK_H
#define MOAIFENCEVK_H

class MOAILogicalDeviceVK;

//================================================================//
// MOAIFenceVK
//================================================================//
class MOAIFenceVK :
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
public:

	IMPLEMENT_FINALIZABLE ( MOAIFenceVK )

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
