// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIRegion.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_pointInside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UNN" )
	
	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );

	state.Push ( self->PointInside ( ZLVec2D ( x, y )));
	
	return 1;
}

//================================================================//
// MOAIRegion
//================================================================//

//----------------------------------------------------------------//
MOAIRegion::MOAIRegion () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRegion::~MOAIRegion () {
}

//----------------------------------------------------------------//
bool MOAIRegion::PointInside ( const ZLVec2D& p ) {

	bool inside = false;

	u32 nPolys = this->Size ();
	for ( u32 i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = ( *this )[ i ];
		if ( poly.PointInside ( p )) {
			inside = !inside;
		}
	}
	return inside;
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaFuncs ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// MOAIRegionBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "pointInside",	_pointInside },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
