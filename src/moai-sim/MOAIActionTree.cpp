// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIActionTree.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRoot
	@text	Returns the current root action.

	@out	MOAIAction root
*/
int MOAIActionTree::_getRoot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIActionTree, "U" )
	
	MOAIAction* root = self->AffirmRoot ();
	root->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setProfilingEnabled
	@text	Enables action profiling.

	@opt	boolean enable		Default value is false.
	@out	nil
*/
int MOAIActionTree::_setProfilingEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIActionTree, "U" )
	
	bool enable = state.GetValue < bool >( 2, false );
	self->SetProfilingEnabled ( enable );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRoot
	@text	Replaces or clears the root action.

	@opt	MOAIAction root		Default value is nil.
	@out	nil
*/
int MOAIActionTree::_setRoot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIActionTree, "U" )
	
	MOAIAction* root = state.GetLuaObject < MOAIAction >( 2, false );
	self->SetRoot ( root );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setThreadInfoEnabled
	@text	Enables function name and line number info for MOAICoroutine.

	@opt	boolean enable		Default value is false.
	@out	nil
*/
int MOAIActionTree::_setThreadInfoEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIActionTree, "U" )

	bool enable = state.GetValue < bool >( 2, false );
	self->SetThreadInfoEnabled ( enable );

	return 0;
}

//================================================================//
// MOAIActionTree
//================================================================//

//----------------------------------------------------------------//
MOAIAction* MOAIActionTree::AffirmRoot () {

	if ( !this->mRoot ) {
		this->SetRoot ( new MOAIAction ());
	}
	return this->mRoot;
}

//----------------------------------------------------------------//
MOAIActionTree::MOAIActionTree () :
	mRoot ( 0 ),
	mProfilingEnabled ( false ),
	mThreadInfoEnabled ( false ) {
	
	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIActionTree, MOAIAction )
}

//----------------------------------------------------------------//
MOAIActionTree::~MOAIActionTree () {
}

//----------------------------------------------------------------//
void MOAIActionTree::SetRoot ( MOAIAction* root ) {

	if ( this->mRoot ) {
		this->mRoot->Detach ();
	}

	this->mRoot = root;

	if ( this->mRoot ) {
		this->mRoot->Attach ( this, false );
	}
}

//----------------------------------------------------------------//
void MOAIActionTree::Update ( double step ) {

	this->MOAIAction::Update ( *this, step );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIActionTree::MOAIAction_DidLoseChild ( MOAIAction* child ) {
	if ( this->mRoot == child ) {
		this->mRoot = 0;
	}
}

//----------------------------------------------------------------//
MOAIAction* MOAIActionTree::MOAIAction_GetDefaultParent () {

	return this->AffirmRoot ();
}

//----------------------------------------------------------------//
bool MOAIActionTree::MOAIAction_IsDone () {
	return false;
}

//----------------------------------------------------------------//
void MOAIActionTree::MOAIAction_Update ( double step ) {
	UNUSED ( step );
}

//----------------------------------------------------------------//
void MOAIActionTree::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIActionTree::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getRoot",				_getRoot },
		{ "setProfilingEnabled",	_setProfilingEnabled },
		{ "setRoot",				_setRoot },
		{ "setThreadInfoEnabled",	_setThreadInfoEnabled },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
