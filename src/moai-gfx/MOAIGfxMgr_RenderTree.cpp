// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxMgr_RenderTree.h>
#include <moai-gfx/MOAIRenderBatch.h>

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//

//----------------------------------------------------------------//
MOAIRenderBatch* MOAIGfxMgr_RenderTree::AffirmDefaultBatch () {

	if ( !this->mRenderBatch ) {
		this->mRenderBatch = MOAIGfxMgr::Get ().CreateRenderBatch ();
	}
	return this->mRenderBatch;
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
void MOAIGfxMgr_RenderTree::PushDrawable ( MOAILuaObject* drawable ) {

	if ( this->mRenderRoot ) {
		this->PushDrawable ( this->mRenderRoot, drawable );
	}
	else {
		if ( !this->mRenderBatch ) {
			this->mRenderBatch = MOAIGfxMgr::Get ().CreateRenderBatch ();
		}
		this->PushDrawable ( this->mRenderBatch->mRenderRoot, drawable );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::PushDrawable ( MOAILuaStrongRef& renderRoot, MOAILuaObject* drawable ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	state.Push ( renderRoot );
	
	if ( !state.IsType ( -1, LUA_TTABLE )) {
		state.Pop ();
		lua_newtable ( state );
		renderRoot.SetRef ( state, -1 );
	}
	
	int top = ( int )state.GetTableSize ( -1 );
	state.SetField ( -1, top + 1, drawable );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::Render () {

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.BeginFrame ();
	this->RenderBatchOrRoot ();
	gfxMgr.EndFrame ();
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	this->mRenderCounter++;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::RenderBatchOrRoot () {

	if ( this->mRenderRoot ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		MOAIAbstractDrawable::Draw ( state, -1 );
	}
	else if ( this->mRenderBatch ) {
		this->mRenderBatch->Render ();
	}
}
