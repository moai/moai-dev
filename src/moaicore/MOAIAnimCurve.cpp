// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAILogMessages.h>
#include <uslscore/USBinarySearch.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getLength
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
/**	@name	getValueAtTime
	@text	Return the interpolated value given a point in time along the curve. This does not change
	        the curve's built in TIME attribute (it simply performs the requisite computation on demand).
	
	@in		MOAIAnimCurve self
	@in		number time
	@out	number interpolated value
*/
int MOAIAnimCurve::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	lua_pushnumber ( state, self->GetFloatValue ( state.GetValue < float >( 2, 0 )));

	return 1;
}

//----------------------------------------------------------------//
/**	@name	reserveKeys
	@text	Reserve key frames.
	
	@in		MOAIAnimCurve self
	@in		number nKeys
	@out	nil
*/
int MOAIAnimCurve::_reserveKeys ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	self->Init ( state.GetValue < u32 >( 2, 0 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setKey
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
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UNNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->Size (), L )) {
		self->SetKey ( index, time, value, mode, weight );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWrapMode
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
bool MOAIAnimCurve::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIAnimCurveAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_TIME:
				this->mTime = attrOp.Apply ( this->mTime, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_VALUE:
				this->mValue = attrOp.Apply ( this->mValue, op, MOAINode::ATTR_READ_WRITE );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
u32 MOAIAnimCurve::FindKeyID ( float time ) const {
	
	MOAIAnimKey key;
	key.mTime = time;
	
	u32 index = USBinarySearchNearest < MOAIAnimKey >( this->Data (), key, this->Size ());
	
	return index;
}

//----------------------------------------------------------------//
bool MOAIAnimCurve::GetBoolValue ( float time ) const {

	float value = this->GetFloatValue ( time );
	return ( value > 0.5f ) ? true : false;
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetFloatDelta ( float t0, float t1 ) const {

	u32 total = this->Size ();
	if ( total < 2 ) return 0.0f;
	if ( t0 == t1 ) return 0.0f;

	float repeat0 = 0.0f;
	float wrapT0 = this->WrapTimeValue ( t0, repeat0 );

	float repeat1 = 0.0f;
	float wrapT1 = this->WrapTimeValue ( t1, repeat1 );

	float length = this->GetLength ();

	float step = t1 - t0;	
	float delta = 0.0f;	
	if ( step > 0.0f ) {
		
		u32 endID = total - 1;
		u32 keyID = this->FindKeyID ( wrapT0 );
		
		bool more = true;
		while ( more ) {
			
			if ( keyID == endID ) {
				keyID = 0;
				wrapT1 -= length;
				wrapT0 -= length;
			}
			
			MOAIAnimKey k0 = ( *this )[ keyID ];
			MOAIAnimKey k1 = ( *this )[ keyID + 1 ];
			
			float v0 = k0.mValue;
			float v1 = k1.mValue;
			
			float span = k1.mTime - k0.mTime;
		
			if ( span == 0.0f ) {
				continue;
			}
			
			float r0 = v0;
			float r1 = v1;
			
			if ( wrapT0 > k0.mTime ) {
				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT0 - k0.mTime ) / span, k0.mWeight );
			}
			
			if ( wrapT1 <= k1.mTime ) {
				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT1 - k0.mTime ) / span, k0.mWeight );
				more = false;
			}
			
			//extra addition for Append mode
			if ( mWrapMode == APPEND ) {
				float valueLength = ( *this )[ endID ].mValue - ( *this )[ 0 ].mValue;
				delta += ( r1 + ( valueLength * repeat1 )) -  ( r0 + ( valueLength * repeat0 )) ;
			}
			else {
				delta += r1 - r0;
			}

			keyID++;
		}
	}
	else {
		
		step = -step;
		
		u32 endID = total - 1;
		u32 keyID = this->FindKeyID ( t0 ) + 1;
		if ( keyID > endID ) {
			keyID = endID;
		}
		
		bool more = true;
		while ( more ) {
			
			if ( keyID == 0 ) {
				keyID = endID;
				wrapT0 += length;
				wrapT1 += length;
			}
			
			MOAIAnimKey k0 = ( *this )[ keyID - 1 ];
			MOAIAnimKey k1 = ( *this )[ keyID ];
			
			float v0 = k0.mValue;
			float v1 = k1.mValue;
			
			float span = k1.mTime - k0.mTime;
		
			if ( span == 0.0f ) {
				continue;
			}
			
			float r0 = v0;
			float r1 = v1;
			
			if ( wrapT0 < k1.mTime ) {
				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT0 - k0.mTime ) / span, k0.mWeight );
			}
			
			if ( wrapT1 >= k0.mTime ) {
				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT1 - k0.mTime ) / span, k0.mWeight );
				more = false;
			}
			
			//extra addition for Append mode
			if ( mWrapMode == APPEND ) {
				float valueLength = ( *this )[ endID ].mValue - ( *this )[ 0 ].mValue;
				delta -= ( r1 + ( valueLength * repeat1 )) -  ( r0 + ( valueLength * repeat0 ));
			}
			else {
				delta -= r1 - r0;
			}
			keyID--;
		}
	}

	
	return delta;
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetFloatValue ( float time ) const {

	u32 total = this->Size ();
	if ( total == 0 ) return 0.0f;
	u32 endID = total - 1;
	
	float repeat = 0.0f;
	float wrapTime = this->WrapTimeValue ( time, repeat );

	u32 keyID = this->FindKeyID ( wrapTime );
	MOAIAnimKey k0 = ( *this )[ keyID ];
	
	float finalValue = 0.0f;

	if ( keyID == endID ) {
		finalValue = k0.mValue;
	}
	else if ( k0.mMode == USInterpolate::kFlat ) {
		finalValue = k0.mValue;
	}
	else if ( k0.mTime == wrapTime ) {
		finalValue = k0.mValue;
	}
	else {

		MOAIAnimKey k1 = ( *this )[ keyID + 1 ];

		float v0 = k0.mValue;
		float v1 = k1.mValue;

		if ( v0 == v1 ) {
			finalValue = v0;
		}
		else {
			float span = k1.mTime - k0.mTime;
			
			if ( span == 0.0f ) {
				finalValue = v0;
			}
			else {
				float t = ( wrapTime - k0.mTime ) / span;
				finalValue = USInterpolate::Interpolate ( k0.mMode, v0, v1, t, k0.mWeight );
			}
		}
	}

	//extra addition for Append mode
	if ( mWrapMode == APPEND ) {
		float valueLength = ( *this )[ endID ].mValue - ( *this )[ 0 ].mValue;
		return ( valueLength * repeat ) + finalValue;
	}
	else {
		return finalValue;
	}
}

