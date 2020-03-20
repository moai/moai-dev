// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxComposer.h>
#include <moai-gfx/MOAIHasGfxComposer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxComposer::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposer, "U" )
	
	state.Push (( MOAIGfxComposer* )self->mComposer );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxComposer::_setComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposer, "U" )

	self->mComposer = state.GetLuaObject < MOAIGfxComposer >( 2, true );
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
		RTTI_EXTEND ( MOAIGfxComposerInterface )
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
MOAIGfxComposer& MOAIHasGfxComposer::MOAIGfxComposerInterface_AffirmComposer () {

	if ( !this->mComposer ) {
		this->mComposer = new MOAIGfxComposer ();
	}
	return *this->mComposer;
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIHasGfxComposer::MOAIGfxComposerInterface_GetComposer () {

	return this->mComposer;
}
