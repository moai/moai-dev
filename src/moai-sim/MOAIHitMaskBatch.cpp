// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMask.h>
#include <moai-sim/MOAIHitMaskBatch.h>

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_getHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_reserveHitMasks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_setHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_setImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_setScalar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_setThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMaskBatch::_testHit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHitMaskBatch, "U" )
	
	return 0;
}

//================================================================//
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHitMaskBatch::MOAIHitMaskBatch () {

	RTTI_BEGIN ( MOAILayoutFrame )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHitMaskBatch >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHitMaskBatch::~MOAIHitMaskBatch () {
}

//----------------------------------------------------------------//
void MOAIHitMaskBatch::SetScalar ( u32 idx, u32 scalar ) {

	this->mHitMasks [ idx ]->SetScalar ( scalar );
}

//----------------------------------------------------------------//
void MOAIHitMaskBatch::SetThreshold ( u32 idx, u32 threshold ) {

	this->mHitMasks [ idx ]->SetThreshold ( threshold );
}

//----------------------------------------------------------------//
bool MOAIHitMaskBatch::TestHit ( u32 idx, const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	return this->mHitMasks [ idx ]->TestHit ( modelQuad, uvQuad, x, y );
}

//----------------------------------------------------------------//
bool MOAIHitMaskBatch::TestHit ( u32 idx, float x, float y ) {

	return this->mHitMasks [ idx ]->TestHit ( x, y );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHitMaskBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHitMaskBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getHitMask",				_getHitMask },
		{ "reserveHitMasks",		_reserveHitMasks },
		{ "setHitMask",				_setHitMask },
		{ "setImage",				_setImage },
		{ "setScalar",				_setScalar },
		{ "setThreshold",			_setThreshold },
		{ "testHit",				_testHit },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
