// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAnimCurveVec.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValueAtTime
	@text	Return the interpolated vector components given a point in
			time along the curve. This does not change the curve's built in TIME
			attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurveVec self
	@in		number time
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIAnimCurveVec::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveVec, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	ZLVec3D value = self->GetValue ( span );
	
	state.Push ( value.mX );
	state.Push ( value.mY );
	state.Push ( value.mZ );
	
	state.Push ( MOAILuaIndex ( span.mKeyID ));
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	setKey
	@text	Initialize a key frame at a given time with a give vector.
			Also set the transition type between the specified key frame
			and the next key frame.
	
	@in		MOAIAnimCurveVec self
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

	ZLIndex index	= state.GetValue < MOAILuaIndex >( 2, 0 );
	float time		= state.GetValue < float >( 3, 0.0f );
	ZLVec3D value	= state.GetVec3D < float >( 4 );
	u32 mode		= state.GetValue < u32 >( 7, ZLInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 8, 1.0f );
	
	if ( MOAILogMgr::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveVec
//================================================================//

//----------------------------------------------------------------//
ZLVec3D MOAIAnimCurveVec::GetCurveDelta () const {

	ZLVec3D delta;

	ZLSize size = this->mKeys.Size ();
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
ZLVec3D MOAIAnimCurveVec::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
ZLVec3D MOAIAnimCurveVec::GetValue ( const MOAIAnimKeySpan& span ) const {

	const MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	ZLVec3D v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
	
		ZLVec3D v1 = this->mSamples [ span.mKeyID + ( ZLSize )1 ];
	
		v0.mX = ZLInterpolate::Interpolate ( key.mMode, v0.mX, v1.mX, span.mTime, key.mWeight );
		v0.mY = ZLInterpolate::Interpolate ( key.mMode, v0.mY, v1.mY, span.mTime, key.mWeight );
		v0.mZ = ZLInterpolate::Interpolate ( key.mMode, v0.mZ, v1.mZ, span.mTime, key.mWeight );
	}
	
	if ( span.mCycle != 0.0f ) {
		ZLVec3D curveDelta = this->GetCurveDelta ();
		curveDelta.Scale ( span.mCycle );
		v0.Add ( curveDelta );
	}
	return v0;
}

//----------------------------------------------------------------//
MOAIAnimCurveVec::MOAIAnimCurveVec () :
	mValue ( 0.0f, 0.0f, 0.0f ) {
		
	RTTI_BEGIN ( MOAIAnimCurveVec )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAnimCurveVec >)
		RTTI_EXTEND ( MOAIAnimCurve )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAnimCurveVec::~MOAIAnimCurveVec () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::SetSample ( ZLIndex idx, const ZLVec3D& value ) {

	if ( this->mKeys.CheckIndex ( idx )) {
		this->mSamples [ idx ] = value;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveVec::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAIAnimCurve_ApplyValueAttrOp ( ZLAttribute& attr, u32 op ) {

	this->mValue = attr.Apply ( this->mValue, op, ZLAttribute::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAIAnimCurve_GetDelta ( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	ZLVec3D v0 = this->GetValue ( span0 );
	ZLVec3D v1 = this->GetValue ( span1 );
	
	v1.Sub ( v0 );
	
	attr.SetValue ( v1 );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAIAnimCurve_GetValue ( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const {

	attr.SetValue ( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAIAnimCurve_GetZero ( ZLAttribute& attr ) const {

	ZLVec3D zero ( 0.0f, 0.0f, 0.0f );
	attr.SetValue ( zero );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAIAnimCurve_ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveVec::MOAINode_Update () {

	this->mValue = this->GetValue ( this->mTime );
}
