// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name getLength
	@param1 self @type userdata
	@text Returns the length of the curve.
	@return The length of the curve. @type number
*/
int MOAIAnimCurve::_getLength ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimCurve, "U" );

	lua_pushnumber ( state, self->GetLength ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name init
	@param1 self @type userdata
	@param2 numKeys @type userdata @text The number of keyframes in the curve.
	@text Initalizes the curve with a set number of keyframes.
	@return nil
*/
int MOAIAnimCurve::_reserveKeys ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimCurve, "UN" );

	self->Init ( state.GetValue < u32 >( 2, 0 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name setKey
	@param1 self @type userdata
	@param2 index @type integer @text The index of the keyframe in the table (starts at 0).
	@param3 time @type float @text The timestamp of the keyframe; when it occurs in the curve.
	@param4 value @type float @text The value of the keyframe; how much the attribute will be modified when the curve is at this keyframe.
	@param5 mode @type enum @text The mode of the keyframe.  Essentially the shape of the curve.  Defaults to SMOOTH; can be one of:
	\li \c EASEIN - Slows down as it approaches the next keyframe.
	\li \c EASEOUT - Comes slowly out of the current keyframe.
	\li \c FLAT - No curve.  Keeps the same value until another keyframe is reached (best for animation frames).
	\li \c LINEAR - A generic straight line that maintains a steady speed to the next keyframe.
	\li \c SMOOTH - Comes slowly out of the current keyframe and slows into the next one.
	\li \c SOFT_EASEIN - Like EASEIN, but with a less snappy curve.
	\li \c SOFT_EASEOUT - Like EASEOUT, but with a less snappy curve.
	\li \c SOFT_SMOOTH - Like SMOOTH, but with a less snappy curve.
	@text Sets the properties of a key within the curve.
	@return nil
*/
int MOAIAnimCurve::_setKey ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimCurve, "UNNN" );

	u32 index		= state.GetValue < u32 >( 2, 1 ) - 1;
	float time		= state.GetValue < float >( 3, 0.0f );
	float value		= state.GetValue < float >( 4, 0.0f );
	u32 mode		= state.GetValue < u32 >( 5, USInterpolate::kSmooth );
	float weight	= state.GetValue < float >( 6, 1.0f );
	
	self->SetKey ( index, time, value, mode, weight );

	return 0;
}

//================================================================//
// MOAIAnimCurve
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimCurve::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_TIME:
			this->mTime = attrOp.Op ( this->mTime );
			return;
		case ATTR_VALUE:
			attrOp.Op ( this->mValue );
			return;
	}
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

	state.SetField ( -1, "ATTR_TIME", ( u32 )ATTR_TIME );
	state.SetField ( -1, "ATTR_VALUE", ( u32 )ATTR_VALUE );
}

//----------------------------------------------------------------//
void MOAIAnimCurve::RegisterLuaFuncs ( USLuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
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
