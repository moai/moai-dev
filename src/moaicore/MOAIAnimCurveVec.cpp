// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurveVec.h>
#include <moaicore/MOAILogMessages.h>
#include <uslscore/USBinarySearch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getValueAtTime
	@text	Return the interpolated vector components given a point in
			time along the curve. This does not change the curve's built in TIME
			attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurveQuat self
	@in		number time
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIAnimCurveVec::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveVec, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	USVec3D value = self->GetValue ( time );
	state.Push ( value.mX );
	state.Push ( value.mY );
	state.Push ( value.mZ );
	return 3;
}

//----------------------------------------------------------------//
/**	@name	setKey
	@text	Initialize a key frame at a given time with a give vector.
			Also set the transition type between the specified key frame
			and the next key frame.
	
	@in		MOAIAnimCurve self
	@in		number index			Index of the keyframe.
	@in		number time				Location of the key frame along the curve.
	@in		number x				X component at time.
	@in		number y				Y component at time.
	@in		number z				Z component at time.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@opt	number weight			Blends between chosen ease type (of any) and a linear transition. Defaults to 1.
	@out	nil
*/
int MOAIAnimCurveVec::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveVec, "UNNNN" );

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
// MOAIAnimCurveVec
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveVec::ApplyValueAttrOp ( MOAIAttrOp& attrOp, u32 op ) {

	this->mValue = attrOp.Apply < USVec3D >( this->mValue, op, MOAIAttrOp::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec::GetCurveDelta () const {

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
void MOAIAnimCurveVec::GetDelta ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	USVec3D v0 = this->GetValue ( span0 );
	USVec3D v1 = this->GetValue ( span1 );
	
	v1.Sub ( v0 );
	
	attrOp.SetValue < USVec3D >( v1 );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
USVec3D MOAIAnimCurveVec::GetValue ( const MOAIAnimKeySpan& span ) const {

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
void MOAIAnimCurveVec::GetValue ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const {

	attrOp.SetValue < USVec3D >( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::GetZero ( MOAIAttrOp& attrOp ) const {

	USVec3D zero ( 0.0f, 0.0f, 0.0f );
	attrOp.SetValue < USVec3D >( zero );
}

//----------------------------------------------------------------//
MOAIAnimCurveVec::MOAIAnimCurveVec () :
	mValue ( 0.0f, 0.0f, 0.0f ) {
	
	RTTI_SINGLE ( MOAIAnimCurveBase )
}

//----------------------------------------------------------------//
MOAIAnimCurveVec::~MOAIAnimCurveVec () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::OnDepNodeUpdate () {

	this->mValue = this->GetValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::SetSample ( u32 id, const USVec3D& value ) {

	if ( id < this->mKeys.Size ()) {
		this->mSamples [ id ] = value;
	}
}
