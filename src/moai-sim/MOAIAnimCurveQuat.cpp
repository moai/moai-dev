// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurveQuat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValueAtTime
	@text	Return the interpolated value (as Euler angles) given a point in
			time along the curve. This does not change the curve's built in TIME
			attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurveQuat self
	@in		number time
	@out	number xRot
	@out	number yRot
	@out	number zRot
*/
int MOAIAnimCurveQuat::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveQuat, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	ZLQuaternion quat = self->GetValue ( span );
	
	ZLVec3D value;
	quat.Get ( value.mX, value.mY, value.mZ );

	state.Push ( value.mX );
	state.Push ( value.mY );
	state.Push ( value.mZ );
	
	state.Push ( MOAILuaIndex ( span.mKeyID ));
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	setKey
	@text	Initialize a key frame at a given time with a give value
			(as Euler angles). Also set the transition type between
			the specified key frame and the next key frame.
	
	@in		MOAIAnimCurveQuat self
	@in		number index			Index of the keyframe.
	@in		number time				Location of the key frame along the curve.
	@in		number xRot				X rotation at time.
	@in		number yRot				Y rotation at time.
	@in		number zRot				Z rotation at time.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@opt	number weight			Blends between chosen ease type (of any) and a linear transition. Defaults to 1.
	@out	nil
*/
int MOAIAnimCurveQuat::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveQuat, "UNNNN" );

	ZLIndex index	= state.GetValue < MOAILuaIndex >( 2, 0 );
	float time		= state.GetValue < float >( 3, 0.0f );
	ZLVec3D value	= state.GetVec3D < float >( 4 );
	u32 mode		= state.GetValue < u32 >( 7, ZLInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 8, 1.0f );
	
	if ( MOAILogMgr::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, value.mX, value.mY, value.mZ );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveQuat
//================================================================//

//----------------------------------------------------------------//
ZLQuaternion MOAIAnimCurveQuat::GetCurveDelta () const {

	ZLQuaternion delta = ZLQuaternion::IDENT;

	ZLSize size = this->mKeys.Size ();
	if ( size > 1 ) {
		delta = this->mSamples [ size - 1 ];
		delta.Sub ( this->mSamples [ 0 ]);
	}
	return delta;
}

//----------------------------------------------------------------//
ZLQuaternion MOAIAnimCurveQuat::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
ZLQuaternion MOAIAnimCurveQuat::GetValue ( const MOAIAnimKeySpan& span ) const {

	const MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	ZLQuaternion v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
	
		ZLQuaternion v1 = this->mSamples [ span.mKeyID + ( ZLSize )1 ];
		
		v0.Slerp ( v0, v1, ZLInterpolate::Curve ( key.mMode, span.mTime, key.mWeight ));
	}
	
	if ( span.mCycle != 0.0f ) {
		ZLQuaternion curveDelta = this->GetCurveDelta ();
		curveDelta.Scale ( span.mCycle );
		v0.Add ( curveDelta );
	}
	return v0;
}

//----------------------------------------------------------------//
MOAIAnimCurveQuat::MOAIAnimCurveQuat () {
	
	RTTI_SINGLE ( MOAIAnimCurveQuat, MOAIAnimCurve )
	
	this->mValue = ZLQuaternion::IDENT;
}

//----------------------------------------------------------------//
MOAIAnimCurveQuat::~MOAIAnimCurveQuat () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::SetSample ( ZLIndex idx, float x, float y, float z ) {

	if ( this->mKeys.CheckIndex ( idx )) {
		this->mSamples [ idx ] = ZLQuaternion ( x, y, z );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAIAnimCurve_ApplyValueAttrOp ( ZLAttribute& attr, u32 op ) {

	this->mValue = attr.Apply ( this->mValue, op, ZLAttribute::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAIAnimCurve_GetDelta ( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	ZLQuaternion v0 = this->GetValue ( span0 );
	ZLQuaternion v1 = this->GetValue ( span1 );
	
	v1.Sub ( v0 );
	
	attr.SetValue ( v1 );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAIAnimCurve_GetValue ( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const {

	attr.SetValue ( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAIAnimCurve_GetZero ( ZLAttribute& attr ) const {

	attr.SetValue ( ZLQuaternion ( 0.0f, 0.0f, 0.0f, 0.0f ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAIAnimCurve_ReserveSamples ( u32 total ) {

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIAnimCurve, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIAnimCurve, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveQuat::MOAINode_Update () {

	this->mValue = this->GetValue ( this->mTime );
}
