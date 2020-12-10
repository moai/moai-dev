// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOLABLEOBJECT_H
#define	MOAIPOOLABLEOBJECT_H

#include <moai-core/MOAILua.h>

//================================================================//
// MOAIPoolableObject
//================================================================//
// TODO: doxygen
class MOAIPoolableObject :
	public virtual MOAILuaObject {
private:

	friend class MOAIPool;

	u32					mPoolType;

	//----------------------------------------------------------------//
	void				ZLRefCountedObjectBase_OnRelease			( u32 refCount );
	void				ZLRefCountedObjectBase_OnRetain				( u32 refCount );
	virtual void		MOAIRenderResource_OnRemit					();

public:

	static const u32 NOT_IN_POOL = ( u32 )-1;

	//----------------------------------------------------------------//
	bool				IsInPool						();
						MOAIPoolableObject				();
						~MOAIPoolableObject				();
};

#endif
