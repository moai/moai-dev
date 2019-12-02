// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTSNAPSHOTVK_H
#define MOAIABSTRACTSNAPSHOTVK_H

class MOAICommandBufferVK;

//================================================================//
// MOAIAbstractSnapshotVK
//================================================================//
class MOAIAbstractSnapshotVK :
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
						MOAIAbstractSnapshotVK				();
	virtual				~MOAIAbstractSnapshotVK				();
	
};

#endif
