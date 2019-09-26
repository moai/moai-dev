// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTCOMMANDBUFFERRESOURCEVK_H
#define MOAIABSTRACTCOMMANDBUFFERRESOURCEVK_H

class MOAICommandBufferVK;

//================================================================//
// MOAIAbstractCommandBufferResourceVK
//================================================================//
class MOAIAbstractCommandBufferResourceVK {
private:

	friend class MOAICommandBufferVK;

	ZLLeanLink < MOAIAbstractCommandBufferResourceVK* > mLink;
	MOAICommandBufferVK* mCommandBuffer;

protected:

	//----------------------------------------------------------------//
	void				ForceRemove											();

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractCommandBufferResourceVK_Unpin			() = 0;

public:
	
	//----------------------------------------------------------------//
	void				ForceUnpin											();
	bool				IsPinned											();
						MOAIAbstractCommandBufferResourceVK					();
						~MOAIAbstractCommandBufferResourceVK				();
};

#endif
