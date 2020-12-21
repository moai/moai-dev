// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractLayer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractLayer::_pushRenderPass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractLayer, "U" )
	MOAIGfxMgr::Get ().PushRenderNode ( *self );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractLayer::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UU" )

	self->mFrameBuffer.Set ( *self, state.GetLuaObject < MOAIFrameBuffer >( 2, true ));

	return 0;
}

//================================================================//
// MOAIAbstractLayer
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractLayer::MOAIAbstractLayer () {
	
	this->mClearFlags = 0; // no clear on default
	
	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractLayer >)
		RTTI_EXTEND ( MOAIAbstractRenderNode )
		RTTI_EXTEND ( MOAISurfaceClearColor )
		RTTI_EXTEND ( MOAIColor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractLayer::~MOAIAbstractLayer () {

	this->mFrameBuffer.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractLayer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractLayer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "pushRenderPass",				_pushRenderPass },
		{ "setFrameBuffer",				_setFrameBuffer },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractLayer::MOAIAbstractRenderNode_LoadGfxState ( MOAIRenderPhaseEnum::_ renderPhase ) {

	if ( !this->MOAIAbstractRenderNode::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) return false;
	if ( this->IsClear ()) return false;
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	// TODO: change this to a push/pop
	if ( this->mFrameBuffer ) {
		gfxMgr.SetFrameBuffer ( this->mFrameBuffer );
	}
	gfxMgr.SetAmbientColor ( this->mColor );
	
	return true;
}
