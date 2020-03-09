// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurve.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getLength
	@text	Return the largest key frame time value in the curve.
	
	@in		MOAIAnimCurve self
	@out	number length
*/
int MOAIAnimCurve::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "U" );

	lua_pushnumber ( state, self->GetLength ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveKeys
	@text	Reserve key frames.
	
	@in		MOAIAnimCurve self
	@in		number nKeys
	@out	nil
*/
int MOAIAnimCurve::_reserveKeys ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	u32 total	= state.GetValue < u32 >( 2, 0 );

	self->ReserveKeys ( total );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurve::_setTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	self->mTime = state.GetValue < float >( 2, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setWrapMode
	@text	Sets the wrap mode for values above 1.0 and below 0.0.
			CLAMP sets all values above and below 1.0 and 0.0 to
			values at 1.0 and 0.0 respectively
	        
	
	@in		MOAIAnimCurve self
	@opt	number mode			One of MOAIAnimCurve.CLAMP, MOAIAnimCurve.WRAP, MOAIAnimCurve.MIRROR,
								MOAIAnimCurve.APPEND. Default value is MOAIAnimCurve.CLAMP.

	@out	nil
*/
int	MOAIAnimCurve::_setWrapMode	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "U" );

	u32 mode = state.GetValue < u32 >( 2, CLAMP );

	self->mWrapMode = mode;
	
	return 0;
}

