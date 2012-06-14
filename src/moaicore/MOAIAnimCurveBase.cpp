// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurveBase.h>
#include <moaicore/MOAILogMessages.h>
#include <uslscore/USBinarySearch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getLength
	@text	Return the largest key frame time value in the curve.
	
	@in		MOAIAnimCurveBase self
	@out	number length
*/
int MOAIAnimCurveBase::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveBase, "U" );

	lua_pushnumber ( state, self->GetLength ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	reserveKeys
	@text	Reserve key frames.
	
	@in		MOAIAnimCurveBase self
	@in		number nKeys
	@out	nil
*/
int MOAIAnimCurveBase::_reserveKeys ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveBase, "UN" );

	u32 total	= state.GetValue < u32 >( 2, 0 );

	self->ReserveKeys ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWrapMode
	@text	Sets the wrap mode for values above 1.0 and below 0.0.
			CLAMP sets all values above and below 1.0 and 0.0 to
			values at 1.0 and 0.0 respectively
	        
	
	@in		MOAIAnimCurveBase self
	@opt	number mode			One of MOAIAnimCurveBase.CLAMP, MOAIAnimCurveBase.WRAP, MOAIAnimCurveBase.MIRROR,
								MOAIAnimCurveBase.APPEND. Default value is MOAIAnimCurveBase.CLAMP.

	@out	nil
*/
int	MOAIAnimCurveBase::_setWrapMode	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveBase, "U" );

	u32 mode = state.GetValue < u32 >( 2, CLAMP );

	self->mWrapMode = mode;
	
	return 0;
}

//================================================================//
// MOAIAnimCurveBase
//================================================================//

//----------------------------------------------------------------//
bool MOAIAnimCurveBase::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIAnimCurveBaseAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_TIME:
				this->mTime = attrOp.Apply ( this->mTime, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_VALUE:
				this->ApplyValueAttrOp ( attrOp, op );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::Clear () {

	this->mKeys.Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::Draw ( u32 resolution ) const {
	UNUSED ( resolution );
}

//----------------------------------------------------------------//
u32 MOAIAnimCurveBase::FindKeyID ( float time ) const {
	
	MOAIAnimKey key;
	key.mTime = time;
	
	u32 index = USBinarySearchNearest < MOAIAnimKey >( this->mKeys.Data (), key, this->mKeys.Size ());
	
	return index;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::GetDelta ( MOAIAttrOp& attrOp, float t0, float t1 ) {
	
	if (( t0 == t1 ) || ( this->mKeys.Size () < 2 )) {
		this->GetZero ( attrOp );
	}
	else {

		MOAIAnimKeySpan s0 = this->GetSpan ( t0 );
		MOAIAnimKeySpan s1 = this->GetSpan ( t1 );
		
		this->GetDelta ( attrOp, s0, s1 );
	}
}

//----------------------------------------------------------------//
const MOAIAnimKey& MOAIAnimCurveBase::GetKey ( u32 id ) const {

	assert ( id < this->mKeys.Size ());
	return this->mKeys [ id ];
}

//----------------------------------------------------------------//
float MOAIAnimCurveBase::GetLength () const {

	u32 total = this->mKeys.Size ();
	if ( total == 0 ) return 0.0f;
	return this->mKeys [ total - 1 ].mTime - this->mKeys [ 0 ].mTime;
}

//----------------------------------------------------------------//
MOAIAnimKeySpan MOAIAnimCurveBase::GetSpan ( float time ) const {

	MOAIAnimKeySpan span;
	span.mKeyID = NULL_KEY_ID;
	span.mTime = 0.0f;
	span.mCycle = 0.0f;

	u32 total = this->mKeys.Size ();
	u32 endID = total - 1;
	assert ( total );
	
	float wrapTime = this->WrapTime ( time, span.mCycle );
	span.mKeyID = this->FindKeyID ( wrapTime );

	if ( span.mKeyID == endID ) {
		return span;
	}
	
	MOAIAnimKey k0 = this->mKeys [ span.mKeyID ];
	
	if ( k0.mMode == USInterpolate::kFlat ) {
		return span;
	}
	
	if ( k0.mTime == wrapTime ) {
		return span;
	}
	
	MOAIAnimKey k1 = this->mKeys [ span.mKeyID + 1 ];
	
	if ( k1.mTime > k0.mTime ) {
		span.mTime = ( wrapTime - k0.mTime ) / ( k1.mTime - k0.mTime );
	}
	return span;
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::GetValue ( MOAIAttrOp& attrOp, float time ) {
	
	MOAIAnimKeySpan span = this->GetSpan ( time );
	this->GetValue ( attrOp, span );
}

//----------------------------------------------------------------//
MOAIAnimCurveBase::MOAIAnimCurveBase () :
	mTime ( 0.0f ),
	mWrapMode ( CLAMP ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIAnimCurveBase::~MOAIAnimCurveBase () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );

	state.SetField ( -1, "ATTR_TIME", MOAIAnimCurveBaseAttr::Pack ( ATTR_TIME ));
	state.SetField ( -1, "ATTR_VALUE", MOAIAnimCurveBaseAttr::Pack ( ATTR_VALUE ));

	state.SetField ( -1, "CLAMP", ( u32 )CLAMP );
	state.SetField ( -1, "WRAP", ( u32 )WRAP );
	state.SetField ( -1, "MIRROR", ( u32 )MIRROR );
	state.SetField ( -1, "APPEND", ( u32 )APPEND );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLength",			_getLength },
		{ "reserveKeys",		_reserveKeys },
		{ "setWrapMode",		_setWrapMode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::ReserveKeys ( u32 total ) {

	this->mKeys.Init ( total );
	this->ReserveSamples ( total );
}

//----------------------------------------------------------------//
void MOAIAnimCurveBase::SetKey ( u32 id, float time, u32 mode, float weight ) {

	if ( id < this->mKeys.Size ()) {
		this->mKeys [ id ].mTime = time;
		this->mKeys [ id ].mMode = mode;
		this->mKeys [ id ].mWeight = weight;
	}
}

//----------------------------------------------------------------//
u32 MOAIAnimCurveBase::Size () const {

	return this->mKeys.Size ();
}

//----------------------------------------------------------------//
// return the time wrapped to fall within the time span defined by the curve
// in APPEND mode, 'repeat' gives the cycle (how many complete spans have elapsed)
float MOAIAnimCurveBase::WrapTime ( float t, float &repeat ) const {

	float startTime = this->mKeys [ 0 ].mTime;
	float length = GetLength ();

	float time = ( t - startTime ) / length; // normalize time
	float wrappedT = 0.0f;
	repeat = 0.0f;

	switch ( mWrapMode ) {
		case CLAMP: {
			wrappedT = USFloat::Clamp ( time, 0.0f, 1.0f );
		}
		break;

		case WRAP: {
			wrappedT = time - USFloat::Floor ( time );
		}
		break;

		case MIRROR: {
			u32 tFloor = ( u32 ) USFloat::Floor ( time );
			if ( tFloor % 2 ) {
				wrappedT = 1.0f - ( time - tFloor );
			}
			else {
				wrappedT = ( time - tFloor );
			}
		}
		break;

		case APPEND: {
			wrappedT = time - USFloat::Floor ( time );
			repeat = USFloat::Floor ( time );
		}
		break;
	}

	wrappedT = wrappedT * length + startTime;
	
	if ( wrappedT + EPSILON > t && wrappedT - EPSILON < t ) { 
		wrappedT = t; 
	}

	return wrappedT;
}
