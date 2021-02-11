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
void MOAIGfxMgr_RenderTree::AffirmRenderRoot () {

	if ( !this->mRenderRoot ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		MOAIRenderNode* node = MOAIGfxMgr::Get ().CreateRenderRoot ();
		node->PushLuaUserdata ( state );
		this->mRenderRoot.SetRef ( state, -1 );
	}
}

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
void MOAIGfxMgr_RenderTree::PushRenderable ( MOAIAbstractRenderNode& node ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	node.PushLuaUserdata ( state );
	this->PushRenderable ( state, -1 );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::PushRenderable ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );

	state.Push ( this->mRenderRoot );
	if ( !state.IsType ( -1, LUA_TTABLE )) {
		
		MOAIRenderNode* node = state.GetLuaObject < MOAIRenderNode >( -1, false );
		
		if ( !node ) {
			state.Pop ();
			this->AffirmRenderRoot ();
			state.Push ( this->mRenderRoot );
		}
	}

	switch (  lua_type ( state, -1 )) {

		case LUA_TUSERDATA: {
			MOAIRenderNode* node = state.GetLuaObject < MOAIRenderNode >( -1, false );
			node->PushChild ( state, idx );
			break;
		}
		
		case LUA_TTABLE: {
			int top = ( int )state.GetTableSize ( -1 );
	
			lua_pushnumber ( state, top + 1 );
			lua_pushvalue ( state, idx );
			lua_settable ( state, -3 );
			break;
		}
		
		default:
			assert ( false );
	}
	state.Pop ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::Render () {

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mRenderRoot );
	MOAIRenderNode::Render ( MOAIRenderPhaseEnum::RENDER_PHASE_DRAW, state, -1 );

	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	this->mRenderCounter++;
}
