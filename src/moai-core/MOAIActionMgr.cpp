// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAction.h>
#include <moai-core/MOAIActionMgr.h>
#include <moai-core/MOAIActionTree.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getActionTree
	@text	Get the action manager's action tree. This is the 'global' action tree
			to which all newly started actions are automatically added.

	@out	MOAIActionTree actionMgr
*/
int MOAIActionMgr::_getActionTree ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIActionMgr, "" )

	self->GetActionTree ().PushLuaUserdata ( state );
	return 1;
}

//================================================================//
// MOAIActionMgr
//================================================================//
	
//----------------------------------------------------------------//
MOAIAction* MOAIActionMgr::GetCurrent () {

	ZLSize top = this->mStack.GetTop ();
	return top > 0 ? this->mStack [ top - 1 ] : 0;
}

//----------------------------------------------------------------//
MOAIAction* MOAIActionMgr::GetDefaultParent () {

	MOAIAction* defaultParent = 0;

	MOAIAction* cursor = this->GetCurrent ();
	for ( ; cursor; cursor = cursor->mParent ) {
		defaultParent = cursor->GetDefaultParent ();
		if ( defaultParent ) {
			return defaultParent;
		}
	}
	return this->mActionTree->GetDefaultParent ();
}

//----------------------------------------------------------------//
MOAIActionMgr::MOAIActionMgr () {

	this->mActionTree.Set ( *this, new MOAIActionTree ());
}

//----------------------------------------------------------------//
MOAIActionMgr::~MOAIActionMgr () {

	this->mActionTree.Set ( *this, NULL );
}

//----------------------------------------------------------------//
void MOAIActionMgr::Pop () {

	this->mStack.Pop ();
}

//----------------------------------------------------------------//
void MOAIActionMgr::Push ( MOAIAction& action ) {

	this->mStack.Push ( &action );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIActionMgr::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getActionTree",				_getActionTree },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIActionMgr::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}
