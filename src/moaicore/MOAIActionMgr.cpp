// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getRoot
	@text	Returns the current root action.

	@out	MOAIAction root
*/
int MOAIActionMgr::_getRoot ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	MOAIAction* root = MOAIActionMgr::Get ().AffirmRoot ();
	root->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setProfilingEnabled
	@text	Enables action profiling.

	@opt	boolean enable		Default value is false.
	@out	nil
*/
int MOAIActionMgr::_setProfilingEnabled ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool enable = state.GetValue < bool >( -1, false );
	MOAIActionMgr::Get ().SetProfilingEnabled ( enable );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRoot
	@text	Replaces or clears the root action.

	@opt	MOAIAction root		Default value is nil.
	@out	nil
*/
int MOAIActionMgr::_setRoot ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	MOAIAction* root = state.GetLuaObject < MOAIAction >( -1, true );
	MOAIActionMgr::Get ().mRoot = root;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setThreadInfoEnabled
	@text	Enables function name and line number info for MOAICoroutine.

	@opt	boolean enable		Default value is false.
	@out	nil
*/
int MOAIActionMgr::_setThreadInfoEnabled ( lua_State* L ) {
	
	MOAILuaState state ( L );
	bool enable = state.GetValue < bool >( -1, false );
	MOAIActionMgr::Get ().SetThreadInfoEnabled ( enable );

	return 0;
}

//================================================================//
// MOAIActionMgr
//================================================================//

//----------------------------------------------------------------//
MOAIAction* MOAIActionMgr::AffirmRoot () {

	if ( !this->mRoot ) {
		this->mRoot = new MOAIAction ();
		this->LuaRetain ( this->mRoot );
	}
	return this->mRoot;
}

//----------------------------------------------------------------//
u32 MOAIActionMgr::GetNextPass () {

	this->mTotalPasses = this->mPass + 2;
	return this->mPass + 1;
}

//----------------------------------------------------------------//
MOAIActionMgr::MOAIActionMgr () :
	mPass ( RESET_PASS ),
	mProfilingEnabled ( false ),
	mThreadInfoEnabled ( false ),
	mRoot ( 0 ),
	mCurrentAction ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIActionMgr::~MOAIActionMgr () {

	this->LuaRelease ( this->mRoot );
	this->mRoot = 0;
}

//----------------------------------------------------------------//
void MOAIActionMgr::RegisterLuaClass ( MOAILuaState& state ) {

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
void MOAIActionMgr::Update ( float step ) {

	MOAIAction* root = this->mRoot;

	if ( root ) {

		this->GetNextPass ();
		
		root->Retain ();
		
		for ( this->mPass = 0; this->mPass < this->mTotalPasses; ++this->mPass ) {
			root->Update ( step, this->mPass, true );
		}

		root->Release ();

		this->mPass = RESET_PASS;
		this->mCurrentAction = 0;
	}
}
