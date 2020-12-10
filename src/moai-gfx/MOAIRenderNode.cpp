// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIRenderNode.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_render ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	
	self->MOAIRenderNode_Render ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_setRender ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	
	self->mRenderRoot.SetRef ( state, 2 );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_setScope ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	
	MOAIScope* scope = state.GetLuaObject < MOAIScope >( 2, false );
	self->mScope.Set ( *self, scope );
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIRenderNode
//================================================================//

//----------------------------------------------------------------//
MOAIRenderNode::MOAIRenderNode () {

	RTTI_BEGIN ( MOAIRenderNode )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderNode >)
		RTTI_EXTEND ( MOAIAbstractDrawable )
		RTTI_EXTEND ( MOAIHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderNode::~MOAIRenderNode () {
}

//----------------------------------------------------------------//
void MOAIRenderNode::Render () {

	if ( this->mRenderRoot ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		state.Push ( this );
		MOAIDraw::Get ().PushCmdInterface ( state );
		state.Push ( *this->mScope );
		state.DebugCall ( 3, 0 );
	}
	else {
		this->MOAIRenderNode_Render ();
	}
	
	if ( this->mScope ) {
		this->mScope->Purge ();
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderNode::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderNode::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "render",						_render },
		{ "setRender",					_setRender },
		{ "setScope",					_setScope },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderNode::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
void MOAIRenderNode::MOAIDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
void MOAIRenderNode::MOAIRenderNode_Render () {

	this->Draw ( 0xffffffff );
}
