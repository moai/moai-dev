// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISEMAPHOREVK_H
#define MOAISEMAPHOREVK_H

//================================================================//
// MOAISemaphoreVK
//================================================================//
class MOAISemaphoreVK :
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
public:

	IMPLEMENT_FINALIZABLE ( MOAISemaphoreVK )

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
