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
int MOAIPlatformerDynamics2D::_jump ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->SetState ( STATE_JUMPING );
	
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

	self->mFallMidHandle.mX		= state.GetValue < float >( 3, 0.0f );
	self->mFallMidHandle.mY		= state.GetValue < float >( 2, 0.0f );
	self->mFallTopHandle.mX		= state.GetValue < float >( 5, 0.0f );
	self->mFallTopHandle.mY		= state.GetValue < float >( 4, 0.0f );
	self->mFallDuration			= state.GetValue < float >( 6, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setJumpParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mJumpMidHandle.mX		= state.GetValue < float >( 3, 0.0f );
	self->mJumpMidHandle.mY		= state.GetValue < float >( 2, 0.0f );
	self->mJumpTopHandle.mX		= state.GetValue < float >( 5, 0.0f );
	self->mJumpTopHandle.mY		= state.GetValue < float >( 4, 0.0f );
	self->mJumpDuration			= state.GetValue < float >( 6, 0.0f );

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

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//

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
		y = this->FindYForX ( this->mJumpMidHandle, this->mJumpTopHandle, t );
	
		draw.DrawLine ( x0, y0, x, y );
	}
	
	t = 0.0f;
	
	for ( u32 i = 0; i < resolution; ++i ) {
	
		float x0 = x;
		float y0 = y;
		
		t += step;
		x += xMove * step;
		y = this->FindYForX ( this->mFallMidHandle, this->mFallTopHandle, 1.0f - t ) + yOff;
		
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
float MOAIPlatformerDynamics2D::FindYForX ( const USVec2D& v0, const USVec2D& v1, float x ) {
	
	// midpoiny between first and second control points
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

	v = this->EvalCurve2D ( v0, v1, t );

	return v.mY;
}

//----------------------------------------------------------------//
bool MOAIPlatformerDynamics2D::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::MOAIPlatformerDynamics2D () :
	mJumpDuration ( 0.0f ),
	mFallDuration ( 0.0f ),
	mState ( STATE_NONE ),
	mWalkAcceleration ( 0.0f ),
	mWalkMax ( 0.0f ),
	mWalkDrag ( 0.0f ),
	mXMove ( 0.0f ) {
	
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
	
	MOAIPlatformerBody2D* body = this->mBody;
	if ( !body ) return;
	
	this->mXMove = this->mWalkDrag > 0.0f ? MOAIPlatformerDynamics2D::ApplyDrag ( this->mXMove, this->mWalkDrag * step ) : this->mXMove;
	this->mXMove += this->mWalkAcceleration * step;
	this->mXMove = USFloat::Clamp ( this->mXMove, -this->mWalkMax, this->mWalkMax );
	
	float yMove = 0.0f;
	
	if ( this->mState != STATE_NONE ) {
		
		if ( this->mState == STATE_JUMPING ) {
			
			float t = this->mStateTimer / this->mJumpDuration;
			
			if ( t > 1.0f ) {
				t = 1.0f;	
			}
			
			float stateY = this->FindYForX ( this->mJumpMidHandle, this->mJumpTopHandle, t );
			yMove += stateY - this->mStateY;
			this->mStateY = stateY;
			
			if ( t >= 1.0f ) {
				this->mState = STATE_FALLING;
				this->mStateTimer -= this->mJumpDuration;
				this->mStateY = 0.0f;
			}
		}
		
		if ( this->mState == STATE_FALLING ) {
			
			float t = this->mStateTimer / this->mFallDuration;
			
			if ( t > 1.0f ) {
				t = 1.0f;
				this->mState = STATE_NONE;
			}
			
			float stateY = this->FindYForX ( this->mFallMidHandle, this->mFallTopHandle, 1.0f - t ) - this->mFallTopHandle.mY;
			yMove += stateY - this->mStateY;
			this->mStateY = stateY;
		}
		
		this->mStateTimer += step;
	}
	
	body->SetMove ( this->mXMove * step, yMove );
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
		{ "jump",					_jump },
		{ "setBody",				_setBody },
		{ "setFallParams",			_setFallParams },
		{ "setJumpParams",			_setJumpParams },
		{ "setWalkParams",			_setWalkParams },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::SetState ( u32 state ) {

	this->mState = state;
	this->mStateTimer = 0.0f;
	this->mStateY = 0.0f;
}
