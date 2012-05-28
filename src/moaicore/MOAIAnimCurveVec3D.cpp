// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurveVec3D.h>
#include <moaicore/MOAILogMessages.h>
#include <uslscore/USBinarySearch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveVec3D::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveVec3D, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	USVec3D value = self->GetValue ( time );
	state.Push ( value.mX );
	state.Push ( value.mY );
	state.Push ( value.mZ );
	return 3;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveVec3D::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveVec3D, "UNNNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	USVec3D value	= state.GetVec3D < float >( 4 );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 8, 1.0f );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveVec3D
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::ApplyValueAttrOp ( MOAIAttrOp& attrOp, u32 op ) {

	this->mValue = attrOp.Apply < USVec3D >( this->mValue, op, MOAIAttrOp::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec3D::GetCurveDelta () const {

	USVec3D delta;

	u32 size = this->mKeys.Size ();
	if ( size > 1 ) {
		delta = this->mSamples [ size - 1 ];
		delta.Sub ( this->mSamples [ 0 ]);
	}
	else {
		delta.Init ( 0.0f, 0.0f, 0.0f );
	}
	return delta;
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::GetDelta ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	USVec3D v0 = this->GetValue ( span0 );
	USVec3D v1 = this->GetValue ( span1 );
	
	v1.Sub ( v0 );
	
	attrOp.SetValue < USVec3D >( v1 );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec3D::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec3D::GetValue ( const MOAIAnimKeySpan& span ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	USVec3D v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
	
		USVec3D v1 = this->mSamples [ span.mKeyID + 1 ];
	
		v0.mX = USInterpolate::Interpolate ( key.mMode, v0.mX, v1.mX, span.mTime, key.mWeight );
		v0.mY = USInterpolate::Interpolate ( key.mMode, v0.mY, v1.mY, span.mTime, key.mWeight );
		v0.mZ = USInterpolate::Interpolate ( key.mMode, v0.mZ, v1.mZ, span.mTime, key.mWeight );
	}
	
	if ( span.mCycle != 0.0f ) {
		USVec3D curveDelta = this->GetCurveDelta ();
		curveDelta.Scale ( span.mCycle );
		v0.Add ( curveDelta );
	}
	return v0;
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::GetValue ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const {

	attrOp.SetValue < USVec3D >( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::GetZero ( MOAIAttrOp& attrOp ) const {

	USVec3D zero ( 0.0f, 0.0f, 0.0f );
	attrOp.SetValue < USVec3D >( zero );
}

//----------------------------------------------------------------//
MOAIAnimCurveVec3D::MOAIAnimCurveVec3D () :
	mValue ( 0.0f, 0.0f, 0.0f ) {
	
	RTTI_SINGLE ( MOAIAnimCurveBase )
}

//----------------------------------------------------------------//
MOAIAnimCurveVec3D::~MOAIAnimCurveVec3D () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::OnDepNodeUpdate () {

	this->mValue = this->GetValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec3D::SetSample ( u32 id, const USVec3D& value ) {

	if ( id < this->mKeys.Size ()) {
		this->mSamples [ id ] = value;
	}
}
