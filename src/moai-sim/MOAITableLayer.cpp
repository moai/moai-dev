// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAITableLayer.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRenderTable
	@text	Returns the table currently being used for rendering.
	
	@in		MOAITableLayer self
	@out	table renderTable
*/
int MOAITableLayer::_getRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITableLayer, "U" )
	state.Push ( self->mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setRenderTable
	@text	Sets the table to be used for rendering. This should be
			an array indexed from 1 consisting of MOAIRenderable objects
			and sub-tables. Objects will be rendered in order starting
			from index 1 and continuing until 'nil' is encountered.
	
	@in		MOAITableLayer self
	@in		table renderTable
	@out	nil
*/
int MOAITableLayer::_setRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITableLayer, "U" )
	self->mRenderTable.SetRef ( *self, state, 2 );
	return 0;
}

//================================================================//
// MOAITableLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableLayer::MOAITableLayer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableLayer::~MOAITableLayer () {
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAILayer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAILayer::RegisterLuaFuncs ( state );

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
void MOAITableLayer::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.SetFrameBuffer ( this->GetFrameBuffer ());
	
	//disable scissor rect for clear
	gfxState.SetScissorRect ();
	this->ClearSurface ();
	
	MOAIDrawable::Draw ( this->mRenderTable );
		
	// restore the frame buffer
	gfxState.SetFrameBuffer ( this->GetFrameBuffer ());
}

