// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIRenderNode.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_render ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	
	self->Render ();
	return 0;
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

	this->Draw ( 0xffffffff );
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
