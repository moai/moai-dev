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
int MOAIHasGfxScript::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScript, "U" )

	state.Push (( MOAIGfxScript* )self->GetGfxScript ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScript::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScript, "U" )

	self->SetGfxScript ( state.GetLuaObject < MOAIGfxScript >( 2, true ));
	return 0;
}

//================================================================//
// MOAIHasGfxScript
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScript* MOAIHasGfxScript::GetGfxScript () {

	return this->mGfxScript;
}

//----------------------------------------------------------------//
MOAIHasGfxScript::MOAIHasGfxScript () {

	RTTI_BEGIN ( MOAIHasGfxScript )
		RTTI_EXTEND ( MOAIAbstractHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScript::~MOAIHasGfxScript () {
}

//----------------------------------------------------------------//
void MOAIHasGfxScript::SetGfxScript ( MOAIGfxScript* gfxScript ) {

	this->mGfxScript = gfxScript;
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
		{ "getGfxScript",				_getGfxScript },
		{ "setGfxScript",				_setGfxScript },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIHasGfxScript::MOAIAbstractHasGfxScript_AffirmGfxScript () {

	MOAIGfxScript* gfxScript = this->mGfxScript ? MOAICast < MOAIGfxScript >( this->mGfxScript ) : NULL;
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScript ();
		this->mGfxScript = gfxScript;
	}
	return *gfxScript;
}
