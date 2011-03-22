// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICp.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( bias ) getBiasCoefficient ( self )</tt>\n
\n
	Return the bias coefficient.
	@param self (in)
	@param bias (out)
*/
int MOAICp::_getBiasCoefficient ( lua_State* L ) {
	
	lua_pushnumber ( L, cp_bias_coef );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICp::_getCollisionSlop ( lua_State* L ) {
	
	lua_pushnumber ( L, cp_collision_slop );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICp::_getContactPersistence ( lua_State* L ) {

	lua_pushnumber ( L, cp_contact_persistence );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICp::_setBiasCoefficient ( lua_State* L ) {
	USLuaState state ( L );
	
	cp_bias_coef =	state.GetValue < cpFloat >( 1, cp_bias_coef );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICp::_setCollisionSlop ( lua_State* L ) {
	USLuaState state ( L );
	
	cp_collision_slop =	state.GetValue < cpFloat >( 1, cp_collision_slop );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICp::_setContactPersistence ( lua_State* L ) {
	USLuaState state ( L );
	
	cp_contact_persistence =	state.GetValue < cpTimestamp >( 1, cp_contact_persistence );
	return 0;
}

//================================================================//
// MOAICp
//================================================================//

//----------------------------------------------------------------//
MOAICp::MOAICp () {

	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAICp::~MOAICp () {
}

//----------------------------------------------------------------//
void MOAICp::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "INFINITY",	INFINITY );

	LuaReg regTable[] = {
		{ "getBiasCoefficient",		_getBiasCoefficient },
		{ "getCollisionSlop",		_getCollisionSlop },
		{ "getContactPersistence",	_getContactPersistence },
		{ "setBiasCoefficient",		_setBiasCoefficient },
		{ "setCollisionSlop",		_setCollisionSlop },
		{ "setContactPersistence",	_setContactPersistence },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

#endif