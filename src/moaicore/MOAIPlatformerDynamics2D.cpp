// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIPlatformerBody2D.h>
#include <moaicore/MOAIPlatformerDynamics2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_drawJumpArc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	u32 resolution	= state.GetValue < u32 >( 2, 0 );
	float xMove		= state.GetValue < float >( 3, 0.0f );

	if ( resolution ) {
		self->DrawJumpArc ( resolution, xMove );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_drawJumpHull ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	float xMove		= state.GetValue < float >( 2, 0.0f );
	self->DrawJumpHull ( xMove );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_drawJumpPoints ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	float xMove		= state.GetValue < float >( 2, 0.0f );
	self->DrawJumpPoints ( xMove );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mBody.Set ( *self, state.GetLuaObject < MOAIPlatformerBody2D >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setFallParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mFallMidHandle.mX = state.GetValue < float >( 3, 0.0f );
	self->mFallMidHandle.mY = state.GetValue < float >( 2, 0.0f );
	self->mFallTopHandle.mX = state.GetValue < float >( 5, 0.0f );
	self->mFallTopHandle.mY = state.GetValue < float >( 4, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setJumpParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mJumpMidHandle.mX = state.GetValue < float >( 3, 0.0f );
	self->mJumpMidHandle.mY = state.GetValue < float >( 2, 0.0f );
	self->mJumpTopHandle.mX = state.GetValue < float >( 5, 0.0f );
	self->mJumpTopHandle.mY = state.GetValue < float >( 4, 0.0f );

	return 0;
}

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpArc ( u32 resolution, float xMove ) {
	
	resolution = resolution >> 1;
	xMove = xMove * 0.5f;
	float yOff = this->mJumpTopHandle.mY - this->mFallTopHandle.mY;
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	float t = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	
	float step = 1.0f / ( float )resolution;
	
	for ( u32 i = 0; i < resolution; ++i ) {
	
		float x0 = x;
		float y0 = y;
	
		t += step;
		x += xMove * step;
		y = this->FindYForX ( this->mJumpMidHandle, this->mJumpTopHandle, t, 10 );
	
		draw.DrawLine ( x0, y0, x, y );
	}
	
	t = 0.0f;
	
	for ( u32 i = 0; i < resolution; ++i ) {
	
		float x0 = x;
		float y0 = y;
		
		t += step;
		x += xMove * step;
		y = this->FindYForX ( this->mFallMidHandle, this->mFallTopHandle, 1.0f - t, 10 ) + yOff;
		
		draw.DrawLine ( x0, y0, x, y );
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpHull ( float xMove ) {
	
	xMove = xMove * 0.5f;
	float yOff = this->mJumpTopHandle.mY - this->mFallTopHandle.mY;
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	USVec2D v0 ( 0.0f, 0.0f );
	USVec2D v1 ( this->mJumpMidHandle.mX * xMove, this->mJumpMidHandle.mY );
	draw.DrawLine ( v0, v1 );
	
	v0 = v1;
	v1.Init ( this->mJumpTopHandle.mX * xMove, this->mJumpTopHandle.mY );
	draw.DrawLine ( v0, v1 );
	
	v0 = v1;
	v1.Init ( xMove, this->mJumpTopHandle.mY );
	draw.DrawLine ( v0, v1 );
	
	v0 = v1;
	v1.Init ( xMove + (( 1.0f - this->mFallTopHandle.mX ) * xMove ), this->mFallTopHandle.mY + yOff );
	draw.DrawLine ( v0, v1 );
	
	v0 = v1;
	v1.Init ( xMove + (( 1.0f - this->mFallMidHandle.mX ) * xMove ), this->mFallMidHandle.mY + yOff );
	draw.DrawLine ( v0, v1 );
	
	v0 = v1;
	v1.Init ( xMove + xMove, yOff );
	draw.DrawLine ( v0, v1 );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpPoints ( float xMove ) {
	
	xMove = xMove * 0.5f;
	float yOff = this->mJumpTopHandle.mY - this->mFallTopHandle.mY;
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	USVec2D v0;
	
	v0.Init ( 0.0f, 0.0f );
	draw.DrawPoint ( v0 );
	
	v0.Init ( this->mJumpMidHandle.mX * xMove, this->mJumpMidHandle.mY );
	draw.DrawPoint ( v0 );

	v0.Init ( this->mJumpTopHandle.mX * xMove, this->mJumpTopHandle.mY );
	draw.DrawPoint ( v0 );

	v0.Init ( xMove, this->mJumpTopHandle.mY );
	draw.DrawPoint ( v0 );

	v0.Init ( xMove + (( 1.0f - this->mFallTopHandle.mX ) * xMove ), this->mFallTopHandle.mY + yOff );
	draw.DrawPoint ( v0 );
	
	v0.Init ( xMove + (( 1.0f - this->mFallMidHandle.mX ) * xMove ), this->mFallMidHandle.mY + yOff );
	draw.DrawPoint ( v0 );
	
	v0.Init ( xMove + xMove, yOff );
	draw.DrawPoint ( v0 );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D::EvalCurve1D ( const USVec2D& v0, const USVec2D& v1, float t ) {
	
	float x1 = v0.mX;
	float y1 = v0.mY;
	
	float x3 = v1.mX;
	float y3 = v1.mY;
	
	float x2 = x1 + (( x3 - x1 ) * 0.5f );
	float y2 = y1 + (( y3 - y1 ) * 0.5f );
	
	float c0;
	float c1;
	
	if ( t <= x2 ) {
		t = t / x2;
		c0 = y1 * t;
		c1 = y1 + (( y2 - y1 ) * t );
	}
	else {
		t = ( t - x2 ) / ( 1.0f - x2 );
		c0 = y2 + (( y3 - y2 ) * t );
		c1 = y3;
	}
	
	return c0 + (( c1 - c0 ) * t );
}

//----------------------------------------------------------------//
USVec2D MOAIPlatformerDynamics2D::EvalCurve2D ( const USVec2D& v0, const USVec2D& v1, float t ) {
	
	// x0, y0 are always ( 0.0f, 0.0f )

	// first contro point	
	float x1 = v0.mX;
	float y1 = v0.mY;
	
	// second control point
	float x3 = v1.mX;
	float y3 = v1.mY;
	
	// midpoiny between first and second control points
	float x2 = x1 + (( x3 - x1 ) * 0.5f );
	float y2 = y1 + (( y3 - y1 ) * 0.5f );
	
	// x4, y4 are always ( 1.0f, y3 )
	
	float xp0;
	float yp0;
	
	float xp1;
	float yp1;
	
	// find out which side of the midpoint t is on
	if ( t <= x2 ) {
	
		// this is the first quadratic (x0, y1 to x2, y2)
		
		// normalize time to first half of curve
		t = t / x2;
		
		// final points to interpolate
		xp0 = x1 * t;
		yp0 = y1 * t;
		xp1 = x1 + (( x2 - x1 ) * t );
		yp1 = y1 + (( y2 - y1 ) * t );
	}
	else {
		
		// this is the second quadratic (x2, y2 to 1.0f, y3)
		
		// normalize time to second half of curve
		t = ( t - x2 ) / ( 1.0f - x2 );
		
		// final points to interpolate
		xp0 = x2 + (( x3 - x2 ) * t );
		yp0 = y2 + (( y3 - y2 ) * t );
		xp1 = x3 + (( 1.0f - x3 ) * t );
		yp1 = y3;
	}
	
	// interpolate the result
	USVec2D r ( xp0 + (( xp1 - xp0 ) * t ), yp0 + (( yp1 - yp0 ) * t ));
	return r;
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D::FindYForX ( const USVec2D& v0, const USVec2D& v1, float x, u32 iterations ) {

	float t = 0.5f;
	float s = 0.5f;

	USVec2D v;
	for ( u32 i = 0; i < iterations; ++i ) {
		v = this->EvalCurve2D ( v0, v1, t );
		t += x < v.mX ? -s : s;
		s *= 0.5f;
	}
	
	return v.mY;
}

//----------------------------------------------------------------//
bool MOAIPlatformerDynamics2D::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::MOAIPlatformerDynamics2D () :
	mJumpDuration ( 0.0f ),
	mJumpElapsed ( 0.0f ),
	mFallDuration ( 0.0f ),
	mFlags ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mJumpMidHandle.Init ( 0.0f, 0.0f );
	this->mJumpTopHandle.Init ( 0.0f, 0.0f );
	
	this->mFallMidHandle.Init ( 0.0f, 0.0f );
	this->mFallMidHandle.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::~MOAIPlatformerDynamics2D () {

	this->mBody.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::OnUpdate ( float step ) {
	UNUSED ( step );
	
	MOAIPlatformerBody2D* body = this->mBody;
	if ( !body ) return;
	
	body->SetMove ( 0.0f, -1.0f );
	body->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "drawJumpArc",			_drawJumpArc },
		{ "drawJumpHull",			_drawJumpHull },
		{ "drawJumpPoints",			_drawJumpPoints },
		{ "setBody",				_setBody },
		{ "setFallParams",			_setFallParams },
		{ "setJumpParams",			_setJumpParams },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
