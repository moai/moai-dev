// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIScope.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIScope::_countRetained ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScope, "U" )
	state.Push (( int )self->mObjects.size ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIScope::_newindex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScope, "U" )

	// on stack:
	// 1: userdata
	// 2: key
	// 3: value

	// get the member table
	lua_getmetatable ( state, 1 ); // get ref table from userdata
	lua_getmetatable ( state, -1 ); // get member table from ref table
	
	// get the original value
	lua_pushvalue ( state, 2 ); // copy key to top
	lua_gettable ( state, -2 ); // get original value
	
	// check to see if the new value is different from the old value
	if ( lua_equal ( state, 3, -1 ) == 0 ) {
	
		// if the old value is a lua object, remove it.
		if ( state.IsType ( -1, LUA_TUSERDATA )) {
			MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -1, false );
			if ( object ) {
				self->RemoveObject ( *object );
			}
		}
	
		// if the new value is a lua object, add it.
		if ( state.IsType ( 3, LUA_TUSERDATA )) {
			MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 3, false );
			if ( object ) {
				self->AffirmObject ( *object );
			}
		}

		lua_pop ( state, 1 ); // pop the original value

		lua_pushvalue ( state, 2 ); // copy key to top
		lua_pushvalue ( state, 3 ); // copy value to top
		lua_settable ( state, -3 );
	}

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIScope::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScope, "U" )
	self->Purge ();
	return 0;
}

//================================================================//
// MOAIScope
//================================================================//

//----------------------------------------------------------------//
void MOAIScope::AffirmObject ( MOAILuaObject& object ) {

	if ( this->mObjects.contains ( &object )) return;
	
	object.Retain ();
	this->mObjects.insert ( &object );
}

//----------------------------------------------------------------//
MOAIScope::MOAIScope () :
	mScopeRefCount ( 0 ) {

	RTTI_BEGIN ( MOAIScope )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIScope >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIScope::~MOAIScope () {

	this->Purge ();
}

//----------------------------------------------------------------//
void MOAIScope::Purge () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushMemberTable ( state );
	
	u32 idx = state.AbsIndex ( -1 );
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
	
		lua_pushvalue ( state, idx ); // push table
		lua_pushvalue ( state, -3 ); // push key
		lua_pushnil ( state );
		lua_rawset ( state, -3 );
		lua_pop ( state, 1 );
	}
	state.Pop ();

	STLSet < MOAILuaObject* >::iterator objectIt = this->mObjects.begin ();
	for ( ; objectIt != this->mObjects.end (); ++objectIt ) {
		MOAILuaObject* object = *objectIt;
		object->Release ();
	}
	this->mObjects.clear ();
}

//----------------------------------------------------------------//
void MOAIScope::RemoveObject ( MOAILuaObject& object ) {
	
	this->mObjects.erase ( &object );
	object.Release ();
}

//----------------------------------------------------------------//
void MOAIScope::ScopeRelease () {

	if ( this->mScopeRefCount > 0 ) {
		this->mScopeRefCount--;
		if ( this->mScopeRefCount == 0 ) {
			this->Purge ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIScope::ScopeRetain () {

	this->mScopeRefCount++;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIScope::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIScope::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "countRetained",		_countRetained },
		{ "purge",				_purge },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIScope::MOAILuaObject_DecorateLuaBinding ( MOAILuaState& state ) {

	lua_getmetatable ( state, -1 );
	lua_pushcfunction ( state, MOAIScope::_newindex );
	lua_setfield ( state, -2, "__newindex" );
	lua_pop ( state, 1 );
}
