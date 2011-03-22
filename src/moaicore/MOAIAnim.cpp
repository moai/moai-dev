// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( length ) getLength ( self )</tt>\n
\n
	Returns the total length of the animation.
	@param self (in)
*/
int	MOAIAnim::_getLength ( lua_State* L ) {
	LUA_SETUP ( MOAIAnim, "U" )
	
	lua_pushnumber ( state, self->GetLength ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveCurves ( self, totalCurves )</tt>\n
\n
	Reserves a specified number of animation curves.
	@param self (in)
	@param totalCurves (in) Number of curves to reserve.
*/
int	MOAIAnim::_reserveCurves ( lua_State* L ) {
	LUA_SETUP ( MOAIAnim, "UN" )
	
	u32 totalCurves = state.GetValue < u32 >( 2, 0 );
	self->ReserveCurves ( totalCurves );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setCurve ( self, curve )</tt>\n
\n
	Set an animation curve into a slot in the anim.
	@param self (in)
	@param curve (out)
*/
int	MOAIAnim::_setCurve ( lua_State* L ) {
	LUA_SETUP ( MOAIAnim, "UNU" )
	
	u32 curveID				= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAIAnimCurve* curve	= state.GetLuaData < MOAIAnimCurve >( 3 );
	
	self->SetCurve ( curveID, curve );
	
	return 0;
}

//================================================================//
// MOAIAnim
//================================================================//

//----------------------------------------------------------------//
void MOAIAnim::Clear () {

	u32 total = this->mCurves.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		MOAIAnimCurve* curve = this->mCurves [ i ];
		if ( curve ) {
			curve->Release ();
		}
	}
	this->mCurves.Clear ();
	this->mLength = 0.0f;
}

//----------------------------------------------------------------//
float MOAIAnim::Eval ( u32 curveID, float time ) {

	if ( this->mCurves [ curveID ]) {
		return this->mCurves [ curveID ]->GetFloatValue ( time );
	}
	return 0.0f;
}

//----------------------------------------------------------------//
float MOAIAnim::EvalDelta ( u32 curveID, float t0, float t1 ) {

	if ( this->mCurves [ curveID ]) {
		return this->mCurves [ curveID ]->GetFloatDelta ( t0, t1 );
	}
	return 0.0f;
}

//----------------------------------------------------------------//
MOAIAnim::MOAIAnim () :
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIAnim::~MOAIAnim () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "getLength",			_getLength },
		{ "reserveCurves",		_reserveCurves },
		{ "setCurve",			_setCurve },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnim::ReserveCurves ( u32 totalCurves ) {

	this->mCurves.Init ( totalCurves );
	this->mCurves.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIAnim::SetCurve ( u32 curveID, MOAIAnimCurve* curve ) {

	if ( curveID < this->mCurves.Size ()) {
	 
		float length = 0.0f;

		if ( curve ) {
			curve->Retain ();
			length = curve->GetLength ();
		}

		if ( this->mLength < length ) {
			this->mLength = length;
		}

		if ( this->mCurves [ curveID ]) {
			this->mCurves [ curveID ]->Release ();
		}

		this->mCurves [ curveID ] = curve;
	}
}

//----------------------------------------------------------------//
STLString MOAIAnim::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mLength )
	PRETTY_PRINT ( repr, mCurves )

	return repr;
}
