// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIDeserializer.h>
#include <moai-core/MOAISerializer.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAIDeserializer
//================================================================//

//----------------------------------------------------------------//
int MOAIDeserializer::_base64Decode ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Base64Decode ( 1 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeserializer::_createObject ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeserializer, "US" );

	cc8* classname = state.GetValue < cc8* >( 2, 0 );
	if ( !classname ) return 0;
	
	lua_getglobal ( state, classname );
	
	if ( state.IsNil ()) {
		state.Pop ( 1 );
	}
	else {
		if ( state.GetFieldWithType ( -1, "new", LUA_TFUNCTION )) {
			lua_replace ( state, -2 );
			int status = state.DebugCall ( 0, 1 );
			if ( status == 0 ) return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIDeserializer::_initObject ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeserializer, "UU" );

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 2, false );
	if ( !object ) return 0;

	int memberTableIdx = 0;
	int initTableIdx = -1;

	if ( state.CheckParams ( 1, "UUS**T", false )) {
		
		memberTableIdx = 5;
		initTableIdx = 6;
	}
	else if ( state.CheckParams ( 1, "UU*T-", false )) {
	
		memberTableIdx = 3;
		initTableIdx = 4;
	}
	else if ( !state.CheckParams ( 1, "UUU-", false )) {
	
		return 0;
	}
	
	if (( memberTableIdx > 0 ) && ( state.IsType ( memberTableIdx, LUA_TTABLE ))) {
		object->SetMemberTable ( state, memberTableIdx );
	}
	
	if ( state.IsType ( initTableIdx, LUA_TTABLE )) {
		object->SerializeIn ( state, *self );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIDeserializer::_registerObjectID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeserializer, "UU" );

	ObjID memberID = state.GetValue < ObjID >( 3, 0 );
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
MOAILuaObject* MOAIDeserializer::MemberIDToObject ( ObjID objectID ) {

	if ( this->mObjectMap.contains ( objectID )) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		MOAISerializerObjectEntry& entry = this->mObjectMap [ objectID ];
		return entry.mObject;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIDeserializer::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "base64Decode",			_base64Decode },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeserializer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "createObject",			_createObject },
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
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	// load the lua file
	status = luaL_loadfile ( state, filename );
	if ( status ) return LOAD_ERROR;
	
	// load self as the func param
	this->PushLuaUserdata ( state );
	
	// call the chunk
	if ( state.DebugCall ( 1, 0 )) return LUA_ERROR;
	
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
