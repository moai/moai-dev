// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZERBASE_H
#define MOAISERIALIZERBASE_H

#include <moaicore/MOAIObject.h>
#include <moaicore/MOAIObject-impl.h>

//================================================================//
// MOAISerializerBase
//================================================================//
class MOAISerializerBase :
	public virtual MOAIObject {
protected:

	// maps IDs onto objects
	typedef STLMap < uintptr, MOAIObject* >::iterator ObjectMapIt;
	STLMap < uintptr, MOAIObject* > mObjectMap;

	// maps IDs onto tables
	typedef STLMap < uintptr, MOAILuaRef >::iterator TableMapIt;
	STLMap < uintptr, MOAILuaRef > mTableMap;

	//----------------------------------------------------------------//
	virtual cc8*	GetFileMagic			();
	uintptr			GetID					( MOAIObject* object );
	uintptr			GetID					( MOAILuaState& state, int idx );

public:

	//----------------------------------------------------------------//
	virtual void	Clear					();
					MOAISerializerBase		();
	virtual			~MOAISerializerBase	();
};

#endif
