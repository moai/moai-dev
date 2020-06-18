// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIAbstractHitMask.h>
#include <moai-sim/MOAIAbstractHasHitMask.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractHasHitMask::_setHitMaskImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMask, "U" )

	MOAIImage* image = MOAIImage::AffirmImage ( state, 2 );
	self->SetImage ( image );

	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractHasHitMask::_setHitMaskScalar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMask, "U" )

	ZLColorVec scalar = state.GetValue < ZLColorVec >( 2, ZLColorVec::WHITE );
	self->SetScalar ( scalar.PackRGBA ());
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractHasHitMask::_setHitMaskThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMask, "U" )

	ZLColorVec threshold = state.GetValue < ZLColorVec >( 2, ZLColorVec::CLEAR );
	self->SetThreshold ( threshold.PackRGBA ());

	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractHasHitMask::_testHit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasHitMask, "U" )
	
	ZLReal x = state.GetValue < float >( 2, 0.0f );
	ZLReal y = state.GetValue < float >( 3, 0.0f );
	
	state.Push ( self->TestHit ( x, y ));

	return 1;
}

//================================================================//
// MOAIAbstractHasHitMask
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIAbstractHasHitMask::AffirmHitMask () {

	return this->MOAIAbstractHasHitMask_AffirmHitMask ();
}

//----------------------------------------------------------------//
MOAIAbstractHasHitMask::MOAIAbstractHasHitMask () {

	RTTI_BEGIN ( MOAIAbstractHasHitMask )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractHasHitMask >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasHitMask::~MOAIAbstractHasHitMask () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMask::SetImage ( MOAIImage* image ) {

	this->AffirmHitMask ().MOAIAbstractHitMask_SetImage ( image );
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMask::SetScalar ( u32 scalar ) {

	this->AffirmHitMask ().MOAIAbstractHitMask_SetScalar ( scalar );
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMask::SetThreshold ( u32 threshold ) {

	this->AffirmHitMask ().MOAIAbstractHitMask_SetThreshold ( threshold );
}

//----------------------------------------------------------------//
bool MOAIAbstractHasHitMask::TestHit ( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	return this->AffirmHitMask ().MOAIAbstractHitMask_TestHit ( modelQuad, uvQuad, x, y );
}

//----------------------------------------------------------------//
bool MOAIAbstractHasHitMask::TestHit ( float x, float y ) {

	return this->AffirmHitMask ().MOAIAbstractHitMask_TestHit ( x, y );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasHitMask::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractHasHitMask::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setHitMaskImage",		_setHitMaskImage },
		{ "setHitMaskScalar",		_setHitMaskScalar },
		{ "setHitMaskThreshold",	_setHitMaskThreshold },
		{ "testHit",				_testHit },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
