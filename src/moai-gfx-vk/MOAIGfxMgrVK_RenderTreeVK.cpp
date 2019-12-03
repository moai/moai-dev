// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDeckShaderOneTriVK.h>
#include <moai-gfx-vk/MOAIDynamicOneTriVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrOneTriVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>

//================================================================//
// MOAIGfxMgrVK_RenderTreeVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK::MOAIGfxMgrVK_RenderTreeVK () :
	mOneTri ( NULL ) {
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK::~MOAIGfxMgrVK_RenderTreeVK () {
}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_RenderTreeVK::MOAIGfxMgr_RenderTree_Render () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAICommandBufferVK& commandBuffer = gfxMgr.GetCommandBuffer ();

//	gfxMgr.Update ();

//	MOAIFrameBufferVK* frameBuffer = MOAICast < MOAIFrameBufferVK >( gfxMgr.GetDefaultFrameBuffer ());
//	assert ( frameBuffer );
//	frameBuffer->NeedsClear ( true );

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	gfxMgr.BeginFrame ();

	VkRenderPass renderPass = gfxMgr.GetRenderPass ();
	VkFramebuffer frameBuffer = gfxMgr.GetFrameBuffer ();
	MOAISwapChainVK& swapChain = gfxMgr.GetSwapChain ();

	// Set clear values for all framebuffer attachments with loadOp set to clear
	// We use two attachments (color and depth) that are cleared at the start of the subpass and as such we need to set clear values for both
	VkClearValue clearValues [] = {
		MOAIGfxStructVK::clearValue ( MOAIGfxStructVK::clearColorValueF ( 1.0, 1.0, 1.0, 1.0 )),
		MOAIGfxStructVK::clearValue ( MOAIGfxStructVK::clearDepthStencilValue ( 1.0, 0 )),
	};
	VkRenderPassBeginInfo renderPassBeginInfo = MOAIGfxStructVK::renderPassBeginInfo ( renderPass, frameBuffer, swapChain.GetRect (), clearValues, 2 );
	
	commandBuffer.Begin ();
	vkCmdBeginRenderPass ( commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );
	
	if ( !this->mOneTri ) {
		this->mOneTri = new MOAIGfxMgrOneTriVK ();
	}
	this->mOneTri->Draw ();
	
//	if ( this->mRenderRoot ) {
//		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//		state.Push ( this->mRenderRoot );
//		MOAIDrawable::Draw ( state, -1 );
//	}
	
	//	// flush any stragglers
	//	gfxMgr.FlushToGPU ();
	
	
	vkCmdEndRenderPass ( commandBuffer );
	commandBuffer.End ();

	commandBuffer.Submit ( gfxMgr.GetPresentSemaphore (), gfxMgr.GetRenderSemaphore ());
	
	gfxMgr.FinishFrame ();
	this->mRenderCounter++;
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
}
