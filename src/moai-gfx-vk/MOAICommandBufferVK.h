// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMMANDBUFFERVK_H
#define MOAICOMMANDBUFFERVK_H

class MOAIAbstractCommandBufferResourceVK;

//================================================================//
// MOAICommandBufferVK
//================================================================//
class MOAICommandBufferVK {
private:

	friend class MOAIAbstractCommandBufferResourceVK;

	VkCommandBuffer											mCommandBuffer;
	ZLLeanList < MOAIAbstractCommandBufferResourceVK* >		mResources;
	bool													mIsValid;

	//----------------------------------------------------------------//
	void			Invalidate					();

public:

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
	void			PinResource					( MOAIAbstractCommandBufferResourceVK& resource );
	void			UnpinResources				();
};

#endif
