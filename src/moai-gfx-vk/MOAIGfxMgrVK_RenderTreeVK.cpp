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

//	if ( !this->mOneTri ) {
//		this->mOneTri = new MOAIGfxMgrOneTriVK ();
//	}
//	this->mOneTri->Draw ();
	
	this->RenderBatchOrRoot ();
}
