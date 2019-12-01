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

	ZLLeanLink < MOAIAbstractSnapshotVK* > 		mLink;
	MOAICommandBufferVK* 						mCommandBuffer;

	//----------------------------------------------------------------//
	void				Remove								();

protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractSnapshotVK_OnUnpin		();

public:
	
	//----------------------------------------------------------------//
	bool				IsPinned							();
						MOAIAbstractSnapshotVK				();
	virtual				~MOAIAbstractSnapshotVK				();
	void				Pin									( MOAICommandBufferVK& commandBuffer );
	void				Unpin								();
};

#endif
