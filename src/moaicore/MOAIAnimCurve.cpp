// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( length ) getLength ()</tt>\n
	\n
	Return the duration of the curve.
	@param length (out)
*/
int MOAIAnimCurve::_getLength ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimCurve, "U" );

	lua_pushnumber ( state, self->GetLength ());

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>init ( self, numKeys )</tt>\n
	\n
	Initializes an animation curve object.
	@param self (in)
	@param numKeys (in) The number of keyframes in the curve.
*/
int MOAIAnimCurve::_reserveKeys ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimCurve, "UN" );

	self->Init ( state.GetValue < u32 >( 2, 0 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setKey ( self, index, time, value, modde )</tt>\n
	\n
	Sets the location and properties of keyframes in the curve.
	@param self (in)
	@param index (in) The index of this keyframe in the table (starts at 0).
	@param time (in) The timestamp of the keyframe, or when it occurs.
	@param value (in) The value of the keyframe.
	@param mode (in) The mode of the keyframe.  Essentially the shape of the curve. (optional , default is SMOOTH)
	\li \c EASEIN - Slows down as it approaches the next keyframe.
	\li \c EASEOUT - Comes slowly out of the current keyframe.
	\li \c FLAT - No curve.  Keeps the same value until another keyframe is reached (best for animation frames).
	\li \c LINEAR - A generic straight line that maintains a steady speed to the next keyframe.
	\li \c SMOOTH - Comes slowly out of the current keyframe and slows into the next one.
	\li \c SOFT_EASEIN - Like EASEIN, but with a less snappy curve.
	\li \c SOFT_EASEOUT - Like EASEOUT, but with a less snappy curve.
	\li \c SOFT_SMOOTH - Like SMOOTH, but with a less snappy curve.
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
u32 MOAIAnimCurve::CountAttributes () {

	return MOAIAnimCurve::TOTAL_ATTR;
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
