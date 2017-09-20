// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxVertexCache.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValueAtTime
	@text	Return the interpolated value given a point in time along the curve. This does not change
	        the curve's built in TIME attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurve self
	@in		number time
	@out	number value	The interpolated value
*/
int MOAIAnimCurve::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	float value = self->GetValue ( span );
	
	state.Push ( value );
	state.Push ( span.mKeyID + 1 );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getValueRange
	@text	Returns the minimum and maximum values in the given time range. 
	
	@in		MOAIAnimCurve self
	@in		number	start time
	@in		number	end time
	@out	number 	min value
	@out	number 	max value
*/
int MOAIAnimCurve::_getValueRange ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UNN" );
	
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
	
	@in		MOAIAnimCurve self
	@in		number index			Index of the keyframe.
	@in		number time				Location of the key frame along the curve.
	@in		number value			Value of the curve at time.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@opt	number weight			Blends between chosen ease type (of any) and a linear transition. Defaults to 1.
	@out	nil
*/
int MOAIAnimCurve::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, ZLInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
	
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurve
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurve::ApplyValueAttrOp ( MOAIAttrOp& attrOp, u32 op ) {

	this->mValue = attrOp.Apply ( this->mValue, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::Draw ( u32 resolution ) const {

	// TODO: this isn't entirely correct. the value of each key frame should be drawn
	// and then the spans between keys should be filled in with an approximation of
	// the resolution.
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	float length = this->GetLength ();
	float step = length / ( float )resolution;
	
	gfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_LINE_STRIP );
	
	for ( u32 i = 0; i < resolution; ++i ) {
		
		float t = step * ( float )i;
		float v = this->GetValue ( t );
		
		gfxMgr.mVertexCache.WriteVtx ( t, v, 0.0f );
		gfxMgr.mVertexCache.WritePenColor4b ();
	}
	
	float t = length;
	float v = this->GetValue ( t );
	
	gfxMgr.mVertexCache.WriteVtx ( t, v, 0.0f );
	gfxMgr.mVertexCache.WritePenColor4b ();
	
	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetCurveDelta () const {

	size_t size = this->mKeys.Size ();
	if ( size > 1 ) {
		return this->mSamples [ size - 1 ] - this->mSamples [ 0 ];
	}
	return 0.0f;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetDelta ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	float v0 = this->GetValue ( span0 );
	float v1 = this->GetValue ( span1 );
	
	attrOp.SetValue < float >( v1 - v0, MOAIAttrOp::ATTR_TYPE_FLOAT );
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetSample ( u32 id ) {

	if ( id < this->mKeys.Size ()) {
		return this->mSamples [ id ];
	}
	return 0.0f;
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetValue ( const MOAIAnimKeySpan& span ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	float v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
		v0 = ZLInterpolate::Interpolate ( key.mMode, v0, this->mSamples [ span.mKeyID + 1 ], span.mTime, key.mWeight );
	}
	return v0 + ( this->GetCurveDelta () * span.mCycle );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetValue ( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const {

	attrOp.SetValue < float >( this->GetValue ( span ), MOAIAttrOp::ATTR_TYPE_FLOAT );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetValueRange ( float t0, float t1, float &min, float &max ) {
	
	u32 key0 = 0;
	u32 key1 = 0;
	
	MOAIAnimKeySpan span0 = this->GetSpan ( t0 );
	MOAIAnimKeySpan span1 = this->GetSpan ( t1 );
	
	if ( t1 - t0 > this->GetLength ()) {
		key0 = 0;
		key1 = this->Size () - 1;
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
	
	for ( u32 id = key0 + 1; id <= key1; id++ ) {
		
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
void MOAIAnimCurve::GetZero ( MOAIAttrOp& attrOp ) const {

	attrOp.SetValue < float >( 0.0f, MOAIAttrOp::ATTR_TYPE_FLOAT );
}

//----------------------------------------------------------------//
MOAIAnimCurve::MOAIAnimCurve () :
	mValue ( 0.0f ) {
	
	RTTI_SINGLE ( MOAIAnimCurveBase )
}

//----------------------------------------------------------------//
MOAIAnimCurve::~MOAIAnimCurve () {
}

//----------------------------------------------------------------//
void MOAIAnimCurve::OnDepNodeUpdate () {

	this->mValue = this->GetValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "getValueRange",		_getValueRange },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::SetSample ( u32 id, float value ) {

	if ( id < this->mKeys.Size ()) {
		this->mSamples [ id ] = value;
	}
}