//----------------------------------------------------------------//
u32 MOAIAnimCurve::GetIndexValue ( float time ) const {

	float value = this->GetFloatValue ( time );
	return ( value < 0.0f ) ? 0 : ( u32 )value;
}

//----------------------------------------------------------------//
int MOAIAnimCurve::GetIntValue ( float time ) const {

	float value = this->GetFloatValue ( time );
	return ( int ) value;
}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetLength () const {

	u32 total = this->Size ();
	if ( total == 0 ) return 0.0f;
	return ( *this )[ total - 1 ].mTime - ( *this )[ 0 ].mTime;
}

//----------------------------------------------------------------//
MOAIAnimCurve::MOAIAnimCurve () :
	mTime ( 0.0f ),
	mValue ( 0.0f ),
	mWrapMode ( CLAMP ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIAnimCurve::~MOAIAnimCurve () {
}

//----------------------------------------------------------------//
void MOAIAnimCurve::OnDepNodeUpdate () {

	this->mValue = this->GetFloatValue ( this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "ATTR_TIME", MOAIAnimCurveAttr::Pack ( ATTR_TIME ));
	state.SetField ( -1, "ATTR_VALUE", MOAIAnimCurveAttr::Pack ( ATTR_VALUE ));

	state.SetField ( -1, "CLAMP", ( u32 ) CLAMP );
	state.SetField ( -1, "WRAP", ( u32 ) WRAP );
	state.SetField ( -1, "MIRROR", ( u32 ) MIRROR );
	state.SetField ( -1, "APPEND", ( u32 ) APPEND );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLength",		_getLength },
		{ "getValueAtTime", _getValueAtTime },
		{ "reserveKeys",	_reserveKeys },
		{ "setKey",			_setKey },
		{ "setWrapMode",	_setWrapMode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::SetKey ( u32 id, float time, float value, u32 mode, float weight ) {

	if ( id < this->Size ()) {
		( *this )[ id ].mTime = time;
		( *this )[ id ].mValue = value;
		( *this )[ id ].mMode = mode;
		( *this )[ id ].mWeight = weight;
	}
}

//----------------------------------------------------------------//
float MOAIAnimCurve::WrapTimeValue ( float t, float &repeat ) const {

	float startTime = ( *this )[ 0 ].mTime;
	float length = GetLength ();

	float time = ( t - startTime ) / length;
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
