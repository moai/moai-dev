// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#ifdef DOXYGEN

//----------------------------------------------------------------//
/**	@name	getFarPlane
	@text	Returns the camera's far plane.

	@in		MOAICamera2D self
	@out	number far
*/
int MOAICamera2D::_getFarPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera2D, "U" )
	lua_pushnumber ( state, self->mFarPlane );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getNearPlane
	@text	Returns the camera's near plane.

	@in		MOAICamera2D self
	@out	number near
*/
int MOAICamera2D::_getNearPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera2D, "U" )
	lua_pushnumber ( state, self->mNearPlane );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFarPlane
	@text	Sets the camera's far plane distance.

	@in		MOAICamera2D self
	@opt	number far			Default value is -1.
	@out	nil
*/
int MOAICamera2D::_setFarPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera2D, "U" )
	self->mFarPlane = state.GetValue < float >( 2, DEFAULT_FAR_PLANE );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setNearPlane
	@text	Sets the camera's near plane distance.

	@in		MOAICamera2D self
	@opt	number near			Default value is 1.
	@out	nil
*/
int MOAICamera2D::_setNearPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera2D, "U" )
	self->mNearPlane = state.GetValue < float >( 2, DEFAULT_NEAR_PLANE );
	return 0;
}

#endif
