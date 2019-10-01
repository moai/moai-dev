// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIONETRIVK_H
#define MOAIONETRIVK_H

class MOAICommandBufferVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetVK;
class MOAIGfxBufferVK;
class MOAIPipelineLayoutVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITexture2DVK;

//================================================================//
// MOAIOneTriVK
//================================================================//
class MOAIOneTriVK :
	public virtual ZLRefCountedObject {
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

	ZLStrongPtr < MOAIVertexBufferVK >		mVertices;
	ZLStrongPtr < MOAIIndexBufferVK >		mIndices;
	ZLStrongPtr < MOAIUniformBufferVK >		mUniforms;
		
	VkImage					mTextureImage;
	VkDeviceMemory			mTextureImageMemory;
	VkImageView				mTextureImageView;
	VkSampler				mTextureSampler;
	VkDescriptorImageInfo	mTextureDescriptor;
	
	uint32_t				mTotalIndices;
	VkDescriptorBufferInfo	mUniformsDescriptor;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	ZLStrongPtr < MOAIPipelineLayoutVK >		mPipelineLayout;
	ZLStrongPtr < MOAIDescriptorSetVK >			mDescriptorSet;
	VkPipeline									mPipeline;

	//----------------------------------------------------------------//
	void			PreparePipeline				();
	void			PrepareTexture				();
	void			PrepareUniformBuffers		();
	void			PrepareVertices				( bool useStagingBuffers = true );
	void			UpdateMatrices				( u32 width, u32 height );

public:

	//----------------------------------------------------------------//
	void			Draw						( MOAICommandBufferVK& commandBuffer, VkRect2D rect );
					MOAIOneTriVK				();
					~MOAIOneTriVK				();
};

#endif
