// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIHasGfxScriptsForPhases.h>
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIHasGfxScriptsForPhases::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptsForPhases, "U" )
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	MOAIGfxScript* script = self->GetGfxScript ( renderPhase );
	state.Push ( script );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptsForPhases::_gfx ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptsForPhases, "U" )
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	MOAIGfxScript& script = self->AffirmGfxScript ( renderPhase );
	script.Reset ();
	script.AffirmMedium ().PushCmdInterfaceWithHandler ( state, self->Get < MOAIDraw >());
	return 1;
}

//----------------------------------------------------------------//
int MOAIHasGfxScriptsForPhases::_gfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptsForPhases, "U" )
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	MOAIGfxScript& script = self->AffirmGfxScript ( renderPhase );
	script.PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
int MOAIHasGfxScriptsForPhases::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptsForPhases, "U" )
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	MOAIGfxScript* script = state.GetLuaObject < MOAIGfxScript >( 3, false );
	self->SetGfxScript ( renderPhase, script );
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIHasGfxScriptsForPhases
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScript& MOAIHasGfxScriptsForPhases::AffirmGfxScript ( MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxScript* gfxScript = this->GetGfxScript ( renderPhase );
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScript ( this->GetContext ());
		this->mGfxScripts [ renderPhase ] = gfxScript;
	}
	assert ( gfxScript );
	return *gfxScript;
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIHasGfxScriptsForPhases::GetGfxScript ( MOAIRenderPhaseEnum::_ renderPhase ) {

	STLMap < u32, ZLStrongPtr < MOAIGfxScript > >::iterator gfxScriptIt = this->mGfxScripts.find ( renderPhase );
	return gfxScriptIt != this->mGfxScripts.end () ? ( MOAIGfxScript* )gfxScriptIt->second : NULL;
}

//----------------------------------------------------------------//
MOAIHasGfxScriptsForPhases::MOAIHasGfxScriptsForPhases ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ) {

	RTTI_BEGIN ( MOAIHasGfxScriptsForPhases )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScriptsForPhases >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptsForPhases::~MOAIHasGfxScriptsForPhases () {
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptsForPhases::SetGfxScript ( MOAIRenderPhaseEnum::_ renderPhase, MOAIGfxScript* gfxScript ) {

	if ( gfxScript ) {
		this->mGfxScripts [ renderPhase ] = gfxScript;
	}
	else {
		this->mGfxScripts.erase ( renderPhase );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxScriptsForPhases::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptsForPhases::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getGfxScript",				_getGfxScript },
		{ "gfx",						_gfx },
		{ "gfxScript",					_gfxScript },
		{ "setGfxScript",				_setGfxScript },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
