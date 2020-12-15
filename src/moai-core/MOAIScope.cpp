// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIScope.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getFarPlane
	@text	Returns the camera's far plane.

	@in		MOAICamera self
	@out	number far
*/
int MOAIScope::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScope, "U" )
	self->Purge ();
	return 0;
}

//================================================================//
// MOAIScope
//================================================================//

//----------------------------------------------------------------//
void MOAIScope::AddObject ( MOAILuaObject* object ) {

	if ( !object ) return;
	if ( this->mObjects.contains ( object )) return;
	
	object->Retain ();
	this->mObjects.insert ( object );
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

	STLSet < MOAILuaObject* >::iterator objectIt = this->mObjects.begin ();
	for ( ; objectIt != this->mObjects.end (); ++objectIt ) {
		MOAILuaObject* object = *objectIt;
		object->Release ();
	}
	this->mObjects.clear ();
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
		{ "purge",				_purge },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
