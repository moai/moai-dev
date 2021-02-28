// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIAbstractRenderable.h>
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIAbstractRenderNodeCallback
//================================================================//
class MOAIAbstractRenderNodeCallback :
	public MOAIAbstractGfxScriptCallback {
public:

	MOAIAbstractRenderable*		mNode;
	MOAIRenderPhaseEnum::_		mRenderPhase;

	//----------------------------------------------------------------//
	void MOAIAbstractGfxScriptCallback_Call () {
	
		this->mNode->MOAIAbstractRenderNode_Render ( this->mRenderPhase );
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractRenderable::_getRenderTypeID ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )

	state.Push ( ZLType::GetID < MOAIAbstractRenderable >());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderable::_hash ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )
	
	state.Push ( ZLHashFNV1a::Hash32 ( state.GetValue < cc8* >( 1, "" )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderable::_render ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderable, "U" )
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 2, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	self->Render ( renderPhase );
	return 0;
}

//================================================================//
// MOAIAbstractRenderable
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractRenderable::MOAIAbstractRenderable ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIHasGfxScriptsForPhases ( context ) {

	RTTI_BEGIN ( MOAIAbstractRenderable )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractRenderable >)
		RTTI_EXTEND ( MOAIHasGfxScriptsForPhases )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractRenderable::~MOAIAbstractRenderable () {
}

//----------------------------------------------------------------//
void MOAIAbstractRenderable::Render ( MOAIRenderPhaseEnum::_ renderPhase ) {

	if ( !this->MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) return;
	
	MOAIGfxScript* gfxScript = this->GetGfxScript ( renderPhase );
	
	if ( gfxScript ) {
		MOAIAbstractRenderNodeCallback callback;
		callback.mNode = this;
		callback.mRenderPhase = renderPhase;
		gfxScript->ExecuteBytecode ( &callback );
		callback.Flush ();
	}
	else {
		this->MOAIAbstractRenderNode_Render ( renderPhase );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractRenderable::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetEnum ( -1, "RENDER_PHASE_DRAW",		MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	state.SetEnum ( -1, "RENDER_PHASE_DRAW_DEBUG",	MOAIRenderPhaseEnum::RENDER_PHASE_DRAW_DEBUG );
	
	luaL_Reg regTable [] = {
		{ "getRenderTypeID",			_getRenderTypeID },
		{ "hash",						_hash },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderable::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "render",						_render },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractRenderable::MOAIAbstractRenderNode_LoadGfxState ( MOAIRenderPhaseEnum::_ renderPhase ) {
	UNUSED ( renderPhase );
	return true;
}
