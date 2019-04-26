// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurveFloat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValueAtTime
	@text	Return the interpolated value given a point in time along the curve. This does not change
	        the curve's built in TIME attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurveFloat self
	@in		number time
	@out	number value	The interpolated value
*/
int MOAIAnimCurveFloat::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveFloat, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	float value = self->GetValue ( span );
	
	state.Push ( value );
	state.Push ( MOAILuaIndex ( span.mKeyID ));
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getValueRange
	@text	Returns the minimum and maximum values in the given time range. 
	
	@in		MOAIAnimCurveFloat self
	@in		number	start time
	@in		number	end time
	@out	number 	min value
	@out	number 	max value
*/
int MOAIAnimCurveFloat::_getValueRange ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveFloat, "UNN" );
	
	float t0 = state.GetValue < float >( 2, 0.0f );
	float t1 = state.GetValue < float >( 3, 0.0f );
	
	float min = 0.0f;
	float max = 0.0f;
	
	if ( t0 > t1 ) {
		float tmp = t0; t0 = t1; t1 = tmp;
	}
	
	self->GetValueRange ( t0, t1, min, max );
	state.Push ( min );
	state.Push ( max );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setKey
	@text	Initialize a key frame at a given time with a give value. Also set the transition type between
			the specified key frame and the next key frame.
	
	@in		MOAIAnimCurveFloat self
	@in		number index			Index of the keyframe.
	@in		number time				Location of the key frame along the curve.
	@in		number value			Value of the curve at time.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@opt	number weight			Blends between chosen ease type (of any) and a linear transition. Defaults to 1.
	@out	nil
*/
int MOAIAnimCurveFloat::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveFloat, "UNN" );

	ZLIndex index	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, ZLInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	if ( MOAILogMgr::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
	
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveFloat
//================================================================//

//----------------------------------------------------------------//
float MOAIAnimCurveFloat::GetCurveDelta () const {

	ZLSize size = this->mKeys.Size ();
	if ( size > 1 ) {
		return this->mSamples [ ZLIndexCast ( size - 1 )] - this->mSamples [ ZLIndexOp::ZERO ];
	}
	return 0.0f;
}

//----------------------------------------------------------------//
float MOAIAnimCurveFloat::GetSample ( ZLIndex id ) {

	if ( id < this->mKeys.Size ()) {
		return this->mSamples [ id ];
	}
	return 0.0f;
}

//----------------------------------------------------------------//
float MOAIAnimCurveFloat::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
float MOAIAnimCurveFloat::GetValue ( const MOAIAnimKeySpan& span ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	float v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
		v0 = ZLInterpolate::Interpolate ( key.mMode, v0, this->mSamples [ span.mKeyID + ( ZLSize )1 ], span.mTime, key.mWeight );
	}
	return v0 + ( this->GetCurveDelta () * span.mCycle );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::GetValueRange ( float t0, float t1, float &min, float &max ) {
	
	ZLIndex key0 = ZLIndexOp::ZERO;
	ZLIndex key1 = ZLIndexOp::ZERO;
	
	MOAIAnimKeySpan span0 = this->GetSpan ( t0 );
	MOAIAnimKeySpan span1 = this->GetSpan ( t1 );
	
	if ( t1 - t0 > this->GetLength ()) {
		key0 = ZLIndexOp::ZERO;
		key1 = ZLIndexCast ( this->Size () - 1 );
	}
	else {
		if ( span0.mKeyID > span1.mKeyID ) {
			
			key0 = span1.mKeyID;
			key1 = span0.mKeyID;
		}
		else {
			
			key0 = span0.mKeyID;
			key1 = span1.mKeyID;
		}
	}
	
	min = this->GetValue ( span0 );
	max = this->GetValue ( span0 );
	
	for ( ZLIndex id = key0 + ( ZLSize )1; id <= key1; id++ ) {
		
		float val = this->mSamples [ id ];
		
		if ( val < min ) {
			min = val;
		}
		if ( val > max ) {
			max = val;
		}
	}
	
	float last = this->GetValue ( span1 );
	
	if ( last < min ) {
		min = last;
	}
	if ( last > max ) {
		max = last;
	}
}

//----------------------------------------------------------------//
MOAIAnimCurveFloat::MOAIAnimCurveFloat () :
	mValue ( 0.0f ) {
	
	RTTI_SINGLE ( MOAIAnimCurve )
}

//----------------------------------------------------------------//
MOAIAnimCurveFloat::~MOAIAnimCurveFloat () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurve::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurve::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "getValueRange",		_getValueRange },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::SetSample ( ZLIndex idx, float value ) {

	if ( this->mKeys.CheckIndex ( idx )) {
		this->mSamples [ idx ] = value;
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAIAnimCurve_ApplyValueAttrOp ( ZLAttribute& attr, u32 op ) {

	this->mValue = attr.Apply ( this->mValue, op, ZLAttribute::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAIAnimCurve_GetDelta ( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	float v0 = this->GetValue ( span0 );
	float v1 = this->GetValue ( span1 );
	
	attr.SetValue ( v1 - v0 );
}

//----------------------------------------------------------------//
float MOAIAnimCurveFloat::MOAIAnimCurve_GetFloatForTime ( float t ) const {

	return this->GetValue ( t );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAIAnimCurve_GetValue ( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const {

	attr.SetValue ( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAIAnimCurve_GetZero ( ZLAttribute& attr ) const {

	attr.SetValue ( 0.0f );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAIAnimCurve_ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveFloat::MOAINode_Update () {

	this->mValue = this->GetValue ( this->mTime );
}
