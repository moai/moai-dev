// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICpArbiter.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_countContacts ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		int count = cpArbiterGetCount ( self->mArbiter );
		lua_pushnumber ( state, count );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_getContactDepth ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "UN" )

	if ( self->mArbiter ) {
		int i = state.GetValue < int >( 2, 0 );
		cpFloat d = cpArbiteGetDepth ( self->mArbiter, i );
		lua_pushnumber ( state, d );
		return 1;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_getContactNormal ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "UN" )

	if ( self->mArbiter ) {
		int i = state.GetValue < int >( 2, 0 );
		cpVect n = cpArbiterGetNormal ( self->mArbiter, i );
		lua_pushnumber ( state, n.x );
		lua_pushnumber ( state, n.y );
		return 2;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_getContactPoint ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "UN" )

	if ( self->mArbiter ) {
		int i = state.GetValue < int >( 2, 0 );
		cpVect p = cpArbiterGetPoint ( self->mArbiter, i );
		lua_pushnumber ( state, p.x );
		lua_pushnumber ( state, p.y );
		return 2;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_getTotalImpulse ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		cpVect v = cpArbiterTotalImpulse ( self->mArbiter );
		lua_pushnumber ( state, v.x );
		lua_pushnumber ( state, v.y );
		return 2;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_getTotalImpulseWithFriction ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		cpVect v = cpArbiterTotalImpulseWithFriction ( self->mArbiter );
		lua_pushnumber ( state, v.x );
		lua_pushnumber ( state, v.y );
		return 2;		
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_isFirstContact ( lua_State* L ) {
	LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		bool b = cpArbiterIsFirstContact ( self->mArbiter ) == cpTrue;
		lua_pushboolean ( state, b );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpArbiter::_new ( lua_State* L ) {
	UNUSED ( L );
	
	printf ( "MOAICpArbiter.new () is unsupported.\n" );
	return 0;
}

//================================================================//
// MOAICpArbiter
//================================================================//

//----------------------------------------------------------------//
MOAICpArbiter::MOAICpArbiter () :
	mArbiter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICpArbiter::~MOAICpArbiter () {
}

//----------------------------------------------------------------//
void MOAICpArbiter::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "new",							_new },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpArbiter::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "countContacts",					_countContacts },
		{ "getContactDepth",				_getContactDepth },
		{ "getContactNormal",				_getContactNormal },
		{ "getContactPoint",				_getContactPoint },
		{ "getTotalImpulse",				_getTotalImpulse },
		{ "getTotalImpulseWithFriction",	_getTotalImpulseWithFriction },
		{ "isFirstContact",					_isFirstContact },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAICpArbiter::ToString () {

	STLString repr;
	return repr;
}

#endif