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
	
	USLuaState state ( L );
	
	MOAIAction* root = MOAIActionMgr::Get ().AffirmRoot ();
	root->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setRoot
	@text	Replaces or clears the root action.

	@opt	MOAIAction root		Default value is nil.
	@out	nil
*/
int MOAIActionMgr::_setRoot ( lua_State* L ) {
	
	USLuaState state ( L );
	
	MOAIAction* root = state.GetLuaObject < MOAIAction >( -1 );
	MOAIActionMgr::Get ().mRoot = root;

	return 0;
}

//================================================================//
// MOAIActionMgr
//================================================================//

//----------------------------------------------------------------//
MOAIAction* MOAIActionMgr::AffirmRoot () {

	if ( !this->mRoot ) {
		this->mRoot = new MOAIAction ();
		assert ( this->mRoot );
	}
	return this->mRoot;
}

//----------------------------------------------------------------//
void MOAIActionMgr::Clear () {

	this->mRoot = 0;
}

//----------------------------------------------------------------//
u32 MOAIActionMgr::GetNextPass () {

	this->mTotalPasses = this->mPass + 2;
	return this->mPass + 1;
}

//----------------------------------------------------------------//
MOAIActionMgr::MOAIActionMgr () :
	mPass ( RESET_PASS ),
	mCurrentAction ( 0 ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIActionMgr::~MOAIActionMgr () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIActionMgr::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRoot",			_getRoot },
		{ "setRoot",			_setRoot },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIActionMgr::StartAction ( MOAIAction& action ) {

	MOAIAction* root = this->AffirmRoot ();
	root->AddChild ( action );
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
