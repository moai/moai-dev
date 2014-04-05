// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAISerializerBase.h>

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
uintptr MOAISerializerBase::GetID ( MOAILuaObject* object ) {

	MOAIScopedLuaState state = object->GetSelf ();
	return ( uintptr )lua_topointer ( state, -1 );
}

//----------------------------------------------------------------//
uintptr MOAISerializerBase::GetID ( MOAILuaState& state, int idx ) {

	return ( uintptr )lua_topointer ( state, idx );
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
