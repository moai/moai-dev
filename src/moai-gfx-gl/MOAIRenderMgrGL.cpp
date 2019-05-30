// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
#include <moai-gfx-gl/MOAIRenderMgrGL.h>

//================================================================//
// MOAIRenderMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIRenderMgrGL::MOAIRenderMgrGL () {
}

//----------------------------------------------------------------//
MOAIRenderMgrGL::~MOAIRenderMgrGL () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderMgrGL::MOAIAbstractRenderMgr_Render () {

	MOAIGfxMgrGL& gfxMgr = MOAIGfxMgrGL::Get ();

	gfxMgr.ResetDrawingAPIs ();
	gfxMgr.Update ();

	MOAIFrameBufferGL* frameBuffer = MOAICast < MOAIFrameBufferGL >( gfxMgr.GetDefaultFrameBuffer ());
	assert ( frameBuffer );
	frameBuffer->NeedsClear ( true );

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	ZLGfx* gfx = gfxMgr.SelectDrawingAPI ( MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
	if ( !gfx ) return;

	ZGL_COMMENT ( *gfx, "RENDER MGR RENDER" );

	if ( this->mRenderRoot ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		
		MOAIAbstractDrawable::Draw ( state, -1 );
	}
	
	this->mRenderCounter++;
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	gfxMgr.FinishFrame ();
}
