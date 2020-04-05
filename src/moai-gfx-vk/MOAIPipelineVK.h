// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEVK_H
#define MOAIPIPELINEVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxConstsVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIPipelineParamsVK
//================================================================//
class MOAIPipelineParamsVK {
public:

	MOAILogicalDeviceVK* 	mLogicalDevice;
	VkRenderPass* 			mRenderPass;
	VkPrimitiveTopology 	mTopology;
	MOAIVertexFormatVK* 	mVertexFormat;
	MOAIShaderVK* 			mShader;
	MOAIBlendMode* 			mBlendMode;

	//----------------------------------------------------------------//
	bool operator < ( const MOAIPipelineParamsVK& other ) const {
	
		return ( memcmp ( this, &other, sizeof ( MOAIPipelineParamsVK )) < 0 );
	}

	//----------------------------------------------------------------//
	MOAIPipelineParamsVK (
		MOAILogicalDeviceVK& logicalDevice,
		VkRenderPass& renderPass,
		VkPrimitiveTopology topology,
		MOAIVertexFormatVK* vertexFormat,
		MOAIShaderVK* shader,
		MOAIBlendMode* blendMode = NULL
	) {
		this->mLogicalDevice	= &logicalDevice;
		this->mRenderPass		= &renderPass;
		this->mTopology			= topology;
		this->mVertexFormat		= vertexFormat;
		this->mShader			= shader;
		this->mBlendMode		= blendMode;
	}
};

//================================================================//
// MOAIPipelineVK
//================================================================//
class MOAIPipelineVK :
	public MOAIAbstractSnapshotVK,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
protected:

	VkPipeline		mPipeline;

	//----------------------------------------------------------------//
	void _Finalize () {
		
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyPipeline ( logicalDevice, this->mPipeline, NULL );
	}

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		() {}

public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineVK )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mPipeline != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkPipeline* () {
		return &this->mPipeline;
	}
	
	//----------------------------------------------------------------//
	operator VkPipeline& () {
		return this->mPipeline;
	}

	//----------------------------------------------------------------//
	void Initialize ( const MOAIPipelineParamsVK& params ) {
	
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
	MOAIPipelineVK () :
		mPipeline ( VK_NULL_HANDLE ) {
	}
	
	//----------------------------------------------------------------//
	~MOAIPipelineVK () {
	
		this->Destruct ();
	}
};

#endif
