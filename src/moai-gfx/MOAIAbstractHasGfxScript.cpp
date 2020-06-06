// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractHasGfxScript.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScriptRetained.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )
	
	state.Push (( MOAIAbstractGfxScript* )self->GetGfxScript ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	self->SetGfxScript ( state.GetLuaObject < MOAIAbstractGfxScript >( 2, true ));
	return 0;
}

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIAbstractHasGfxScript::AffirmGfxScript () {

	return this->MOAIAbstractHasGfxScript_AffirmGfxScript ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractHasGfxScript::GetGfxScript () {

	return this->MOAIAbstractHasGfxScript_GetGfxScript ();
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::MOAIAbstractHasGfxScript () {

	RTTI_BEGIN ( MOAIAbstractHasGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractHasGfxScript >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::~MOAIAbstractHasGfxScript () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetGfxScript ( MOAIAbstractGfxScript* gfxScript ) {

	this->MOAIAbstractHasGfxScript_SetGfxScript ( gfxScript );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getGfxScript",				_getGfxScript },
		{ "setGfxScript",				_setGfxScript },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}
