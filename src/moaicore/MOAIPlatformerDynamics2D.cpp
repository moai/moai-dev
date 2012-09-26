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
int MOAIPlatformerDynamics2D::_setBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mBody.Set ( *self, state.GetLuaObject < MOAIPlatformerBody2D >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setFallParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mFallMid			= state.GetValue < float >( 2, self->mFallMid );
	self->mFallBottom		= state.GetValue < float >( 3, self->mFallBottom );
	self->mFallDuration		= state.GetValue < float >( 4, self->mFallDuration );
	self->mFallMode			= state.GetValue < u32 >( 5, self->mFallMode );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerDynamics2D::_setJumpParams ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerDynamics2D, "U" );

	self->mJumpMid			= state.GetValue < float >( 2, self->mJumpMid );
	self->mJumpTop			= state.GetValue < float >( 3, self->mJumpTop );
	self->mJumpDuration		= state.GetValue < float >( 4, self->mJumpDuration );
	self->mJumpMode			= state.GetValue < u32 >( 5, self->mJumpMode );

	return 0;
}

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerDynamics2D::DrawJumpArc ( u32 resolution, float xMove ) {
	
	resolution = resolution >> 1;
	xMove = xMove * 0.5f;
	
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
		y = this->EvalJump ( t );
	
		draw.DrawLine ( x0, y0, x, y );	
	}
	
	t = 0.0f;
	y = 0.0f;
	
	for ( u32 i = 0; i < resolution; ++i ) {
	
		float x0 = x;
		float y0 = y;
		
		t += step;
		x += xMove * step;
		y = this->EvalFall ( t );
		
		draw.DrawLine ( x0, y0 + this->mJumpTop, x, y + this->mJumpTop );	
	}
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D::EvalFall ( float t ) {
	
	t = USInterpolate::Curve ( this->mFallMode, t );
	
	float c1 = 0.0f;
	float c2 = this->mFallMid;
	float c3 = this->mFallBottom;
	
	float a1 = c1 + (( c2 - c1 ) * t );
	float a2 = c2 + (( c3 - c2 ) * t );
	
	float b1 = a1 * t;
	float b2 = a1 + (( a2 - a1 ) * t );
	
	return b1 + (( b2 - b1 ) * t );
}

//----------------------------------------------------------------//
float MOAIPlatformerDynamics2D::EvalJump ( float t ) {
	
	t = USInterpolate::Curve ( this->mJumpMode, t );
	
	float c0 = 0.0f;
	float c1 = this->mJumpMid;
	float c2 = this->mJumpTop;
	
	float a0 = c0 + (( c1 - c0 ) * t );
	float a1 = c1 + (( c2 - c1 ) * t );
	
	float b1 = a0 + (( a1 - a0 ) * t );
	float b2 = a1 + (( c2 - a1 ) * t );
	
	return b1 + (( b2 - b1 ) * t );
}

//----------------------------------------------------------------//
bool MOAIPlatformerDynamics2D::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIPlatformerDynamics2D::MOAIPlatformerDynamics2D () :
	mJumpMid ( 0.0f ),
	mJumpTop ( 0.0f ),
	mJumpDuration ( 0.0f ),
	mJumpElapsed ( 0.0f ),
	mJumpMode ( USInterpolate::kLinear ),
	mFallMid ( 0.0f ),
	mFallBottom ( 0.0f ),
	mFallDuration ( 0.0f ),
	mFallTerminal ( 0.0f ),
	mFallMode ( USInterpolate::kLinear ),
	mFlags ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
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
		{ "setBody",				_setBody },
		{ "setFallParams",			_setFallParams },
		{ "setJumpParams",			_setJumpParams },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
