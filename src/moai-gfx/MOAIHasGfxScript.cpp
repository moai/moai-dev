// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIDraw.h>
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
int MOAIHasGfxScript::_gfx ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScript, "U" )
	
	MOAIGfxScript& gfxScript = self->AffirmGfxScript ();
	gfxScript.Reset ();
	gfxScript.AffirmMedium ().PushCmdInterfaceWithHandler ( state, MOAIDraw::Get ());
	
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
MOAIGfxScript& MOAIHasGfxScript::AffirmGfxScript () {

	return this->MOAIHasGfxScript_AffirmGfxScript ();
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIHasGfxScript::GetGfxScript () {

	return this->mGfxScript;
}

//----------------------------------------------------------------//
MOAIHasGfxScript::MOAIHasGfxScript () {

	RTTI_BEGIN ( MOAIHasGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScript >)
		RTTI_EXTEND ( MOAILuaObject )
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
		{ "gfx",						_gfx },
		{ "setGfxScript",				_setGfxScript },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIGfxScript& MOAIHasGfxScript::MOAIHasGfxScript_AffirmGfxScript () {

	if ( !this->mGfxScript ) {
		this->mGfxScript = new MOAIGfxScript ();
	}
	return *this->mGfxScript;
}
