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
int MOAIAbstractRenderNode::_hash ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )
	
	state.Push ( ZLHashFNV1a::Hash32 ( state.GetValue < cc8* >( 1, "" )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderNode::_localScope ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	self->AffirmLocalScope ();
	self->mLocalScope.PushRef ( state );
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
int MOAIAbstractRenderNode::_setSharedScope ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	MOAIScope* scope = state.GetLuaObject < MOAIScope >( 2, false );
	self->mSharedScope.Set ( *self, scope );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderNode::_sharedScope ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractRenderNode, "U" )
	
	if ( self->mSharedScope ) {
		self->mSharedScope.PushRef ( state );
		return 1;
	}
	return 0;
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
void MOAIAbstractRenderNode::AffirmLocalScope () {

	if ( !this->mLocalScope ) {
		this->mLocalScope.Set ( *this, new MOAIScope ());
	}
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIAbstractRenderNode::GetGfxScript ( u32 renderPhase ) {

	STLMap < u32, ZLStrongPtr < MOAIGfxScript > >::iterator gfxScriptIt = this->mGfxScripts.find ( renderPhase );
	return gfxScriptIt != this->mGfxScripts.end () ? ( MOAIGfxScript* )gfxScriptIt->second : NULL;
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

	this->mLocalScope.Set ( *this, NULL );
	this->mSharedScope.Set ( *this, NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::Render ( u32 renderPhase ) {

	if ( !this->LoadGfxState ( renderPhase )) return;
	
	if ( this->mSharedScope ) {
		this->mSharedScope->ScopeRetain ();
	}
	
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
	
	if ( this->mLocalScope ) {
		this->mLocalScope->Purge ();
	}
	
	if ( this->mSharedScope ) {
		this->mSharedScope->ScopeRelease ();
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

	state.SetField ( -1, "RENDER_PHASE_DRAW",			( u32 )RENDER_PHASE_DRAW );
	state.SetField ( -1, "RENDER_PHASE_DRAW_DEBUG",		( u32 )RENDER_PHASE_DRAW_DEBUG );
	
	luaL_Reg regTable [] = {
		{ "hash",						_hash },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getGfxScript",				_getGfxScript },
		{ "gfxScript",					_gfxScript },
		{ "localScope",					_localScope },
		{ "render",						_render },
		{ "setGfxScript",				_setGfxScript },
		{ "setSharedScope",				_setSharedScope },
		{ "sharedScope",				_sharedScope },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractRenderNode::MOAIAbstractRenderNode_LoadGfxState ( u32 renderPhase ) {
	UNUSED ( renderPhase );
	return true;
}
