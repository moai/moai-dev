// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAILogMessages.h>

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
	@in		number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@in		number weight			Blends between chosen ease type (of any) and a linear transition.
	@out	nil
*/
int MOAIAnimCurve::_setKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurve, "UNNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	MOAI_CHECK_INDEX ( index, self->Size ())
	
	self->SetKey ( index, time, value, mode, weight );

	return 0;
}

//================================================================//
// MOAIAnimCurve
//================================================================//

//----------------------------------------------------------------//
bool MOAIAnimCurve::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	if ( MOAIAnimCurveAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_TIME:
				this->mTime = attrOp.Op ( this->mTime );
				return true;
			case ATTR_VALUE:
				attrOp.Op ( this->mValue );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIAnimCurve::MOAIAnimCurve () :
	mTime ( 0.0f ),
	mValue ( 0.0f ) {
	
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
void MOAIAnimCurve::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "ATTR_TIME", MOAIAnimCurveAttr::Pack ( ATTR_TIME ));
	state.SetField ( -1, "ATTR_VALUE", MOAIAnimCurveAttr::Pack ( ATTR_VALUE ));
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaFuncs ( USLuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLength",		_getLength },
		{ "reserveKeys",	_reserveKeys },
		{ "setKey",			_setKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIAnimCurve::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mTime );
	PRETTY_PRINT ( repr, mValue );

	return repr;
}
