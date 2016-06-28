// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#include "pch.h"

#include <moai-assimp/MOAIAssimpScene.h>
#include <moai-assimp/MOAIAssimpSceneMember.h>
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAssimpSceneMember::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpSceneMember, "U" )

	if ( self->mScene ) {
		state.Push ( self->mIndex );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpSceneMember::_getName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpSceneMember, "U" )

	if ( self->mScene ) {
		if ( self->mName.size ()) {
			state.Push ( self->mName.c_str ());
		}
		else {
			state.Push ( self->mIndex );
		}
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIAssimpSceneMember
//================================================================//

//----------------------------------------------------------------//
void MOAIAssimpSceneMember::DetachFromScene () {

	this->mScene = 0;
	this->mNode = 0;
	this->mName.clear ();
	this->mIndex = ( uint )-1;
}

//----------------------------------------------------------------//
MOAIAssimpSceneMember::MOAIAssimpSceneMember () :
	mScene ( 0 ),
	mIndex (( uint )-1 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAssimpSceneMember::~MOAIAssimpSceneMember () {
}

//----------------------------------------------------------------//
void MOAIAssimpSceneMember::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAssimpSceneMember::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "getName",				_getName },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
