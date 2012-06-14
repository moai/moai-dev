// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZERBASE_H
#define MOAISERIALIZERBASE_H

#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaSharedPtr.h>

#include <moaicore/MOAILuaState-impl.h>
#include <moaicore/MOAILuaObject-impl.h>

//================================================================//
// MOAISerializerObjectEntry
//================================================================//
class MOAISerializerObjectEntry {
public:

	MOAILuaObject*	mObject;
	MOAILuaRef		mLuaRef;
	STLString		mClassName;
};

//================================================================//
// MOAISerializerBase
//================================================================//
class MOAISerializerBase :
	public virtual MOAILuaObject {
protected:

	// maps IDs onto objects
	typedef STLMap < uintptr, MOAISerializerObjectEntry >::iterator ObjectMapIt;
	STLMap < uintptr, MOAISerializerObjectEntry > mObjectMap;

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
