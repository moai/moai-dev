// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZERBASE_H
#define MOAISERIALIZERBASE_H

#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaObject.h>

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
public:

	typedef u32 ObjID;
	static const ObjID NULL_OBJ_ID;

private:

	STLMap < const void*, u32 > mObjectIDs;

protected:

	// maps IDs onto objects
	typedef STLMap < ObjID, MOAISerializerObjectEntry >::iterator ObjectMapIt;
	STLMap < ObjID, MOAISerializerObjectEntry > mObjectMap;

	// maps IDs onto tables
	typedef STLMap < ObjID, MOAILuaStrongRef >::iterator TableMapIt;
	STLMap < ObjID, MOAILuaStrongRef > mTableMap;

	//----------------------------------------------------------------//
	virtual cc8*	GetFileMagic			();
	ObjID			GetID					( MOAILuaObject* object );
	ObjID			GetID					( MOAILuaState& state, int idx );

public:

	//----------------------------------------------------------------//
	virtual void	Clear					();
					MOAISerializerBase		();
	virtual			~MOAISerializerBase	();
};

#endif
