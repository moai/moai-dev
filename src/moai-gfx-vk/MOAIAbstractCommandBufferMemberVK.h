// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTCOMMANDBUFFERMEMBERVK_H
#define MOAIABSTRACTCOMMANDBUFFERMEMBERVK_H

class MOAICommandBufferVK;

//================================================================//
// MOAIAbstractCommandBufferMemberVK
//================================================================//
class MOAIAbstractCommandBufferMemberVK :
	public virtual ZLRefCountedObject {
private:

	friend class MOAICommandBufferVK;

	ZLLeanLink < MOAIAbstractCommandBufferMemberVK* > mLink;
	MOAICommandBufferVK* mCommandBuffer;

protected:

	//----------------------------------------------------------------//
	void				ForceRemove									();

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractCommandBufferMemberVK_Discard	() = 0;

public:
	
	//----------------------------------------------------------------//
	void				ForceUnpin									();
	bool				IsPinned									();
						MOAIAbstractCommandBufferMemberVK			();
						~MOAIAbstractCommandBufferMemberVK			();
};

#endif
