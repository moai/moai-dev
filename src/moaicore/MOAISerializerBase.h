// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZERBASE_H
#define MOAISERIALIZERBASE_H

#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaObject-impl.h>

//================================================================//
// MOAISerializerBase
//================================================================//
class MOAISerializerBase :
	public virtual MOAILuaObject {
protected:

	// maps IDs onto objects
	typedef STLMap < uintptr, MOAILuaObject* >::iterator ObjectMapIt;
	STLMap < uintptr, MOAILuaObject* > mObjectMap;

	// maps IDs onto tables
	typedef STLMap < uintptr, MOAILuaRef >::iterator TableMapIt;
	STLMap < uintptr, MOAILuaRef > mTableMap;

	//----------------------------------------------------------------//
	virtual cc8*	GetFileMagic			();
	uintptr			GetID					( MOAILuaObject* object );
	uintptr			GetID					( MOAILuaState& state, int idx );

public:

	//----------------------------------------------------------------//
	virtual void	Clear					();
					MOAISerializerBase		();
	virtual			~MOAISerializerBase	();
};

#endif
