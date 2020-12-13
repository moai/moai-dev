// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxMgr_RenderTree.h>
#include <moai-gfx/MOAIRenderNode.h>

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree::MOAIGfxMgr_RenderTree () :
	mRenderCounter ( 0 ),
	mRenderDuration ( 1.0 / 60.0 ),
	mRenderTime ( 0.0 ) {
}

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree::~MOAIGfxMgr_RenderTree () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::PushRenderNode ( MOAIAbstractRenderNode* node ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	if ( this->mRenderRoot ) {
		state.Push ( this->mRenderRoot );
	}
	else {
//		MOAIRenderNode* node =  MOAIGfxMgr::Get ().CreateRenderBatch ();
//		state.Push ( node );
//		this->mRenderRoot.SetRef ( state, -1 );
		lua_newtable ( state );
		this->mRenderRoot.SetRef ( state, -1 );
	}

	int idx = state.AbsIndex ( -1 );
	int valType = lua_type ( state, idx );

	switch ( valType ) {

//		case LUA_TUSERDATA: {
//			MOAIRenderNode* node = state.GetLuaObject < MOAIRenderNode >( idx, false );
//			if ( node ) {
//				node->PushChild ( node );
//			}
//			break;
//		}

		case LUA_TTABLE: {
			int top = ( int )state.GetTableSize ( -1 );
			state.SetField ( -1, top + 1, node );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::Render () {

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.BeginFrame ();
	
	gfxMgr.SetFrameBuffer ();
	gfxMgr.SetViewRect ();
	gfxMgr.SetScissorRect ();

	gfxMgr.SetClearColor ( 0x00000000 );
	gfxMgr.SetClearFlags ( MOAIClearFlagsEnum::COLOR_BUFFER_BIT | MOAIClearFlagsEnum::DEPTH_BUFFER_BIT );
	gfxMgr.ClearSurface ();
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mRenderRoot );
	MOAIRenderNode::Render ( MOAIAbstractRenderNode::RENDER_PHASE_DRAW, state, -1, NULL );
	
	gfxMgr.EndFrame ();
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	this->mRenderCounter++;
}
