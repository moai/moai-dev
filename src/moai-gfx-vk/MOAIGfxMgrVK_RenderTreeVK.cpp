// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>

//================================================================//
// MOAIGfxMgrVK_RenderTreeVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK::MOAIGfxMgrVK_RenderTreeVK () {
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK::~MOAIGfxMgrVK_RenderTreeVK () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_RenderTreeVK::MOAIGfxMgr_RenderTree_Render () {

//	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
//
//	gfxMgr.ResetDrawingAPIs ();
//	gfxMgr.Update ();
//
//	MOAIFrameBufferVK* frameBuffer = MOAICast < MOAIFrameBufferVK >( gfxMgr.GetDefaultFrameBuffer ());
//	assert ( frameBuffer );
//	frameBuffer->NeedsClear ( true );
//
//	// Measure performance
//	double startTime = ZLDeviceTime::GetTimeInSeconds ();
//	
//	ZLGfx* gfx = gfxMgr.SelectDrawingAPI ( MOAIGfxMgrVK_PipelineClerkVK::DRAWING_PIPELINE );
//	if ( !gfx ) return;
//
//	ZGL_COMMENT ( *gfx, "RENDER MGR RENDER" );
//
//	if ( this->mRenderRoot ) {
//	
//		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//		state.Push ( this->mRenderRoot );
//		
//		MOAIDrawable::Draw ( state, -1 );
//	}
//	
//	this->mRenderCounter++;
//	
//	// Measure performance
//	double endTime = ZLDeviceTime::GetTimeInSeconds ();
//	this->mRenderDuration = endTime - startTime;
//	this->mRenderTime += this->mRenderDuration;
//	
//	gfxMgr.FinishFrame ();
}
