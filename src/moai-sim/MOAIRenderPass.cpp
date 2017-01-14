// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIRenderPass.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRenderTable
	@text	Returns the table currently being used for rendering.
	
	@in		MOAIRenderPass self
	@out	table renderTable
*/
int MOAIRenderPass::_getRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderPass, "U" )
	state.Push ( self->mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setRenderTable
	@text	Sets the table to be used for rendering. This should be
			an array indexed from 1 consisting of MOAIRenderable objects
			and sub-tables. Objects will be rendered in order starting
			from index 1 and continuing until 'nil' is encountered.
	
	@in		MOAIRenderPass self
	@in		table renderTable
	@out	nil
*/
int MOAIRenderPass::_setRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderPass, "U" )
	self->mRenderTable.SetRef ( *self, state, 2 );
	return 0;
}

//================================================================//
// MOAIRenderPass
//================================================================//

//----------------------------------------------------------------//
MOAIRenderPass::MOAIRenderPass () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIRenderPassBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderPass::~MOAIRenderPass () {
}

//----------------------------------------------------------------//
void MOAIRenderPass::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIRenderPassBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIRenderPass::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIRenderPassBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getRenderTable",				_getRenderTable },
		{ "setRenderTable",				_setRenderTable },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderPass::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mGfxState.SetFrameBuffer ( this->GetFrameBuffer ());
	
	//disable scissor rect for clear
	gfxMgr.mGfxState.SetScissorRect ();
	this->ClearSurface ();
	
	MOAIDrawable::Draw ( this->mRenderTable );
		
	// restore the frame buffer
	gfxMgr.mGfxState.SetFrameBuffer ( this->GetFrameBuffer ());
}

