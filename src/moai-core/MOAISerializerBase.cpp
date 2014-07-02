// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAISerializerBase.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAISerializerBase
//================================================================//

const MOAISerializerBase::ObjID MOAISerializerBase::NULL_OBJ_ID = 0;

//----------------------------------------------------------------//
void MOAISerializerBase::Clear () {

	this->mObjectIDs.clear ();
	this->mObjectMap.clear ();
	this->mTableMap.clear ();
}

//----------------------------------------------------------------//
cc8* MOAISerializerBase::GetFileMagic () {
	return "--MOAI";
}

//----------------------------------------------------------------//
MOAISerializerBase::ObjID MOAISerializerBase::GetID ( MOAILuaObject* object ) {

	MOAIScopedLuaState state = object->GetSelf ();
	return this->GetID ( state, -1 );
}

//----------------------------------------------------------------//
MOAISerializerBase::ObjID MOAISerializerBase::GetID ( MOAILuaState& state, int idx ) {

	void const* ptr = lua_topointer ( state, idx );
	
	if ( !this->mObjectIDs.contains ( ptr )) {
		this->mObjectIDs [ ptr ] = ( u32 )( this->mObjectIDs.size () + 1 );
	}
	return this->mObjectIDs [ ptr ];
}

//----------------------------------------------------------------//
MOAISerializerBase::MOAISerializerBase () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISerializerBase::~MOAISerializerBase () {

	this->Clear ();
}
