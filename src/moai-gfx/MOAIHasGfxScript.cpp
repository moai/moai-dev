// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxScript.h>
#include <moai-gfx/MOAIHasGfxScript.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScript::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScript, "U" )
	
	state.Push (( MOAIAbstractGfxScript* )self->mComposer );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScript::_setComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScript, "U" )

	self->mComposer = state.GetLuaObject < MOAIAbstractGfxScript >( 2, true );
	return 0;
}

//================================================================//
// MOAIHasGfxScript
//================================================================//

//----------------------------------------------------------------//
bool MOAIHasGfxScript::HasComposer () {

	return ( this->GetComposer () != NULL );
}

//----------------------------------------------------------------//
MOAIHasGfxScript::MOAIHasGfxScript () {

	RTTI_BEGIN ( MOAIHasGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScript >)
		RTTI_EXTEND ( MOAIAbstractGfxScriptInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScript::~MOAIHasGfxScript () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxScript::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasGfxScript::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getComposer",				_getComposer },
		{ "setComposer",				_setComposer },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIHasGfxScript::MOAIAbstractGfxScriptInterface_AffirmComposer () {

	if ( !this->mComposer ) {
		this->mComposer = new MOAIGfxScript ();
	}
	return *this->mComposer;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIHasGfxScript::MOAIAbstractGfxScriptInterface_GetComposer () {

	return this->mComposer;
}
