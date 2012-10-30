// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIPlatformerBody2D.h>
#include <moaicore/MOAIPlatformerDynamics2D.h>
#include <moaicore/MOAIPlatformerDynamics2D_states.h>
#include <moaicore/MOAISim.h>

//================================================================//
// MOAIPlatformerArc
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerArc::Draw ( u32 resolution, float xOff, float yOff, float xScl, float yScl ) {
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	float x1 = 0.0f;
	float y1 = this->Evaluate ( x1 );
	
	float step = 1.0f / ( float )resolution;
	
	for ( u32 i = 0; i < resolution; ++i ) {
	
		float x0 = x1;
		float y0 = y1;
		
		x1 += step;
		y1 = this->Evaluate ( x1 );
	
		draw.DrawLine (( x0 * xScl ) + xOff, ( y0 * yScl ) + yOff, ( x1 * xScl ) + xOff, ( y1 * yScl ) + yOff );
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::DrawHull ( float xOff, float yOff, float xScl, float yScl ) {

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	USVec2D points [ 4 ];
	this->GetPoints ( points );
	
	draw.DrawLine (
		( points [ 0 ].mX * xScl ) + xOff,
		( points [ 0 ].mY * yScl ) + yOff,
		( points [ 1 ].mX * xScl ) + xOff,
		( points [ 1 ].mY * yScl ) + yOff
	);
	
	draw.DrawLine (
		( points [ 1 ].mX * xScl ) + xOff,
		( points [ 1 ].mY * yScl ) + yOff,
		( points [ 2 ].mX * xScl ) + xOff,
		( points [ 2 ].mY * yScl ) + yOff
	);
	
	draw.DrawLine (
		( points [ 2 ].mX * xScl ) + xOff,
		( points [ 2 ].mY * yScl ) + yOff,
		( points [ 3 ].mX * xScl ) + xOff,
		( points [ 3 ].mY * yScl ) + yOff
	);
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::DrawPoints ( float xOff, float yOff, float xScl, float yScl ) {

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	USVec2D points [ 4 ];
	this->GetPoints ( points );

	draw.DrawPoint (( points [ 0 ].mX * xScl ) + xOff, ( points [ 0 ].mY * yScl ) + yOff );
	draw.DrawPoint (( points [ 1 ].mX * xScl ) + xOff, ( points [ 1 ].mY * yScl ) + yOff );
	draw.DrawPoint (( points [ 2 ].mX * xScl ) + xOff, ( points [ 2 ].mY * yScl ) + yOff );
	draw.DrawPoint (( points [ 3 ].mX * xScl ) + xOff, ( points [ 3 ].mY * yScl ) + yOff );
}

//----------------------------------------------------------------//
float MOAIPlatformerArc::EvalCurve1D ( const USVec2D& v0, const USVec2D& v1, float t ) {

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
USVec2D MOAIPlatformerArc::EvalCurve2D ( const USVec2D& v0, const USVec2D& v1, float t ) {

	// x0, y0 are always ( 0.0f, 0.0f )

	// first contro point	
	float x1 = v0.mX;
	float y1 = v0.mY;
	
	// second control point
	float x3 = v1.mX;
	float y3 = v1.mY;
	
	// midpoint between first and second control points
	float x2 = x1 + (( x3 - x1 ) * 0.5f );
	float y2 = y1 + (( y3 - y1 ) * 0.5f );
	
	// x4, y4 are always ( 1.0f, y3 )
	
	float xp0;
	float yp0;
	
	float xp1;
	float yp1;
	
	// find out which side of the midpoint t is on
	if ( t <= 0.5f ) {
	
		// this is the first quadratic (x0, y1 to x2, y2)
		
		// normalize time to first half of curve
		t = t / 0.5f;
		
		// final points to interpolate
		xp0 = x1 * t;
		yp0 = y1 * t;
		xp1 = x1 + (( x2 - x1 ) * t );
		yp1 = y1 + (( y2 - y1 ) * t );
	}
	else {
		
		// this is the second quadratic (x2, y2 to 1.0f, y3)
		
		// normalize time to second half of curve
		t = ( t - 0.5f ) / ( 1.0f - 0.5f );
		
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
float MOAIPlatformerArc::Evaluate ( float time ) {

	if ( this->mDuration <= 0.0f ) return 0.0f;

	time /= this->mDuration;
	if ( this->mReverse ) {
		time = 1.0f - time;
	}
	
	if ( time >= 1.0f ) {
		return this->mTopHandle.mY;
	}
	else if ( time <= 0.0f ) {
		return this->mMidHandle.mX > 0.0f ? time * ( this->mMidHandle.mY / this->mMidHandle.mX ) : 0.0f;
	}
	
	return this->FindYForX ( this->mMidHandle, this->mTopHandle, time );
}

//----------------------------------------------------------------//
float MOAIPlatformerArc::FindYForX ( const USVec2D& v0, const USVec2D& v1, float x ) {

	// midpoint between first and second control points
	float x2 = v0.mX + (( v1.mX - v0.mX ) * 0.5f );
	
	float t = 0.0f;

	float p0, p1, p2;

	if ( x <= x2 ) {
		p0 = 0.0f; //x0
		p1 = v0.mX; //x1
		p2 = x2;
	} else {
		p0 = x2;
		p1 = v1.mX; //x3
		p2 = 1.0f; //x4
	}

	float a = p0 - ( 2 * p1 ) + p2;
	float b = ( -2 * p0 ) + ( 2 * p1 );
	float c = p0 - x;

	float t0 = (( -1 * b ) + sqrt ( b * b - 4 * a * c )) / ( 2 * a );
	float t1 = (( -1 * b ) - sqrt ( b * b - 4 * a * c )) / ( 2 * a );

	if ( t0 > 0.0f - 0.001f && t0 < 1.0f + 0.001f ) {
		t = t0 / 2.0f;
	} else {
		t = t1 / 2.0f;
	}

	if ( x > x2 ) {
		t += 0.5f;
	}

	USVec2D v;

	v = MOAIPlatformerArc::EvalCurve2D ( v0, v1, t );

	return v.mY;
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::GetPoints ( USVec2D* points ) {

	if ( this->mReverse ) {
		points [ 0 ].Init ( 0.0f, this->mTopHandle.mY );
		points [ 1 ].Init ( 1.0f - this->mTopHandle.mX, this->mTopHandle.mY );
		points [ 2 ].Init ( 1.0f - this->mMidHandle.mX, this->mMidHandle.mY );
		points [ 3 ].Init ( 1.0f, 0.0f );
	}
	else {
		points [ 0 ].Init ( 0.0f, 0.0f );
		points [ 1 ].Init ( this->mMidHandle.mX, this->mMidHandle.mY );
		points [ 2 ].Init ( this->mTopHandle.mX, this->mTopHandle.mY );
		points [ 3 ].Init ( 1.0f, this->mTopHandle.mY );
	}
}

//----------------------------------------------------------------//
MOAIPlatformerArc::MOAIPlatformerArc () :
	mDuration ( 1.0f ),
	mReverse ( false ) {

	this->mMidHandle.Init ( 0.0f, 0.0f );
	this->mTopHandle.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIPlatformerArc::~MOAIPlatformerArc () {
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::SetDuration ( float duration, bool reverse ) {

	this->mDuration = duration;
	this->mReverse = reverse;
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::SetMidHandle ( float time, float value ) {

	this->mMidHandle.mX = time;
	this->mMidHandle.mY = value;
}

//----------------------------------------------------------------//
void MOAIPlatformerArc::SetTopHandle ( float time, float value ) {

	this->mTopHandle.mX = time;
	this->mTopHandle.mY = value;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_boost ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	float duration = state.GetValue < float >( 2, 0.0f );

	if ( duration > 0.0f ) {
		MOAIPlatformerDynamics2D_Boost& boost = self->mFSM.SetState < MOAIPlatformerDynamics2D_Boost >();
		boost.mDuration = duration;
	}
	else {
		self->mFSM.SetState < MOAIPlatformerDynamics2D_Jump >();
	}
	return 0;
}

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

	float xMove = state.GetValue < float >( 2, 0.0f );
	self->DrawJumpHull ( xMove );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_drawJumpPoints ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	float xMove = state.GetValue < float >( 2, 0.0f );
	self->DrawJumpPoints ( xMove );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_jump ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mFSM.SetState < MOAIPlatformerDynamics2D_Jump >();
	
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

	self->mFallArc.SetMidHandle ( state.GetValue < float >( 2, 0.0f ), state.GetValue < float >( 3, 0.0f ));
	self->mFallArc.SetTopHandle ( state.GetValue < float >( 4, 0.0f ), state.GetValue < float >( 5, 0.0f ));
	self->mFallArc.SetDuration ( state.GetValue < float >( 6, 1.0f ), true );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setJumpParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mJumpArc.SetMidHandle ( state.GetValue < float >( 2, 0.0f ), state.GetValue < float >( 3, 0.0f ));
	self->mJumpArc.SetTopHandle ( state.GetValue < float >( 4, 0.0f ), state.GetValue < float >( 5, 0.0f ));
	self->mJumpArc.SetDuration ( state.GetValue < float >( 6, 1.0f ), false );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setWalkParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mWalkAcceleration = state.GetValue < float >( 2, 0.0f );
	self->mWalkMax = state.GetValue < float >( 3, 0.0f );
	self->mWalkDrag = state.GetValue < float >( 4, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_step ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	float step = state.GetValue < float >( 2, ( float )MOAISim::Get ().GetStep ());
	self->Step ( step );
	state.Push ( self->mState );
	
	return 1;
}

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::AccumulateMove ( float x, float y ) {

	this->mMove.mX += x;
	this->mMove.mY += y;
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D::ApplyDrag ( float n, float d ) {
	
	if ( n > 0.0f ) {
		if ( n > d ) {
			return n - d;
		}
	}
	else {
		if ( n < -d ) {
			return n + d;
		}
	}
	return 0.0f;
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpArc ( u32 resolution, float xMove ) {
	
	xMove *= 0.5f;
	resolution = resolution >> 1;
	
	this->mJumpArc.Draw ( resolution, 0.0f, 0.0f, xMove, 1.0f );
	
	float xOff = this->mJumpArc.mDuration * xMove;
	float yOff = this->mJumpArc.mTopHandle.mY - this->mFallArc.mTopHandle.mY;
	
	this->mFallArc.Draw ( resolution, xOff, yOff, xMove, 1.0f );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpHull ( float xMove ) {
	
	xMove *= 0.5f;
	
	this->mJumpArc.DrawHull ( 0.0f, 0.0f, xMove, 1.0f );
	
	float xOff = this->mJumpArc.mDuration * xMove;
	float yOff = this->mJumpArc.mTopHandle.mY - this->mFallArc.mTopHandle.mY;
	
	this->mFallArc.DrawHull ( xOff, yOff, xMove, 1.0f );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpPoints ( float xMove ) {
	
	xMove *= 0.5f;
	
	this->mJumpArc.DrawPoints ( 0.0f, 0.0f, xMove, 1.0f );
	
	float xOff = this->mJumpArc.mDuration * xMove;
	float yOff = this->mJumpArc.mTopHandle.mY - this->mFallArc.mTopHandle.mY;
	
	this->mFallArc.DrawPoints ( xOff, yOff, xMove, 1.0f );
}

//----------------------------------------------------------------//
bool MOAIPlatformerDynamics2D::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::MOAIPlatformerDynamics2D () :
	mWalkAcceleration ( 0.0f ),
	mWalkMax ( 0.0f ),
	mWalkDrag ( 0.0f ),
	mXMove ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mFSM.SetSelf ( *this );
	this->mFSM.SetState < MOAIPlatformerDynamics2D_Idle >();
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::~MOAIPlatformerDynamics2D () {

	this->mBody.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::OnUpdate ( float step ) {
	
	MOAIPlatformerBody2D* body = this->mBody;
	if ( !body ) return;
	
	this->mMove.Init ( 0.0f, 0.0f );
	this->mFSM.Update ( step );
	body->SetMove ( this->mMove.mX, this->mMove.mY );
	this->mStateTimer += step;
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaClass ( state );
	
	state.SetField ( -1, "STATE_BOOST",		STATE_BOOST );
	state.SetField ( -1, "STATE_FALL",		STATE_FALL );
	state.SetField ( -1, "STATE_IDLE",		STATE_IDLE );
	state.SetField ( -1, "STATE_JUMP",		STATE_JUMP );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "boost",					_boost },
		{ "drawJumpArc",			_drawJumpArc },
		{ "drawJumpHull",			_drawJumpHull },
		{ "drawJumpPoints",			_drawJumpPoints },
		{ "jump",					_jump },
		{ "setBody",				_setBody },
		{ "setFallParams",			_setFallParams },
		{ "setJumpParams",			_setJumpParams },
		{ "setWalkParams",			_setWalkParams },
		{ "step",					_step },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::UpdateXMove ( float step ) {

	this->mXMove = this->mWalkDrag > 0.0f ? MOAIPlatformerDynamics2D::ApplyDrag ( this->mXMove, this->mWalkDrag * step ) : this->mXMove;
	this->mXMove += this->mWalkAcceleration * step;
	this->mXMove = USFloat::Clamp ( this->mXMove, -this->mWalkMax, this->mWalkMax );
	
	this->mMove.mX += this->mXMove * step;
}
