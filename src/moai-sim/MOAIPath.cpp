// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPath.h>

#define DEFAULT_FLATNESS 0.125f
#define DEFAULT_ANGLE 15.0f

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: path
int MOAIPath::_bless ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );

	self->Bless ();
	return 0;
}

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
int MOAIPath::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );
	
	state.Push ( self->GetLength ());
	return 1;
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
	float x			= state.GetValue < float >( 3, 0.0f );
	float y			= state.GetValue < float >( 4, 0.0f );
	
	self->SetPoint ( idx, x, y );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPath::_setThresholds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPath, "U" );

	float flatness	= state.GetValue < float >( 2, DEFAULT_FLATNESS );
	float angle		= state.GetValue < float >( 3, DEFAULT_ANGLE );
	
	self->SetFlatness ( flatness );
	self->SetAngle ( angle );
	
	return 0;
}

//================================================================//
// MOAIPath
//================================================================//

//----------------------------------------------------------------//
void MOAIPath::Bless () {

	size_t totalSegments = this->CountSegments ();
	
	this->mSegmentLengths.Init ( totalSegments );
	
	this->mLength = 0.0f;
	
	for ( size_t i = 0; i < totalSegments; ++i ) {
	
		ZLCubicBezier2D curve = this->GetSegment ( i );
		
		float length = curve.GetFlattenedLength ( this->mFlatness, this->mAngle );
		this->mSegmentLengths [ i ] = length;
		this->mLength += length;
	}
}

//----------------------------------------------------------------//
size_t MOAIPath::CountSegments () {

	return ( this->mControlPoints.Size () - 1 ) / 3;
}

//----------------------------------------------------------------//
ZLVec2D MOAIPath::Evaluate ( float t ) {

	ZLCubicBezier2D curve = this->GetSegmentForTime ( t, &t );
	return curve.Evaluate ( ZLFloat::Clamp ( t, 0.0f, 1.0f ));
}

//----------------------------------------------------------------//
ZLCubicBezier2D MOAIPath::GetSegment ( size_t idx ) {

	size_t basePoint = idx > 0 ? idx * 3 : 0;
	
	ZLCubicBezier2D curve;
	
	curve.Init (
		this->mControlPoints [ basePoint ],
		this->mControlPoints [ basePoint + 1 ],
		this->mControlPoints [ basePoint + 2 ],
		this->mControlPoints [ basePoint + 3 ]
	);
	
	return curve;
}

//----------------------------------------------------------------//
ZLCubicBezier2D MOAIPath::GetSegmentForTime ( float t, float* st ) {

	size_t totalSegments = this->CountSegments ();
	
	t = ZLFloat::Clamp ( t, 0.0f, 1.0f ) * ( float )totalSegments;
	
	float s = ZLFloat::Floor ( t );
	
	if ( st ) {
		( *st ) = t - s;
	}
	
	return this->GetSegment (( size_t )s );
}

//----------------------------------------------------------------//
MOAIPath::MOAIPath () :
	mFlatness ( DEFAULT_FLATNESS ),
	mAngle ( DEFAULT_ANGLE ),
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPath::~MOAIPath () {
}

//----------------------------------------------------------------//
void MOAIPath::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPath::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "bless",				_bless },
		{ "evaluate",			_evaluate },
		{ "getLength",			_getLength },
		{ "reserve",			_reserve },
		{ "setPoint",			_setPoint },
		{ "setThresholds",		_setThresholds },
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
