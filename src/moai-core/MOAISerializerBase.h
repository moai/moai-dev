// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZERBASE_H
#define MOAISERIALIZERBASE_H

#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaSharedPtr.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAISerializerObjectEntry
//================================================================//
class MOAISerializerObjectEntry {
public:

	MOAILuaObject*		mObject;
	MOAILuaStrongRef	mLuaRef;
	STLString			mClassName;
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
	typedef STLMap < uintptr, MOAILuaStrongRef >::iterator TableMapIt;
	STLMap < uintptr, MOAILuaStrongRef > mTableMap;

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