//================================================================//
// MOAIAnimCurve
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurve::Clear () {

	this->mKeys.Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimCurve::Draw ( u32 resolution ) const {
	this->MOAIAnimCurve_Draw ( resolution );
}

//----------------------------------------------------------------//
ZLIndex MOAIAnimCurve::FindKeyID ( float time ) const {
	
	MOAIAnimKey key;
	key.mTime = time;
	
	return ZLBinarySearchNearest < MOAIAnimKey >( this->mKeys.GetBuffer (), key, ( u32 )this->mKeys.Size ());
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetDelta ( ZLAttribute& attr, float t0, float t1 ) {
	
	if (( t0 == t1 ) || ( this->mKeys.Size () < 2 )) {
		this->MOAIAnimCurve_GetZero ( attr );
	}
	else {

		MOAIAnimKeySpan s0 = this->GetSpan ( t0 );
		MOAIAnimKeySpan s1 = this->GetSpan ( t1 );
		
		this->MOAIAnimCurve_GetDelta ( attr, s0, s1 );
	}
}

//----------------------------------------------------------------//
const MOAIAnimKey& MOAIAnimCurve::GetKey ( ZLIndex idx ) const {

	assert ( this->mKeys.CheckIndex ( idx ));
	return this->mKeys [ idx ];
}

//----------------------------------------------------------------//
ZLReal MOAIAnimCurve::GetLength () const {

	ZLSize total = this->mKeys.Size ();
	if ( total == 0 ) return 0.0f;
	return this->mKeys [ total - 1 ].mTime - this->mKeys [ 0 ].mTime;
}

//----------------------------------------------------------------//
MOAIAnimKeySpan MOAIAnimCurve::GetSpan ( float time ) const {

	MOAIAnimKeySpan span;
	span.mKeyID = ZLIndexOp::INVALID;
	span.mTime = 0.0f;
	span.mCycle = 0.0f;

	u32 total = ( u32 )this->mKeys.Size ();
	u32 endID = total - 1;
	assert ( total );
	
	float wrapTime = this->WrapTime ( time, span.mCycle );
	span.mKeyID = this->FindKeyID ( wrapTime );

	if ( span.mKeyID == endID ) {
		return span;
	}
	
	MOAIAnimKey k0 = this->mKeys [ span.mKeyID ];
	
	if ( k0.mMode == ZLInterpolate::kFlat ) {
		return span;
	}
	
	if ( k0.mTime == wrapTime ) {
		return span;
	}
	
	MOAIAnimKey k1 = this->mKeys [ span.mKeyID + ( ZLSize )1 ];
	
	if ( k1.mTime > k0.mTime ) {
		span.mTime = ( wrapTime - k0.mTime ) / ( k1.mTime - k0.mTime );
	}
	return span;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetValue ( ZLAttribute& attr, float time ) {
	
	MOAIAnimKeySpan span = this->GetSpan ( time );
	this->MOAIAnimCurve_GetValue ( attr, span );
}

//----------------------------------------------------------------//
MOAIAnimCurve::MOAIAnimCurve () :
	mTime ( 0.0f ),
	mWrapMode ( CLAMP ) {
	
	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIAnimCurve, MOAINode )
}

//----------------------------------------------------------------//
MOAIAnimCurve::~MOAIAnimCurve () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimCurve::ReserveKeys ( u32 total ) {

	this->mKeys.Init ( total );
	this->MOAIAnimCurve_ReserveSamples ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::SetKey ( ZLIndex idx, float time, u32 mode, float weight ) {

	if ( this->mKeys.CheckIndex ( idx )) {
		this->mKeys [ idx ].mTime = time;
		this->mKeys [ idx ].mMode = mode;
		this->mKeys [ idx ].mWeight = weight;
	}
}

//----------------------------------------------------------------//
u32 MOAIAnimCurve::Size () const {

	return ( u32 )this->mKeys.Size ();
}

//----------------------------------------------------------------//
// return the time wrapped to fall within the time span defined by the curve
// in APPEND mode, 'repeat' gives the cycle (how many complete spans have elapsed)
ZLReal MOAIAnimCurve::WrapTime ( ZLReal t, ZLReal &repeat ) const {

	ZLReal startTime = this->mKeys [ 0 ].mTime;
	ZLReal length = GetLength ();

	ZLReal time = ( t - startTime ) / length; // normalize time
	ZLReal wrappedT = 0.0f;
	repeat = 0.0f;

	switch ( mWrapMode ) {
		case CLAMP: {
			wrappedT = ZLFloat::Clamp ( time, 0.0, 1.0 );
		}
		break;

		case WRAP: {
			wrappedT = time - ZLFloat::Floor ( time );
		}
		break;

		case MIRROR: {
			u32 tFloor = ( u32 )ZLFloat::Floor ( time );
			if ( tFloor % 2 ) {
				wrappedT = 1.0f - ( time - tFloor );
			}
			else {
				wrappedT = ( time - tFloor );
			}
		}
		break;

		case APPEND: {
			wrappedT = time - ZLFloat::Floor ( time );
			repeat = ZLFloat::Floor ( time );
		}
		break;
	}

	float result = wrappedT * length + startTime;
	
	if( wrappedT != 0.0f && wrappedT != 1.0f ) {
		if ( result + EPSILON > t && result - EPSILON < t ) { 
			result = t; 
		}
	}

	return result;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurve::MOAIAnimCurve_Draw ( u32 resolution ) const {

	// TODO: this isn't entirely correct. the value of each key frame should be drawn
	// and then the spans between keys should be filled in with an approximation of
	// the resolution.
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLReal length = this->GetLength ();
	ZLReal step = length / ( ZLReal )resolution;
	
	gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_STRIP, resolution );
	
	for ( ZLSize i = 0; i < resolution; ++i ) {
		
		ZLReal t = step * ( ZLReal )i;
		ZLReal v = this->MOAIAnimCurve_GetFloatForTime ( t );
		
		gfxMgr.WriteVtx ( t, v, 0.0 );
		gfxMgr.WritePenColor4b ();
	}
	
	ZLReal t = length;
	ZLReal v = this->MOAIAnimCurve_GetFloatForTime ( t );
	
	gfxMgr.WriteVtx ( t, v, 0.0 );
	gfxMgr.WritePenColor4b ();
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
ZLReal MOAIAnimCurve::MOAIAnimCurve_GetFloatForTime ( ZLReal t ) const {

	return 0.0;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaClass ( composer, state ));

	state.SetField ( -1, "ATTR_TIME",	AttrID::Pack ( ATTR_TIME ).ToRaw ());
	state.SetField ( -1, "ATTR_VALUE", 	AttrID::Pack ( ATTR_VALUE ).ToRaw ());

	state.SetField ( -1, "CLAMP", ( u32 )CLAMP );
	state.SetField ( -1, "WRAP", ( u32 )WRAP );
	state.SetField ( -1, "MIRROR", ( u32 )MIRROR );
	state.SetField ( -1, "APPEND", ( u32 )APPEND );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getLength",			_getLength },
		{ "reserveKeys",		_reserveKeys },
		{ "setTime",			_setTime },
		{ "setWrapMode",		_setWrapMode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAnimCurve::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {

		switch ( attrID.Unpack ()) {
			case ATTR_TIME:
				this->mTime = attr.Apply ( this->mTime, op, ZLAttribute::ATTR_READ_WRITE );
				return true;
			case ATTR_VALUE:
				this->MOAIAnimCurve_ApplyValueAttrOp ( attr, op );
				return true;
		}
	}
	return false;
}
