// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>

//================================================================//
// MOAIGfxMgrVK_RenderTreeVK
//================================================================//

//----------------------------------------------------------------//
MOAIOneTriVK& MOAIGfxMgrVK_RenderTreeVK::AffirmOneTri () {

	if ( !this->mOneTri ) {
		this->mOneTri = new MOAIOneTriVK ();
	}
	return *this->mOneTri;
}

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

//	gfxMgr.Update ();

//	MOAIFrameBufferVK* frameBuffer = MOAICast < MOAIFrameBufferVK >( gfxMgr.GetDefaultFrameBuffer ());
//	assert ( frameBuffer );
//	frameBuffer->NeedsClear ( true );

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	gfxMgr.BeginFrame ();

//	if ( this->mRenderRoot ) {
//	
//		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//		state.Push ( this->mRenderRoot );
//		
//		MOAIDrawable::Draw ( state, -1 );
//	}
//	
//	// flush any stragglers
//	gfxMgr.FlushToGPU ();
	
	VkCommandBuffer commandBuffer	= gfxMgr.GetCommandBuffer ();
	VkRenderPass renderPass			= gfxMgr.GetRenderPass ();
	VkFramebuffer frameBuffer		= gfxMgr.GetFrameBuffer ();
	MOAISwapChainVK& swapChain		= gfxMgr.GetSwapChain ();
	
	u32 width = swapChain.GetWidth ();
	u32 height = swapChain.GetHeight ();

	VkCommandBufferBeginInfo cmdBufInfo = MOAIGfxStructVK::commandBufferBeginInfo ();
	vkBeginCommandBuffer ( commandBuffer, &cmdBufInfo );

	// Set clear values for all framebuffer attachments with loadOp set to clear
	// We use two attachments (color and depth) that are cleared at the start of the subpass and as such we need to set clear values for both
	VkClearValue clearValues [] = {
		MOAIGfxStructVK::clearValue ( MOAIGfxStructVK::clearColorValueF ( 0.0, 0.0, 0.2, 1.0 ), MOAIGfxStructVK::clearDepthStencilValue ( 0.0, 0 )),
		MOAIGfxStructVK::clearValue ( MOAIGfxStructVK::clearColorValueF ( 0.0, 0.0, 0.0, 0.0 ), MOAIGfxStructVK::clearDepthStencilValue ( 1.0, 0 )),
	};
	VkRenderPassBeginInfo renderPassBeginInfo = MOAIGfxStructVK::renderPassBeginInfo ( renderPass, frameBuffer, MOAIGfxStructVK::rect2D ( width, height ), clearValues, 2 );
	vkCmdBeginRenderPass ( commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

	this->AffirmOneTri ().Draw ( commandBuffer, width, height );

	vkCmdEndRenderPass ( commandBuffer );

	// Update dynamic state
	VkViewport viewport 	= MOAIGfxStructVK::viewport (( float )width, ( float )height, 0.0, 1.0 );
	VkRect2D scissor 		= MOAIGfxStructVK::rect2D ( width, height );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	VK_CHECK_RESULT ( vkEndCommandBuffer ( commandBuffer ));
	
	this->mRenderCounter++;
	
	gfxMgr.FinishFrame ();
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
}
