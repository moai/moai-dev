// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIAbstractRenderNode.h>
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIAbstractRenderNodeCallback
//================================================================//
class MOAIAbstractRenderNodeCallback :
	public MOAIAbstractGfxScriptCallback {
public:

	MOAIAbstractRenderNode*		mNode;
	u32							mRenderPhase;

	//----------------------------------------------------------------//
	void MOAIAbstractGfxScriptCallback_Call () {
	
		this->mNode->MOAIAbstractRenderNode_Render ( this->mRenderPhase );
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractRenderNode::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	u32 renderPhase = state.GetValue < u32 >( 2, RENDER_PHASE_DRAW );
	MOAIGfxScript* script = self->GetGfxScript ( renderPhase );
	state.Push ( script );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAbstractRenderNode::_gfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	u32 renderPhase = state.GetValue < u32 >( 2, RENDER_PHASE_DRAW );
	MOAIGfxScript* script = self->AffirmGfxScript ( renderPhase );
	state.Push ( script );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderNode::_render ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	u32 renderPhase = state.GetValue < u32 >( 2, RENDER_PHASE_DRAW );
	self->Render ( renderPhase );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractRenderNode::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	u32 renderPhase = state.GetValue < u32 >( 2, RENDER_PHASE_DRAW );
	MOAIGfxScript* script = state.GetLuaObject < MOAIGfxScript >( 3, false );
	self->SetGfxScript ( renderPhase, script );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderNode::_setScope ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	MOAIScope* scope = state.GetLuaObject < MOAIScope >( 2, false );
	self->mScope.Set ( *self, scope );
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIAbstractRenderNode
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScript* MOAIAbstractRenderNode::AffirmGfxScript ( u32 renderPhase ) {

	MOAIGfxScript* gfxScript = this->GetGfxScript ( renderPhase );
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScript ();
		this->mGfxScripts [ renderPhase ] = gfxScript;
	}
	return gfxScript;
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIAbstractRenderNode::GetGfxScript ( u32 renderPhase ) {

	STLMap < u32, ZLStrongPtr < MOAIGfxScript > >::iterator gfxScriptIt = this->mGfxScripts.find ( renderPhase );
	return gfxScriptIt != this->mGfxScripts.end () ? ( MOAIGfxScript* )gfxScriptIt->second : NULL;
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::InvokeGfxScript ( u32 renderPhase ) {

	MOAIGfxScript* gfxScript = this->GetGfxScript ( renderPhase );
	
	if ( gfxScript ) {
		MOAIAbstractRenderNodeCallback callback;
		callback.mNode = this;
		callback.mRenderPhase = renderPhase;
		gfxScript->ExecuteBytecode ( &callback );
	}
	else {
		this->MOAIAbstractRenderNode_Render ( renderPhase );
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractRenderNode::LoadGfxState ( u32 renderPhase ) {

	return this->MOAIAbstractRenderNode_LoadGfxState ( renderPhase );
}

//----------------------------------------------------------------//
MOAIAbstractRenderNode::MOAIAbstractRenderNode () {

	RTTI_BEGIN ( MOAIAbstractRenderNode )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractRenderNode >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractRenderNode::~MOAIAbstractRenderNode () {
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::Render ( u32 renderPhase ) {

	if ( this->LoadGfxState ( renderPhase )) {
		this->InvokeGfxScript ( renderPhase );
	}
	
	if ( this->mScope ) {
		this->mScope->Purge ();
	}
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::SetGfxScript ( u32 renderPhase, MOAIGfxScript* gfxScript ) {

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
void MOAIAbstractRenderNode::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "render",						_render },
		{ "setScope",					_setScope },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractRenderNode::MOAIAbstractRenderNode_LoadGfxState ( u32 renderPhase ) {
	UNUSED ( renderPhase );
	return true;
}
