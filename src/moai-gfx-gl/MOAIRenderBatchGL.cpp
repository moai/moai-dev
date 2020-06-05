// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIRenderBatchGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderBatchGL::_getFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBatchGL, "U" )
	state.Push ( self->GetFrameBuffer ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderBatchGL::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBatchGL, "U" )
	
	MOAIFrameBuffer* frameBuffer = state.GetLuaObject < MOAIFrameBuffer >( 2, true );
	self->SetFrameBuffer ( frameBuffer );
	return 0;
}

//================================================================//
// MOAIRenderBatch
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAIRenderBatchGL::GetFrameBuffer () {

	return this->mFrameBuffer ? this->mFrameBuffer : MOAIGfxMgr::Get ().GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIRenderBatchGL::MOAIRenderBatchGL () {

	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderBatchGL >)
		RTTI_EXTEND ( MOAISurfaceClearColor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderBatchGL::~MOAIRenderBatchGL () {
}

//----------------------------------------------------------------//
void MOAIRenderBatchGL::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->mFrameBuffer.Set ( *this, frameBuffer );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderBatchGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderBatchGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "setFrameBuffer",				_setFrameBuffer },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderBatchGL::MOAIRenderBatch_Render () {

	MOAIFrameBuffer* frameBuffer = this->GetFrameBuffer ();
	if ( !frameBuffer ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.SetFrameBuffer ( frameBuffer );
	
//	ZLRect bufferRect ( 0, 0, frameBuffer->GetBufferWidth (), frameBuffer->GetBufferHeight ());
//	gfxMgr.SetViewRect ( bufferRect );
//	gfxMgr.SetScissorRect ( bufferRect );

	gfxMgr.SetViewRect ();
	gfxMgr.SetScissorRect ();

	this->ClearSurface ();
	this->DrawRenderRoot ();
	
	gfxMgr.SetFrameBuffer ( this->GetFrameBuffer ()); // why is this here?
}
