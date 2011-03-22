// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPointerSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>getLoc ( )</tt>\n
\n
	Returns the X and Y coordinates of the mouse pointer.
	@return X and y coordinates of the mouse in screen coordinates.
*/
int MOAIPointerSensor::_getLoc ( lua_State* L ) {
	LUA_SETUP ( MOAIPointerSensor, "U" )

	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>setCallback ( ref )</tt>\n
\n
	Sets the callback function for when the pointer moves.
	@param ref Function to call.
*/
int MOAIPointerSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIPointerSensor, "UF" )
	
	self->mOnMove.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAIPointerSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIPointerSensor::HandleEvent ( USStream& eventStream ) {

	int x = eventStream.Read < int >();
	int y = eventStream.Read < int >();

	this->mX = x;
	this->mY = y;
	
	if ( this->mOnMove ) {
		USLuaStateHandle state = this->mOnMove.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
MOAIPointerSensor::MOAIPointerSensor () {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIPointerSensor::~MOAIPointerSensor () {
}

//----------------------------------------------------------------//
void MOAIPointerSensor::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPointerSensor::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "getLoc",			_getLoc },
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIPointerSensor::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mX )
	PRETTY_PRINT ( repr, mY )

	return repr;
}

//----------------------------------------------------------------//
void MOAIPointerSensor::WriteEvent ( USStream& eventStream, int x, int y ) {

	eventStream.Write < int >( x );
	eventStream.Write < int >( y );
}
