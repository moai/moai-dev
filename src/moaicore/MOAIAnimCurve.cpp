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

	float time = state.GetValue < float >( 2, 0 );

	MOAIAnimSample sample;
	self->GetSampleForTime ( sample, time );

	return sample.Push ( state );
}

//----------------------------------------------------------------//
/**	@name	reserveKeys
	@text	Reserve key frames.
	
	@in		MOAIAnimCurve self
	@in		number nKeys
	@opt	number type			One of MOAIAnimCurve.TYPE_BOOL, MOAIAnimCurve.TYPE_FLOAT, MOAIAnimCurve.TYPE_INDEX,
								MOAIAnimCurve.TYPE_INT, MOAIAnimCurve.TYPE_QUATERNION, MOAIAnimCurve.TYPE_TRANSFORM_2D,
								MOAIAnimCurve.TYPE_TRANSFORM_3D, MOAIAnimCurve.TYPE_VEC2, MOAIAnimCurve.TYPE_VEC3.
								Default value is MOAIAnimCurve.TYPE_FLOAT.
	@out	nil
*/
int MOAIAnimCurve::_reserveKeys ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );

	u32 total	= state.GetValue < u32 >( 2, 0 );
	u32 type	= state.GetValue < u32 >( 3, MOAIAnimSample::TYPE_FLOAT );

	self->Init ( total, type );

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
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
	
		self->SetKey ( index, time, mode, weight );
		
		if ( self->mSampleType == MOAIAnimSample::TYPE_FLOAT ) {
		
			MOAIAnimSample sample;
			sample.Set ( value );
		
			self->SetSample ( index, sample );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAnimCurve::_setSample ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UN" );
	
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( MOAILogMessages::CheckIndexPlusOne ( index, self->mKeys.Size (), L )) {
		
		MOAIAnimSample sample;
		sample.Set ( state, 3, self->mSampleType );
		
		self->SetSample ( index, sample );
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
				this->mValue.GetAttrOp ( attrOp );
				attrOp.Apply ( op, MOAINode::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::Clear () {

	this->mKeys.Clear ();
	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurve::Draw ( u32 resolution ) const {
	UNUSED ( resolution );

	// TODO: MOAIAnimCurve

	// TODO: this isn't entirely correct. the value of each key frame should be drawn
	// and then the spans between keys should be filled in with an approximation of
	// the resolution.

	//if ( !curve ) return;
	//
	//MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	//
	//float length = curve.GetLength ();
	//float step = length / ( float )resolution;
	//
	//gfxDevice.BeginPrim ( GL_LINE_STRIP );
	//
	//for ( u32 i = 0; i < resolution; ++i ) {
	//	
	//	float t = step * ( float )i;
	//	float v = curve.GetFloatValue ( t );
	//	
	//	gfxDevice.WriteVtx ( t, v, 0.0f );
	//	gfxDevice.WriteFinalColor4b ();
	//}
	//
	//float t = length;
	//float v = curve.GetFloatValue ( t );
	//
	//gfxDevice.WriteVtx ( t, v, 0.0f );
	//gfxDevice.WriteFinalColor4b ();
	//
	//gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
u32 MOAIAnimCurve::FindKeyID ( float time ) const {
	
	MOAIAnimKey key;
	key.mTime = time;
	
	u32 index = USBinarySearchNearest < MOAIAnimKey >( this->mKeys.Data (), key, this->mKeys.Size ());
	
	return index;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetCurveDelta ( MOAIAnimSample& sample ) {

	MOAIAnimSample v0;
	MOAIAnimSample v1;
	
	this->GetSampleForKey ( v0, 0 );
	this->GetSampleForKey ( v1, this->mKeys.Size () - 1 );
	
	sample.Sub ( v1, v0 );	
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetDelta ( MOAIAttrOp& attrOp, float t0, float t1 ) {
	UNUSED ( attrOp );
	UNUSED ( t0 );
	UNUSED ( t1 );
}

//----------------------------------------------------------------//
//float MOAIAnimCurve::GetFloatDelta ( float t0, float t1 ) const {
//
//	return 0.0f;
//
//	// total number of keys
//	u32 totalKeys = this->mKeys.Size ();
//	
//	if ( totalKeys < 2 ) return 0.0f; // curve with < 2 keyframes cannot produce a delta
//	if ( t0 == t1 ) return 0.0f; // time is same, so no delta
//
//	// 'delta' will hold the result
//	float delta = 0.0f;	
//
//	// size of the current step
//	float step = t1 - t0;
//
//	// get the wrapped value and cycle count for t0 and t1
//	float repeat0 = 0.0f;
//	float wrapT0 = this->WrapTimeValue ( t0, repeat0 );
//
//	float repeat1 = 0.0f;
//	float wrapT1 = this->WrapTimeValue ( t1, repeat1 );
//
//	// total length of the curve
//	float curveLength = this->GetLength ();
//
//	if ( step > 0.0f ) {
//		
//		// id of last key
//		u32 endID = totalKeys - 1;
//		
//		// id of key defining span containing wrapped t0
//		u32 keyID = this->FindKeyID ( wrapT0 );
//		
//		bool more = true;
//		while ( more ) {
//			
//			if ( keyID == endID ) {
//				keyID = 0;
//				wrapT1 -= curveLength;
//				wrapT0 -= curveLength;
//			}
//			
//			const MOAIAnimKey& k0 = this->mKeys.[ keyID ];
//			const MOAIAnimKey& k1 = this->mKeys.[ keyID + 1 ];
//			
//			float v0 = k0.mValue;
//			float v1 = k1.mValue;
//			
//			float span = k1.mTime - k0.mTime;
//		
//			if ( span == 0.0f ) {
//				continue;
//			}
//			
//			float r0 = v0;
//			float r1 = v1;
//			
//			if ( wrapT0 > k0.mTime ) {
//				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT0 - k0.mTime ) / span, k0.mWeight );
//			}
//			
//			if ( wrapT1 <= k1.mTime ) {
//				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT1 - k0.mTime ) / span, k0.mWeight );
//				more = false;
//			}
//			
//			//extra addition for Append mode
//			if ( mWrapMode == APPEND ) {
//				float valueLength = ( *this )[ endID ].mValue - ( *this )[ 0 ].mValue;
//				delta += ( r1 + ( valueLength * repeat1 )) - ( r0 + ( valueLength * repeat0 )) ;
//			}
//			else {
//				delta += r1 - r0;
//			}
//
//			keyID++;
//		}
//	}
//	else {
//		
//		step = -step;
//		
//		u32 endID = totalKeys - 1;
//		u32 keyID = this->FindKeyID ( t0 ) + 1;
//		if ( keyID > endID ) {
//			keyID = endID;
//		}
//		
//		bool more = true;
//		while ( more ) {
//			
//			if ( keyID == 0 ) {
//				keyID = endID;
//				wrapT0 += curveLength;
//				wrapT1 += curveLength;
//			}
//			
//			MOAIAnimKey k0 = ( *this )[ keyID - 1 ];
//			MOAIAnimKey k1 = ( *this )[ keyID ];
//			
//			float v0 = k0.mValue;
//			float v1 = k1.mValue;
//			
//			float span = k1.mTime - k0.mTime;
//		
//			if ( span == 0.0f ) {
//				continue;
//			}
//			
//			float r0 = v0;
//			float r1 = v1;
//			
//			if ( wrapT0 < k1.mTime ) {
//				r1 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT0 - k0.mTime ) / span, k0.mWeight );
//			}
//			
//			if ( wrapT1 >= k0.mTime ) {
//				r0 = USInterpolate::Interpolate ( k0.mMode, v0, v1, ( wrapT1 - k0.mTime ) / span, k0.mWeight );
//				more = false;
//			}
//			
//			//extra addition for Append mode
//			if ( mWrapMode == APPEND ) {
//				float valueLength = ( *this )[ endID ].mValue - ( *this )[ 0 ].mValue;
//				delta -= ( r1 + ( valueLength * repeat1 )) -  ( r0 + ( valueLength * repeat0 ));
//			}
//			else {
//				delta -= r1 - r0;
//			}
//			keyID--;
//		}
//	}
//
//	return delta;
//}

//----------------------------------------------------------------//
float MOAIAnimCurve::GetLength () const {

	u32 total = this->mKeys.Size ();
	if ( total == 0 ) return 0.0f;
	return this->mKeys [ total - 1 ].mTime - this->mKeys [ 0 ].mTime;
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetSampleForKey ( MOAIAnimSample& sample, u32 keyID ) {

	void* buffer = this->GetBufferForKey ( keyID );
	sample.Set ( buffer, this->mSampleType );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetSampleForTime ( MOAIAnimSample& sample, float time ) {

	u32 total = this->mKeys.Size ();
	if ( total == 0 ) {
		sample.Set ( this->mSampleType );
	}
	u32 endID = total - 1;
	
	float repeat = 0.0f;
	float wrapTime = this->WrapTimeValue ( time, repeat );

	u32 keyID = this->FindKeyID ( wrapTime );
	MOAIAnimKey k0 = this->mKeys [ keyID ];

	if ( keyID == endID ) {
		this->GetSampleForKey ( sample, keyID );
	}
	else if ( k0.mMode == USInterpolate::kFlat ) {
		this->GetSampleForKey ( sample, keyID );
	}
	else if ( k0.mTime == wrapTime ) {
		this->GetSampleForKey ( sample, keyID );
	}
	else {

		MOAIAnimSample v0;
		MOAIAnimSample v1;
		
		this->GetSampleForKey ( v0, keyID );
		this->GetSampleForKey ( v1, keyID + 1 );

		if ( v0.Equals ( v1 )) {
			sample = v0;
		}
		else {
		
			MOAIAnimKey k1 = this->mKeys [ keyID + 1 ];
			float span = k1.mTime - k0.mTime;
			
			if ( span == 0.0f ) {
				sample = v0;
			}
			else {
				float t = ( wrapTime - k0.mTime ) / span;
				sample.Interpolate ( v0, v1, k0.mMode, t, k0.mWeight );
			}
		}
	}

	//extra addition for Append mode
	if ( mWrapMode == APPEND ) {
		MOAIAnimSample curveDelta;
		this->GetCurveDelta ( curveDelta );
		curveDelta.Scale ( repeat );
		sample.Add ( sample, curveDelta );
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurve::GetValue ( MOAIAttrOp& attrOp, float time ) {

	MOAIAnimSample sample;
	this->GetSampleForTime ( sample, time );
	sample.GetAttrOp ( attrOp );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::Init ( u32 total, u32 type ) {

	this->Clear ();

	this->mKeys.Init ( total );
	
	this->mSampleType = type;
	this->mSampleSize = MOAIAnimSample::GetSize ( this->mSampleType );
	this->mBuffer = malloc ( total * this->mSampleSize );
}

//----------------------------------------------------------------//
MOAIAnimCurve::MOAIAnimCurve () :
	mTime ( 0.0f ),
	mWrapMode ( CLAMP ),
	mSampleType ( 0 ),
	mSampleSize ( 0 ),
	mBuffer ( 0 ) {
	
	RTTI_SINGLE ( MOAINode )
	
	this->mValue.Set ();
}

//----------------------------------------------------------------//
MOAIAnimCurve::~MOAIAnimCurve () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnimCurve::OnDepNodeUpdate () {

	this->GetSampleForTime ( this->mValue, this->mTime );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "ATTR_TIME", MOAIAnimCurveAttr::Pack ( ATTR_TIME ));
	state.SetField ( -1, "ATTR_VALUE", MOAIAnimCurveAttr::Pack ( ATTR_VALUE ));

	state.SetField ( -1, "CLAMP", ( u32 )CLAMP );
	state.SetField ( -1, "WRAP", ( u32 )WRAP );
	state.SetField ( -1, "MIRROR", ( u32 )MIRROR );
	state.SetField ( -1, "APPEND", ( u32 )APPEND );
	
	state.SetField ( -1, "TYPE_BOOL", ( u32 )MOAIAnimSample::TYPE_BOOL );
	state.SetField ( -1, "TYPE_FLOAT", ( u32 )MOAIAnimSample::TYPE_FLOAT );
	state.SetField ( -1, "TYPE_INDEX", ( u32 )MOAIAnimSample::TYPE_INDEX );
	state.SetField ( -1, "TYPE_INT", ( u32 )MOAIAnimSample::TYPE_INT );
	state.SetField ( -1, "TYPE_QUATERNION", ( u32 )MOAIAnimSample::TYPE_QUATERNION );
	state.SetField ( -1, "TYPE_TRANSFORM_2D", ( u32 )MOAIAnimSample::TYPE_TRANSFORM_2D );
	state.SetField ( -1, "TYPE_TRANSFORM_3D", ( u32 )MOAIAnimSample::TYPE_TRANSFORM_3D );
	state.SetField ( -1, "TYPE_VEC2", ( u32 )MOAIAnimSample::TYPE_VEC2 );
	state.SetField ( -1, "TYPE_VEC3", ( u32 )MOAIAnimSample::TYPE_VEC3 );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLength",			_getLength },
		{ "getValueAtTime",		_getValueAtTime },
		{ "reserveKeys",		_reserveKeys },
		{ "setKey",				_setKey },
		{ "setSample",			_setSample },
		{ "setWrapMode",		_setWrapMode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::SetKey ( u32 id, float time, u32 mode, float weight ) {

	if ( id < this->mKeys.Size ()) {
		this->mKeys [ id ].mTime = time;
		this->mKeys [ id ].mMode = mode;
		this->mKeys [ id ].mWeight = weight;
	}
}

//----------------------------------------------------------------//
void MOAIAnimCurve::SetSample ( u32 id, const MOAIAnimSample& sample ) {

	if ( id < this->mKeys.Size ()) {
		void* buffer = this->GetBufferForKey ( id );
		sample.GetBuffer ( buffer );
	}
}

//----------------------------------------------------------------//
// return the time wrapped to fall within the time span defined by the curve
// in APPEND mode, 'repeat' gives the cycle (how many complete spans have elapsed)
float MOAIAnimCurve::WrapTimeValue ( float t, float &repeat ) const {

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
