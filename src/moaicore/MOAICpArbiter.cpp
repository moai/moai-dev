// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICpArbiter.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	countContacts
	@text	Returns the number of contacts occurring with this arbiter.

	@in		MOAICpArbiter self
	@out	number count		The number of contacts occurring.
*/
int MOAICpArbiter::_countContacts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		int count = cpArbiterGetCount ( self->mArbiter );
		lua_pushnumber ( state, count );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getContactDepth
	@text	Returns the depth of a contact point between two objects.

	@in		MOAICpArbiter self
	@in		number id			The ID of the contact.
	@out	number depth		The depth of the contact in pixels (i.e. how far it overlaps).
*/
int MOAICpArbiter::_getContactDepth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "UN" )

	if ( self->mArbiter ) {
		int i = state.GetValue < int >( 2, 0 );
		cpFloat d = cpArbiteGetDepth ( self->mArbiter, i );
		lua_pushnumber ( state, d );
		return 1;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getContactNormal
	@text	Returns the normal of a contact point between two objects.

	@in		MOAICpArbiter self
	@in		number id			The ID of the contact.
	@out	boolean x			The X component of the normal vector.
	@out	boolean y			The Y component of the normal vector.
*/
int MOAICpArbiter::_getContactNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "UN" )

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
/**	@name	getContactPoint
	@text	Returns the position of a contact point between two objects.

	@in		MOAICpArbiter self
	@in		number id			The ID of the contact.
	@out	boolean x			The X component of the position vector.
	@out	boolean y			The Y component of the position vector.
*/
int MOAICpArbiter::_getContactPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "UN" )

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
/**	@name	getTotalImpulse
	@text	Returns the total impulse of a contact point between two objects.

	@in		MOAICpArbiter self
	@out	boolean x			The X component of the force involved in the contact.
	@out	boolean y			The Y component of the force involved in the contact.
*/
int MOAICpArbiter::_getTotalImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		cpVect v = cpArbiterTotalImpulse ( self->mArbiter );
		lua_pushnumber ( state, v.x );
		lua_pushnumber ( state, v.y );
		return 2;	
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTotalImpulseWithFriction
	@text	Returns the total impulse of a contact point between two objects, also including frictional forces.

	@in		MOAICpArbiter self
	@out	boolean x			The X component of the force involved in the contact.
	@out	boolean y			The Y component of the force involved in the contact.
*/
int MOAICpArbiter::_getTotalImpulseWithFriction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		cpVect v = cpArbiterTotalImpulseWithFriction ( self->mArbiter );
		lua_pushnumber ( state, v.x );
		lua_pushnumber ( state, v.y );
		return 2;		
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	isFirstContact
	@text	Returns whether this is the first time that these two objects have contacted.

	@in		MOAICpArbiter self
	@out	boolean first		Whether this is the first instance of a collision.
*/
int MOAICpArbiter::_isFirstContact ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpArbiter, "U" )

	if ( self->mArbiter ) {
		bool b = cpArbiterIsFirstContact ( self->mArbiter ) == cpTrue;
		lua_pushboolean ( state, b );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAICpArbiter
//================================================================//

//----------------------------------------------------------------//
MOAICpArbiter::MOAICpArbiter () :
	mArbiter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICpArbiter::~MOAICpArbiter () {
}

//----------------------------------------------------------------//
void MOAICpArbiter::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "new",							MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpArbiter::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
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

#endif