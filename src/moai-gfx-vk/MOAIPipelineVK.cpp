// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIPipelineCacheVK.h>
#include <moai-gfx-vk/MOAIPipelineVK.h>

//================================================================//
// MOAIPipelineVK
//================================================================//

//----------------------------------------------------------------//
MOAIPipelineVK::MOAIPipelineVK ( const MOAIPipelineParamsVK& params ) :
	MOAIPipelineParamsVK ( params ),
	mCache ( NULL ),
	mPage ( NULL ),
	mKey ( *this ),
	mPipeline ( VK_NULL_HANDLE ) {
	
	this->SetDependency < MOAILogicalDeviceVK >( *params.mLogicalDevice );

	VkDynamicState dynamicStateEnables [] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineColorBlendAttachmentState blendAttachmentState;

	if ( params.mBlendMode ) {

		VkBlendOp blendOp			= MOAIGfxConstsVK::Remap ( params.mBlendMode->mEquation );
		VkBlendFactor srcFactor		= MOAIGfxConstsVK::Remap ( params.mBlendMode->mSourceFactor );
		VkBlendFactor dstFactor		= MOAIGfxConstsVK::Remap ( params.mBlendMode->mDestFactor );

		blendAttachmentState = MOAIGfxStructVK::pipelineColorBlendAttachmentState (
			VK_TRUE,
			blendOp,
			srcFactor,
			dstFactor,
			blendOp,
			srcFactor,
			dstFactor
		);
	}
	else {
		blendAttachmentState = MOAIGfxStructVK::pipelineColorBlendAttachmentState ();
	}

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= MOAIGfxStructVK::pipelineInputAssemblyStateCreateInfo ( params.mTopology );
	VkPipelineRasterizationStateCreateInfo rasterizationState 	= MOAIGfxStructVK::pipelineRasterizationStateCreateInfo ();
	VkPipelineColorBlendStateCreateInfo colorBlendState 		= MOAIGfxStructVK::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used)
	VkPipelineViewportStateCreateInfo viewportState 			= MOAIGfxStructVK::pipelineViewportStateCreateInfo (); // overridden by dynamic state
	VkPipelineDepthStencilStateCreateInfo depthStencilState 	= MOAIGfxStructVK::pipelineDepthStencilStateCreateInfo ();
	VkPipelineMultisampleStateCreateInfo multisampleState 		= MOAIGfxStructVK::pipelineMultisampleStateCreateInfo ();
	VkPipelineDynamicStateCreateInfo dynamicState				= MOAIGfxStructVK::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = MOAIGfxStructVK::graphicsPipelineCreateInfo (
		NULL,
		0,
		NULL,
		&inputAssemblyState,
		NULL,
		&viewportState,
		&rasterizationState,
		&multisampleState,
		&depthStencilState,
		&colorBlendState,
		&dynamicState,
		NULL,
		*params.mRenderPass
	);

	params.mVertexFormat->UpdatePipelineCreateInfo ( pipelineCreateInfo );
	params.mShader->GetProgram ()->UpdatePipelineCreateInfo ( pipelineCreateInfo );

	// Create rendering pipeline using the specified states
	VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( *params.mLogicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->mPipeline ));
}

//----------------------------------------------------------------//
MOAIPipelineVK::~MOAIPipelineVK () {
	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineVK::_Finalize () {
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	vkDestroyPipeline ( logicalDevice, this->mPipeline, NULL );
}

//----------------------------------------------------------------//
void MOAIPipelineVK::MOAIAbstractSnapshotVK_OnPin (
	MOAICommandBufferVK& commandBuffer ) { UNUSED ( commandBuffer );
}

//----------------------------------------------------------------//
void MOAIPipelineVK::MOAIAbstractSnapshotVK_OnUnpin () {
}

//----------------------------------------------------------------//
MOAIPipelineVK* MOAIPipelineVK::MOAISnapshotFactoryVK_GetSnapshot () {

	return this;
}
