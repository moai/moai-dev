// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTPINNABLEVK_H
#define MOAIABSTRACTPINNABLEVK_H

class MOAICommandBufferVK;

//================================================================//
// MOAIAbstractPinnableVK
//================================================================//
class MOAIAbstractPinnableVK :
	public virtual ZLRefCountedObject {
private:

	friend class MOAICommandBufferVK;

	ZLSize 				mCommandBufferRefCount;

	//----------------------------------------------------------------//
	void				Pin									();
	void				Unpin								();

protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractSnapshotVK_OnUnpin		() = 0;

public:
	
	//----------------------------------------------------------------//
	bool				IsPinned							();
						MOAIAbstractPinnableVK				();
	virtual				~MOAIAbstractPinnableVK				();
	
};

#endif
