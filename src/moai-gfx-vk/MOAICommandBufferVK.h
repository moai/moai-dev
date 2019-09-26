// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMMANDBUFFERVK_H
#define MOAICOMMANDBUFFERVK_H

//================================================================//
// MOAICommandBufferVK
//================================================================//
class MOAICommandBufferVK {
public:

	VkCommandBuffer		mCommandBuffer;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mCommandBuffer != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkCommandBuffer* () {
	
		return &this->mCommandBuffer;
	}
	
	//----------------------------------------------------------------//
	operator VkCommandBuffer& () {
	
		return this->mCommandBuffer;
	}
	
	//----------------------------------------------------------------//
				MOAICommandBufferVK			();
				~MOAICommandBufferVK		();
};

#endif
