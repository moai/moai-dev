// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAISerializerBase.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAISerializerBase
//================================================================//

//----------------------------------------------------------------//
void MOAISerializerBase::Clear () {

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
	return ( ObjID )lua_topointer ( state, -1 );
}

//----------------------------------------------------------------//
MOAISerializerBase::ObjID MOAISerializerBase::GetID ( MOAILuaState& state, int idx ) {

	return ( ObjID )lua_topointer ( state, idx );
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
