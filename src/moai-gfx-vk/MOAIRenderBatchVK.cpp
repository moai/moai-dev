// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIRenderBatchVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderBatchVK::_getFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBatchVK, "U" )
	state.Push ( self->GetFrameBuffer ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderBatchVK::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBatchVK, "U" )
	
	MOAIFrameBufferVK* frameBuffer = state.GetLuaObject < MOAIFrameBufferVK >( 2, true );
	self->SetFrameBuffer ( frameBuffer );
	return 0;
}

//================================================================//
// MOAIRenderBatch
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferVK* MOAIRenderBatchVK::GetFrameBuffer () {

	return this->mFrameBuffer ? this->mFrameBuffer : &MOAIGfxMgrVK::Get ().GetFrameBuffer ();
}

//----------------------------------------------------------------//
MOAIRenderPassVK* MOAIRenderBatchVK::GetRenderPass () {

	return this->mRenderPassVK ? this->mRenderPassVK : &MOAIGfxMgrVK::Get ().GetRenderPass ();
}

//----------------------------------------------------------------//
MOAIRenderBatchVK::MOAIRenderBatchVK () {

	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderBatchVK >)
		RTTI_EXTEND ( MOAISurfaceClearColor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderBatchVK::~MOAIRenderBatchVK () {
}

//----------------------------------------------------------------//
void MOAIRenderBatchVK::SetFrameBuffer ( MOAIFrameBufferVK* frameBuffer ) {

	this->mFrameBuffer.Set ( *this, frameBuffer );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderBatchVK::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderBatchVK::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "setFrameBuffer",				_setFrameBuffer },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderBatchVK::MOAIRenderBatch_Render () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAICommandBufferVK& commandBuffer = gfxMgr.GetCommandBuffer ();

	MOAIFrameBufferVK* frameBuffer = this->GetFrameBuffer ();
	if ( !frameBuffer ) return;
	
	MOAIFrameBufferSnapshotVK* frameBufferSnapshot = frameBuffer->GetSnapshot ();
	if ( !frameBufferSnapshot ) return;

	MOAIRenderPassVK* renderPass = frameBuffer->GetRenderPass ();
	if ( !renderPass ) return;
	
	ZLSize totalAttachments = renderPass->CountAttachments ();
	VkClearValue* clearValues = ( VkClearValue* )alloca ( renderPass->CountAttachments () + totalAttachments );
	
	ZLColorVec clearColor = this->GetClearColorVec ();
	
	for ( ZLIndex i = 0; i < totalAttachments; ++i ) {
		switch ( renderPass->GetAttachmentClearType ( i )) {
		
			case MOAIRenderPassVK::CLEAR_NONE:
				break;
			
			case MOAIRenderPassVK::CLEAR_COLOR:
				clearValues [ i ].color = MOAIGfxStructVK::clearColorValueF (
					clearColor.mR,
					clearColor.mG,
					clearColor.mB,
					clearColor.mA
				);
				break;
			
			case MOAIRenderPassVK::CLEAR_DEPTH_STENCIL:
				clearValues [ i ].depthStencil = MOAIGfxStructVK::clearDepthStencilValue ( 1.0, 0 );
				break;
		}
	}

	commandBuffer.Pin ( *frameBufferSnapshot );
	VkRenderPassBeginInfo renderPassBeginInfo = MOAIGfxStructVK::renderPassBeginInfo (
		*renderPass,
		*frameBufferSnapshot,
		gfxMgr.GetSwapChain ().GetRect (),
		clearValues,
		2
	);
	
	vkCmdBeginRenderPass ( commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

	//	if ( !this->mOneTri ) {
	//		this->mOneTri = new MOAIGfxMgrOneTriVK ();
	//	}
	//	this->mOneTri->Draw ();

	this->DrawRenderRoot ();
	gfxMgr.FlushToGPU ();
	
	vkCmdEndRenderPass ( commandBuffer );
}
