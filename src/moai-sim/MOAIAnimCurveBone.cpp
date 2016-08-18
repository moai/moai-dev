// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurveBone.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveBone::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveBone, "UN" );

	float time = state.GetValue < float >( 2, 0 );
	
	MOAIAnimKeySpan span = self->GetSpan ( time );
	ZLAffine3D mtx = self->GetValue ( span );

	state.Push ( mtx );
	
	return 12;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurveBone::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveBone, "UNNNN" );

	u32 index				= state.GetValue < u32 >( 2, 1 ) - 1;
	float time				= state.GetValue < float >( 3, 0.0f );
	ZLVec3D position		= state.GetValue < ZLVec3D >( 4, ZLVec3D::ORIGIN );
	ZLQuaternion rotation	= state.GetValue < ZLQuaternion >( 7, ZLQuaternion::IDENT );
	ZLVec3D scale			= state.GetValue < ZLVec3D >( 11, ZLVec3D::AXIS );
	u32 mode				= state.GetValue < u32 >( 14, ZLInterpolate::kSmooth );
	float weight			= state.GetValue < float >( 15, 1.0f );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		self->SetKey ( index, time, mode, weight );
		
		self->SetSamplePosition ( index, position.mX, position.mY, position.mZ );
		self->SetSampleRotation ( index, rotation.mV.mX, rotation.mV.mY, rotation.mV.mZ, rotation.mS );
		self->SetSampleScale ( index, scale.mX, scale.mY, scale.mZ );
	}
	return 0;
}

//================================================================//
// MOAIAnimCurveBone
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveBone::ApplyValueAttrOp ( MOAIAttribute& attr, u32 op ) {

	this->mValue = attr.ApplyNoAdd ( this->mValue, op, MOAIAttribute::ATTR_READ_WRITE );
}

//----------------------------------------------------------------//
ZLAffine3D MOAIAnimCurveBone::Compose ( const ZLVec3D& pos, const ZLQuaternion& rot, const ZLVec3D& scl ) {

	ZLAffine3D value;
	value.Scale ( scl );
	
	ZLAffine3D mtx;
	rot.Get ( mtx );
	value.Append ( mtx );
	
	mtx.Translate ( pos );
	value.Append ( mtx );
	
	return value;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetCurveDelta ( ZLVec3D& pos, ZLQuaternion& rot, ZLVec3D& scl ) const {

	u32 size = ( u32 )this->mKeys.Size ();
	if ( size > 1 ) {
	
		pos = this->mPositionSamples [ size - 1 ];
		pos.Sub ( this->mPositionSamples [ 0 ]);
	
		rot = this->mRotationSamples [ size - 1 ];
		rot.Sub ( this->mRotationSamples [ 0 ]);
		
		scl = this->mScaleSamples [ size - 1 ];
		scl.Sub ( this->mScaleSamples [ 0 ]);
	}
	else {
		pos = ZLVec3D::ORIGIN;
		rot = ZLQuaternion::IDENT;
		scl = ZLVec3D::AXIS;
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetDelta ( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	ZLVec3D p0;
	ZLQuaternion q0;
	ZLVec3D s0;
	
	this->GetValue ( span0, p0, q0, s0 );
	
	ZLVec3D p1;
	ZLQuaternion q1;
	ZLVec3D s1;
	
	this->GetValue ( span1, p1, q1, s1 );

	p1.Sub ( p0 );
	q1.Sub ( q0 );
	s1.Sub ( s0 );
	
	attr.SetValue ( MOAIAnimCurveBone::Compose ( p1, q1, s1 ));
}

//----------------------------------------------------------------//
ZLAffine3D MOAIAnimCurveBone::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
ZLAffine3D MOAIAnimCurveBone::GetValue ( const MOAIAnimKeySpan& span ) const {
	
	ZLVec3D p0;
	ZLQuaternion q0;
	ZLVec3D s0;
		
	this->GetValue ( span, p0, q0, s0 );
	
	// TODO: test this
//	if ( span.mCycle != 0.0f ) {
//	
//		ZLVec3D p1;
//		ZLQuaternion q1;
//		ZLVec3D s1;
//	
//		this->GetCurveDelta ( p1, q1, s1 );
//		
//		p1.Scale ( span.mCycle );
//		q1.Scale ( span.mCycle );
//		s1.Scale ( span.mCycle );
//		
//		p0.Add ( p1 );
//		q0.Add ( q1 );
//		s0.Add ( s1 );
//	}
	
	return MOAIAnimCurveBone::Compose ( p0, q0, s0 );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetValue ( const MOAIAnimKeySpan& span, ZLVec3D& pos, ZLQuaternion& rot, ZLVec3D& scl ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	
	ZLVec3D p0			= this->mPositionSamples [ span.mKeyID ];
	ZLQuaternion q0		= this->mRotationSamples [ span.mKeyID ];
	ZLVec3D s0			= this->mScaleSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
	
		ZLVec3D p1			= this->mPositionSamples [ span.mKeyID + 1 ];
		ZLQuaternion q1		= this->mRotationSamples [ span.mKeyID + 1 ];
		ZLVec3D s1			= this->mScaleSamples [ span.mKeyID + 1 ];
		
		p0.Lerp ( p1, ZLInterpolate::Curve ( key.mMode, span.mTime, key.mWeight ));
		q0.Slerp ( q0, q1, ZLInterpolate::Curve ( key.mMode, span.mTime, key.mWeight ));
		s0.Lerp ( s1, ZLInterpolate::Curve ( key.mMode, span.mTime, key.mWeight ));
	}
		
	pos = p0;
	rot = q0;
	scl = s0;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetValue ( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const {

	attr.SetValue ( this->GetValue ( span ));
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetZero ( MOAIAttribute& attr ) const {

	attr.SetValue ( ZLQuaternion ( 0.0f, 0.0f, 0.0f, 0.0f ));
}

//----------------------------------------------------------------//
MOAIAnimCurveBone::MOAIAnimCurveBone () {
	
	RTTI_SINGLE ( MOAIAnimCurveBase )
	
	this->mValue.Ident ();
}

//----------------------------------------------------------------//
MOAIAnimCurveBone::~MOAIAnimCurveBone () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::OnDepNodeUpdate () {

	this->mValue = this->GetValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAnimCurveBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getValueAtTime",		_getValueAtTime },
		{ "setKey",				_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::ReserveSamples ( u32 total ) {

	this->mPositionSamples.Init ( total );
	this->mRotationSamples.Init ( total );
	this->mScaleSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::SetSamplePosition ( u32 idx, float x, float y, float z ) {

	if ( idx < this->mKeys.Size ()) {
		this->mPositionSamples [ idx ] = ZLVec3D ( x, y, z );
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::SetSampleRotation ( u32 idx, float x, float y, float z, float w ) {

	if ( idx < this->mKeys.Size ()) {
		this->mRotationSamples [ idx ] = ZLQuaternion ( w, x, y, z );
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::SetSampleScale ( u32 idx, float x, float y, float z ) {

	if ( idx < this->mKeys.Size ()) {
		this->mScaleSamples [ idx ] = ZLVec3D ( x, y, z );
	}
}
