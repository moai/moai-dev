// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	
	MOAIAction* root = state.GetLuaObject < MOAIAction >( 2, true );
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
		this->mRoot.Set ( *this, new MOAIAction ());
	}
	return this->mRoot;
}

//----------------------------------------------------------------//
MOAIAction* MOAIActionTree::GetDefaultParent () {

	MOAIAction* cursor = this->mCurrentAction;
	for ( ; cursor; cursor = cursor->mParent ) {
		if ( cursor->mIsDefaultParent ) {
			return cursor;
		}
	}
	return this->AffirmRoot ();
}

//----------------------------------------------------------------//
bool MOAIActionTree::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIActionTree::MOAIActionTree () :
	mPass ( RESET_PASS ),
	mProfilingEnabled ( false ),
	mThreadInfoEnabled ( false ),
	mCurrentAction ( 0 ),
	mDefaultParent ( 0 ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIActionTree::~MOAIActionTree () {

	this->mRoot.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIActionTree::OnUpdate ( double step ) {

	this->Update ( step );
}

//----------------------------------------------------------------//
void MOAIActionTree::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIActionTree::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRoot",				_getRoot },
		{ "setProfilingEnabled",	_setProfilingEnabled },
		{ "setRoot",				_setRoot },
		{ "setThreadInfoEnabled",	_setThreadInfoEnabled },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIActionTree::SetDefaultParent () {

	this->mDefaultParent = 0;
}

//----------------------------------------------------------------//
void MOAIActionTree::SetDefaultParent ( MOAIAction* defaultParent ) {

	this->mDefaultParent = defaultParent;
}

//----------------------------------------------------------------//
void MOAIActionTree::SetRoot ( MOAIAction* root ) {

	this->mRoot.Set ( *this, root );
}

//----------------------------------------------------------------//
void MOAIActionTree::Update ( double step ) {

	MOAIAction* root = this->mRoot;	

	if ( root ) {

		this->mPass++; // on to the next pass
		
		// handles the case when Moai has been running continuously
		// for approx. 136 years at 60 fps
		if ( this->mPass == 0xffffffff ) {
			root->ResetPass ();
		}
		
		root->Retain ();
		
		this->mDefaultParent = 0;
		root->Update ( step );

		root->Release ();

		this->mDefaultParent = 0;
		this->mCurrentAction = 0;
	}
}
