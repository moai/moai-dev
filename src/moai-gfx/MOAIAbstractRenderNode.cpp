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
int MOAIAbstractRenderNode::_getRenderTypeID ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )

	state.Push ( ZLType::GetID < MOAIAbstractRenderNode >());
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
	
	MOAIRenderPhaseEnum::_ renderPhase = state.GetEnum ( 2, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	self->Render ( renderPhase );
	return 0;
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
void MOAIAbstractRenderNode::AffirmLocalScope () {

	if ( !this->mLocalScope ) {
		this->mLocalScope.Set ( *this, new MOAIScope ());
	}
}

//----------------------------------------------------------------//
MOAIAbstractRenderNode::MOAIAbstractRenderNode () {

	RTTI_BEGIN ( MOAIAbstractRenderNode )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractRenderNode >)
		RTTI_EXTEND ( MOAIHasGfxScriptsForPhases )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractRenderNode::~MOAIAbstractRenderNode () {

	this->mLocalScope.Set ( *this, NULL );
	this->mSharedScope.Set ( *this, NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::Render ( MOAIRenderPhaseEnum::_ renderPhase ) {

	if ( !this->MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) return;
	
	if ( this->mSharedScope ) {
		this->mSharedScope->ScopeRetain ();
	}
	
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
	
	if ( this->mLocalScope ) {
		this->mLocalScope->Purge ();
	}
	
	if ( this->mSharedScope ) {
		this->mSharedScope->ScopeRelease ();
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractRenderNode::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
void MOAIAbstractRenderNode::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "localScope",					_localScope },
		{ "render",						_render },
		{ "setSharedScope",				_setSharedScope },
		{ "sharedScope",				_sharedScope },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractRenderNode::MOAIAbstractRenderNode_LoadGfxState ( MOAIRenderPhaseEnum::_ renderPhase ) {
	UNUSED ( renderPhase );
	return true;
}
