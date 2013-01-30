// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAISerializer.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaRef.h>

#include <moaicore/MOAILuaState-impl.h>
#include <moaicore/MOAIDeserializer.h>

//================================================================//
// MOAIDeserializer
//================================================================//

//----------------------------------------------------------------//
int MOAIDeserializer::_initObject ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeserializer, "UU*T" );

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 2, false );
	if ( !object ) return 0;

	if ( state.IsType ( 3, LUA_TTABLE )) {
		object->SetMemberTable ( state, 3 );
	}
	object->SerializeIn ( state, *self );

	return 0;
}

//----------------------------------------------------------------//
int MOAIDeserializer::_registerObjectID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeserializer, "UU" );

	uintptr memberID = state.GetValue < uintptr >( 3, 0 );
	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 2, false );
	if ( object ) {
		MOAISerializerObjectEntry& entry = self->mObjectMap [ memberID ];
		entry.mObject = object;
	}
	lua_pushvalue ( state, 2 );
	return 1;
}

//================================================================//
// MOAIDeserializer
//================================================================//

//----------------------------------------------------------------//
u32 MOAIDeserializer::IsLuaFile ( cc8* filename ) {

	ZLFILE* file = ( ZLFILE* )zl_fopen ( filename, "r" );
	if ( !file ) return LOAD_ERROR;
	
	char magic [ 256 ];
	char* str = zl_fgets ( magic, 6, file );
	UNUSED ( str );
	zl_fclose ( file );
	
	if ( strcmp ( magic, this->GetFileMagic ()) != 0 ) return INVALID_FILE;
	
	return SUCCESS;
}

//----------------------------------------------------------------//
MOAILuaObject* MOAIDeserializer::MemberIDToObject ( uintptr objectID ) {

	if ( this->mObjectMap.contains ( objectID )) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		MOAISerializerObjectEntry& entry = this->mObjectMap [ objectID ];
		return entry.mObject;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIDeserializer::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeserializer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "initObject",				_initObject },
		{ "registerObjectID",		_registerObjectID },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
u32 MOAIDeserializer::SerializeFromFile ( cc8* filename ) {
	
	this->Clear ();
	
	int status;
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	// load the lua file
	status = luaL_loadfile ( state, filename );
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return LOAD_ERROR;
	
	// load self as the func param
	this->PushLuaUserdata ( state );
	
	// call the chunk
	if ( state.DebugCall ( 1, 0 )) return LUA_ERROR;
	
	lua_gc ( state, LUA_GCCOLLECT, 0 );
	
	// done!
	return SUCCESS;
}

//----------------------------------------------------------------//
MOAIDeserializer::MOAIDeserializer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAISerializerBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeserializer::~MOAIDeserializer () {
}
