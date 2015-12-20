// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPath.h>
#include <moai-sim/MOAIGfxDevice.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: path
int MOAIPath::_evaluate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );

	float t			= state.GetValue < float >( 2, 0.0f );
	
	ZLVec2D result = self->Evaluate ( t );

	state.Push ( result.mX );
	state.Push ( result.mY );

	return 2;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPath::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );

	size_t size = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( size );
	return 0;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPath::_setPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );

	size_t idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	float x			= state.GetValue < float >( 2, 0.0f );
	float y			= state.GetValue < float >( 3, 0.0f );
	
	self->SetPoint ( idx, x, y );
	
	return 0;
}

//================================================================//
// MOAIPath
//================================================================//

//----------------------------------------------------------------//
ZLVec2D MOAIPath::Evaluate ( float t ) {

	ZLCubicBezier2D curve;
	t = this->GetCubicBezier ( t, curve );
	return curve.Evaluate ( ZLFloat::Clamp ( t, 0.0f, 1.0f ));
}

//----------------------------------------------------------------//
float MOAIPath::GetCubicBezier ( float t, ZLCubicBezier2D& curve ) {

	size_t totalSegments = this->mControlPoints.Size () >> 2;
	
	t = ZLFloat::Clamp ( t, 0.0f, 1.0f ) * ( float )totalSegments;
	
	float s = ZLFloat::Floor ( t );
	
	size_t basePoint = ( size_t )s << 2;
	
	curve.Init (
		this->mControlPoints [ basePoint ],
		this->mControlPoints [ basePoint + 1 ],
		this->mControlPoints [ basePoint + 2 ],
		this->mControlPoints [ basePoint + 3 ]
	);
	
	return t - s;
}

//----------------------------------------------------------------//
MOAIPath::MOAIPath () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
void MOAIPath::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIPath::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "evaluate",			_evaluate },
		{ "reserve",			_reserve },
		{ "setPoint",			_setPoint },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPath::Reserve ( size_t size ) {

	this->mControlPoints.Init ( size );
}

//----------------------------------------------------------------//
void MOAIPath::SetPoint ( size_t idx, float x, float y ) {

	this->mControlPoints [ idx ].Init ( x, y );
}
