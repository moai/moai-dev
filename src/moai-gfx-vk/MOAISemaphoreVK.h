// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISEMAPHOREVK_H
#define MOAISEMAPHOREVK_H

//================================================================//
// MOAISemaphoreVK
//================================================================//
class MOAISemaphoreVK :
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	ZL_FINALIZATION_VISITOR_FRIEND

	//----------------------------------------------------------------//
	void 				_Finalize			();

public:

	IMPLEMENT_DEPENDS_ON ( MOAISemaphoreVK )

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
