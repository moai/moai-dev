// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurveQuat.h>
#include <moaicore/MOAILogMessages.h>
#include <uslscore/USBinarySearch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveQuat::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveQuat, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	USQuaternion quat;
	quat = self->GetValue( time );
	
	USVec3D value;
	quat.Get ( value.mX, value.mY, value.mZ );
	state.Push ( value.mX );
	state.Push ( value.mY );
	state.Push ( value.mZ );
	return 3;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveQuat::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveQuat, "UNNNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	USVec3D value	= state.GetVec3D < float >( 4 );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		self->SetKey ( index, time, mode, 1.0f );
		self->SetSample ( index, value.mX, value.mY, value.mZ );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveQuat
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::ApplyValueAttrOp ( MOAIAttrOp& attrOp, u32 op ) {

	this->mValue = attrOp.Apply < USQuaternion >( this->mValue, op, MOAIAttrOp::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
USQuaternion MOAIAnimCurveQuat::GetCurveDelta () const {

	USQuaternion delta;

	u32 size = this->mKeys.Size ();
	if ( size > 1 ) {
		delta = this->mSamples [ size - 1 ];
		delta.Sub ( this->mSamples [ 0 ]);
	}
	else {
		delta.Set ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	return delta;
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::GetDelta ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	USQuaternion v0 = this->GetValue ( span0 );
	USQuaternion v1 = this->GetValue ( span1 );
	
	v1.Sub ( v0 );
	
	attrOp.SetValue < USQuaternion >( v1 );
}

//----------------------------------------------------------------//
USQuaternion MOAIAnimCurveQuat::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
USQuaternion MOAIAnimCurveQuat::GetValue ( const MOAIAnimKeySpan& span ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	USQuaternion v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
	
		USQuaternion v1 = this->mSamples [ span.mKeyID + 1 ];
		
		v0.Slerp ( v0, v1, USInterpolate::Curve ( key.mMode, span.mTime ));
	}
	
	if ( span.mCycle != 0.0f ) {
		USQuaternion curveDelta = this->GetCurveDelta ();
		curveDelta.Scale ( span.mCycle );
		v0.Add ( curveDelta );
	}
	return v0;
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::GetValue ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const {

	attrOp.SetValue < USQuaternion >( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::GetZero ( MOAIAttrOp& attrOp ) const {

	USQuaternion quat;
	quat.Set ( 0.0f, 0.0f, 0.0f, 0.0f );
	attrOp.SetValue < USQuaternion >( quat );
}

//----------------------------------------------------------------//
MOAIAnimCurveQuat::MOAIAnimCurveQuat () {
	
	RTTI_SINGLE ( MOAIAnimCurveBase )
	
	this->mValue.Identity ();
}

//----------------------------------------------------------------//
MOAIAnimCurveQuat::~MOAIAnimCurveQuat () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::OnDepNodeUpdate () {

	this->mValue = this->GetValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::SetSample ( u32 id, float x, float y, float z ) {

	if ( id < this->mKeys.Size ()) {
		this->mSamples [ id ].Set ( x, y, z );
	}
}
