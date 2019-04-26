// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAITableLayer.h>

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
		RTTI_EXTEND ( MOAIAbstractLayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableLayer::~MOAITableLayer () {
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAbstractLayer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAbstractLayer::RegisterLuaFuncs ( state );

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
void MOAITableLayer::MOAIAbstractDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.SetFrameBuffer ( this->GetFrameBuffer ());
	
	//disable scissor rect for clear
	gfxMgr.SetScissorRect ();
	this->ClearSurface ();
	
	MOAIAbstractDrawable::Draw ( this->mRenderTable );
		
	// restore the frame buffer
	gfxMgr.SetFrameBuffer ( this->GetFrameBuffer ());
}

