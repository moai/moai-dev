// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrOneTriVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIGfxMgrOneTriVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrOneTriVK::MOAIGfxMgrOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	MOAIImage image;
	image.Load ( "../resources/moai.png", ZLImageTransform::TRUECOLOR );
	image.Convert ( image );

	this->mTexture = new MOAITexture2DVK ();
	this->mTexture->Init ( image, "" );

	this->mVertices = new MOAIVertexBufferVK ();
	this->mVertices->Initialize ( logicalDevice, sizeof ( Vertex ) * 3 );

	this->mVertices->Write < Vertex >( Vertex (  1.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,	1.0, 0.0 ));
	this->mVertices->Write < Vertex >( Vertex ( -1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0, 0.0 ));
	this->mVertices->Write < Vertex >( Vertex (  0.0f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.5, 1.0 ));

	this->mIndices = new MOAIIndexBufferVK ();
	this->mIndices->Initialize ( logicalDevice, sizeof ( u32 ) * 3 );
	this->mIndices->SetIndexSize ( 4 );

	this->mIndices->Write < u32 >( 0 );
	this->mIndices->Write < u32 >( 1 );
	this->mIndices->Write < u32 >( 2 );
}

//----------------------------------------------------------------//
MOAIGfxMgrOneTriVK::~MOAIGfxMgrOneTriVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrOneTriVK::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxMgrVK& gfxMgr					= MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	MOAISwapChainVK& swapChain				= gfxMgr.GetSwapChain ();

	ZLRect rect = MOAIGfxStructVK::zlRect ( swapChain.GetRect ());

	gfxMgr.SetViewRect ( rect );
	gfxMgr.SetScissorRect ( rect );

	gfxMgr.SetShader ( DECK2D_SHADER );
	gfxMgr.SetTexture ( this->mTexture );
	gfxMgr.SetVertexFormat ( XYZWUVC );
	
	gfxMgr.SetVertexBuffer ( this->mVertices );
	gfxMgr.SetIndexBuffer ( this->mIndices );
	
	gfxMgr.DrawPrims ( MOAIGfxTopologyEnum::TRIANGLE_LIST, 0, this->mIndices->CountIndices ());
}

//----------------------------------------------------------------//
void MOAIGfxMgrOneTriVK::MOAIDrawable_DrawDebug ( int subPrimID ) {
}
