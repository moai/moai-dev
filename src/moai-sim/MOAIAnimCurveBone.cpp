// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	
	if ( MOAILogMgr::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {

		ZLBone bone = ZLBone::IDENT;
		bone.SetLocation ( position );
		bone.SetQuaternion ( rotation );
		bone.SetScale ( scale );
		
		self->SetKey ( index, time, mode, weight );
		self->SetSample ( index, bone );
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
void MOAIAnimCurveBone::GetCurveDelta ( ZLBone& bone ) const {

	u32 size = ( u32 )this->mKeys.Size ();
	if ( size > 1 ) {
	
		bone = this->mSamples [ size - 1 ];
		bone.Sub ( this->mSamples [ 0 ]);
	}
	else {
		bone.Ident ();
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetDelta ( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const {

	ZLBone b0;
	this->GetValue ( span0, b0 );
	
	ZLBone b1;
	this->GetValue ( span1, b1 );

	b1.Sub ( b0 );
	
	ZLAffine3D mtx;
	b1.Compose ( mtx );
	attr.SetValue ( mtx );
}

//----------------------------------------------------------------//
ZLAffine3D MOAIAnimCurveBone::GetValue ( float time ) const {

	MOAIAnimKeySpan span = this->GetSpan ( time );
	return this->GetValue ( span );
}

//----------------------------------------------------------------//
ZLAffine3D MOAIAnimCurveBone::GetValue ( const MOAIAnimKeySpan& span ) const {
	
	ZLBone bone;
	this->GetValue ( span, bone );
	
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
	
	ZLAffine3D mtx;
	bone.Compose ( mtx );
	return mtx;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::GetValue ( const MOAIAnimKeySpan& span, ZLBone& bone ) const {

	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	
	const ZLBone& b0			= this->mSamples [ span.mKeyID ];

	if ( span.mTime > 0.0f ) {
		const ZLBone& b1	= this->mSamples [ span.mKeyID + 1 ];
		bone.Interpolate ( b0, b1, ZLInterpolate::Curve ( key.mMode, span.mTime, key.mWeight ));
	}
	else {
		bone = b0;
	}
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

	this->mSamples.Init ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::SetSample ( u32 idx, const ZLBone& bone ) {

	if ( idx < this->mKeys.Size ()) {
		this->mSamples [ idx ] = bone;
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurveBone::MOAIAnimCurveBase_Accumulate ( MOAIAnimChannel& channel, float time, float opacity ) {
	UNUSED ( channel );
	UNUSED ( time );
	UNUSED ( opacity );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBone::MOAINode_Update () {

	this->mValue = this->GetValue ( this->mTime );
}
