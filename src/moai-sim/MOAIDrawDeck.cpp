// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDrawDeck.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawDeck::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )

	int top = state.GetTop ();

	switch ( top ) {
	
		case 5:
			self->mBounds.Init ( state.GetRect < float >( 2 ));
			break;
	
		case 7:
			self->mBounds.Init ( state.GetBox ( 2 ));
			break;
			
		default:
			self->mBounds = ZLBounds::GLOBAL;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBoundsCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( number index ).

	@in		MOAIDrawDeck self
	@in		function callback
	@out	nil
*/
int MOAIDrawDeck::_setBoundsCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )
	
	self->mOnBounds.SetRef ( *self, state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDrawCallback
	@text	Sets the callback to be issued when draw events occur.
			The callback's parameters are ( number index, number xOff, number yOff, number xScale, number yScale ).

	@in		MOAIDrawDeck self
	@in		function callback
	@out	nil
*/
int MOAIDrawDeck::_setDrawCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDrawDeck, "U" )
	
	self->mOnDraw.SetRef ( *self, state, 2 );
	return 0;
}

//================================================================//
// MOAIDrawDeck
//================================================================//

//----------------------------------------------------------------//
MOAIDrawDeck::MOAIDrawDeck () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->mBounds = ZLBounds::GLOBAL;
}

//----------------------------------------------------------------//
MOAIDrawDeck::~MOAIDrawDeck () {
}

//----------------------------------------------------------------//
void MOAIDrawDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDrawDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setBounds",				_setBounds },
		{ "setBoundsCallback",		_setBoundsCallback },
		{ "setDrawCallback",		_setDrawCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDrawDeck::MOAIDeck_ComputeMaxBounds () {

	return this->mBounds;
}

//----------------------------------------------------------------//
void MOAIDrawDeck::MOAIDeck_Draw ( u32 idx ) {
	
	if ( this->mOnDraw ) {
	
		MOAIMaterialMgr& materialStack = MOAIMaterialMgr::Get ();
		materialStack.LoadGfxState ();
	
		MOAIDraw::Get ().Bind ();
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->mOnDraw.PushRef ( state );
		lua_pushnumber ( state, idx + 1 );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
ZLBounds MOAIDrawDeck::MOAIDeck_GetBounds ( u32 idx ) {
	
	if ( this->mOnBounds ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		
		int top = state.GetTop ();
		
		this->mOnBounds.PushRef ( state );
		
		lua_pushnumber ( state, idx );
		state.DebugCall ( 1, 6 );
		
		ZLBounds bounds;
		bounds.Init ( state.GetBox ( -6 ));
		return bounds;
	}
	return this->mBounds;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDrawDeck::MOAIDeck_GetCollisionShape ( u32 idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
bool MOAIDrawDeck::MOAIDeck_Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	return false;
}

//----------------------------------------------------------------//
bool MOAIDrawDeck::MOAIDeck_Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	return false;
}
