// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxComposerRetained.h>
#include <moai-gfx/MOAIHasGfxComposer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxComposer::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposer, "U" )
	
	state.Push (( MOAIAbstractGfxComposer* )self->mComposer );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxComposer::_setComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposer, "U" )

	self->mComposer = state.GetLuaObject < MOAIAbstractGfxComposer >( 2, true );
	return 0;
}

//================================================================//
// MOAIHasGfxComposer
//================================================================//

//----------------------------------------------------------------//
bool MOAIHasGfxComposer::HasComposer () {

	return ( this->GetComposer () != NULL );
}

//----------------------------------------------------------------//
MOAIHasGfxComposer::MOAIHasGfxComposer () {

	RTTI_BEGIN ( MOAIHasGfxComposer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxComposer >)
		RTTI_EXTEND ( MOAIAbstractGfxComposerInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxComposer::~MOAIHasGfxComposer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxComposer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasGfxComposer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getComposer",				_getComposer },
		{ "setComposer",				_setComposer },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIHasGfxComposer::MOAIAbstractGfxComposerInterface_AffirmComposer () {

	if ( !this->mComposer ) {
		this->mComposer = new MOAIGfxComposerRetained ();
	}
	return *this->mComposer;
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIHasGfxComposer::MOAIAbstractGfxComposerInterface_GetComposer () {

	return this->mComposer;
}
