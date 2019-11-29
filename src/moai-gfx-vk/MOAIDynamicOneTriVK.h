// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDYNAMICONETRIVK_H
#define MOAIDYNAMICONETRIVK_H

#include <moai-gfx-vk/MOAIGfxBufferVK.h>

class MOAICommandBufferVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetVK;
class MOAIPipelineLayoutVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITexture2DVK;
class MOAIUtilityBufferVK;

//================================================================//
// MOAIDynamicOneTriVK
//================================================================//
class MOAIDynamicOneTriVK :
	public virtual MOAIDrawable {
private:

	struct Vertex {
		ZLVec4D32	mPosition;
		ZLVec2D32 	mUV;
		u32			mRGBA32;
		
		Vertex ( float x, float y, float z, float r, float g, float b, float u, float v ) {
			this->mPosition.Init ( x, y, z, 1.0 );
			this->mUV.Init ( u, v );
			this->mRGBA32 = ZLColor::PackRGBA ( r, g, b, 1.0 );
		}
	};

	ZLStrongPtr < MOAIUtilityBufferVK >		mVertices;
	ZLStrongPtr < MOAIUtilityBufferVK >		mIndices;
	ZLStrongPtr < MOAIUniformBufferVK >		mUniforms;
	
	VkImage					mTextureImage;
	VkDeviceMemory			mTextureImageMemory;
	VkImageView				mTextureImageView;
	VkSampler				mTextureSampler;
	VkDescriptorImageInfo	mTextureDescriptor;
	
	uint32_t				mTotalIndices;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

//	ZLStrongPtr < MOAIPipelineLayoutVK >		mPipelineLayout;
	ZLStrongPtr < MOAIDescriptorSetVK >			mDescriptorSet;
	VkPipeline									mPipeline;

	//----------------------------------------------------------------//
	void			PreparePipeline				( MOAIPipelineLayoutVK& pipeline );
	void			PrepareTexture				();
	void			PrepareVertices				( bool useStagingBuffers = true );
	void			UpdateMatrices				( u32 width, u32 height );

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw			( int subPrimID );
	void			MOAIDrawable_DrawDebug		( int subPrimID );

public:

	DECL_LUA_FACTORY ( MOAIDynamicOneTriVK )

	//----------------------------------------------------------------//
					MOAIDynamicOneTriVK			();
					~MOAIDynamicOneTriVK		();
};

#endif
