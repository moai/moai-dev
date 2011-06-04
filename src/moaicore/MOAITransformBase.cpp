// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
/**	@name	getWorldDir
	@text	Returns the normalized direction vector ot the transform.
			This value is returned in world space so includes parent
			transforms (if any).
	
	@in		MOAITransform self
	@out	number xDirection
	@out	number yDirection
*/
int MOAITransformBase::_getWorldDir ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D direction = self->mLocalToWorldMtx.GetHeading ();

	lua_pushnumber ( state, direction.mX );
	lua_pushnumber ( state, direction.mY );

	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITransformBase::_getWorldLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D loc = self->mLocalToWorldMtx.GetTranslation ();

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITransformBase::_getWorldRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	float rot = self->mLocalToWorldMtx.GetRot ();
	
	lua_pushnumber ( state, rot );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITransformBase::_getWorldScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D scale = self->mLocalToWorldMtx.GetStretch ();
	
	lua_pushnumber ( state, scale.mX );
	lua_pushnumber ( state, scale.mY );

	return 2;
}

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITransformBase::GetTransformTrait () {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransformBase::MOAITransformBase () {
	
	RTTI_SINGLE ( MOAITraits )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransformBase::~MOAITransformBase () {
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaClass ( USLuaState& state ) {
	
	MOAITraits::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITraits::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getWorldDir",		_getWorldDir },
		{ "getWorldLoc",		_getWorldLoc },
		{ "getWorldRot",		_getWorldRot },
		{ "getWorldScl",		_getWorldScl },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAITransformBase::ToString () {

	STLString repr;
	return repr;
}
