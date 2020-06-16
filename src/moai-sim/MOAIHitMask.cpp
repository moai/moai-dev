// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMask.h>

//----------------------------------------------------------------//
int MOAIHitMask::_setImage ( MOAILuaState& state, u32 idx ) {
	MOAI_LUA_SETUP ( MOAIHitMask, "U" )

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMask::_setScalar ( MOAILuaState& state, u32 idx ) {
	MOAI_LUA_SETUP ( MOAIHitMask, "U" )

	ZLColorVec scalar = state.GetValue < ZLColorVec >( 2, ZLColorVec::WHITE );
	this->SetHitMaskScalar ( materialIdx, scalar.PackRGBA ());
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMask::_setThreshold ( MOAILuaState& state, u32 idx ) {
	MOAI_LUA_SETUP ( MOAIHitMask, "U" )

	ZLColorVec threshold = state.GetValue < ZLColorVec >( id2x, ZLColorVec ( 0.0f, 0.0f, 0.0f, 0.0f ));
	this->SetHitMaskThreshold ( materialIdx, threshold.PackRGBA ());

	return 0;
}

//----------------------------------------------------------------//
int MOAIHitMask::_testHit ( MOAILuaState& state, u32 idx ) {
	MOAI_LUA_SETUP ( MOAIHitMask, "U" )
	
	ZLReal x = state.GetValue < float >( 2, 0.0f );
	ZLReal y = state.GetValue < float >( 3, 0.0f );
	
	state.Push ( self->TestHit ( x, y );

	return 1;
}

//================================================================//
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHitMask::MOAIHitMask () {

	RTTI_BEGIN ( MOAILayoutFrame )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHitMask >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHitMask::~MOAIHitMask () {
}

//----------------------------------------------------------------//
void MOAIHitMask::SetScalar ( u32 scalar ) {

	this->mHitColorScalar = scalar;
}

//----------------------------------------------------------------//
void MOAIHitMask::SetThreshold ( u32 threshold ) {

	this->mHitColorThreshold = threshold;
}

//----------------------------------------------------------------//
bool MOAIHitMask::TestHit ( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	if ( !this->mImage ) return false;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY );
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return this->TestHit ( override, materialID, deckIndex, uv.mX, uv.mY );
	}

	return false;
}

//----------------------------------------------------------------//
bool MOAIHitMask::TestHit ( float x, float y ) {

	if ( !this->mImage ) return false;

	static const ZLColorVec defaultHitColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	if ( this->mHitMask ) {
	
		float width = ( float )this->mHitMask->GetWidth ();
		float height = ( float )this->mHitMask->GetHeight ();
	
		ZLColorVec scalar ( this->mHitColorScalar );
		ZLColorVec threshold ( this->mHitColorThreshold );
	
		ZLColorVec maskColor ( this->mHitMask->GetColor (( u32 )( width * x ), ( u32 )( height * y )));
		
		maskColor.Modulate ( scalar );
		
		return (
			( threshold.mR <= maskColor.mR ) &&
			( threshold.mG <= maskColor.mG ) &&
			( threshold.mB <= maskColor.mB ) &&
			( threshold.mA <= maskColor.mA )
		);
	}
	return true;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHitMask::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHitMask::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setImage",				_setImage },
		{ "setScalar",				_setScalar },
		{ "setThreshold",			_setThreshold },
		{ "testHit",				_testHit },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
