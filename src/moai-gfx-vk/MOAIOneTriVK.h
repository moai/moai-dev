// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIONETRIVK_H
#define MOAIONETRIVK_H

class MOAIOneTriBufferVK;

//================================================================//
// MOAIOneTriVK
//================================================================//
class MOAIOneTriVK :
	public virtual ZLRefCountedObject {
private:

	struct Vertex {
		float position [ 3 ];
		float color [ 3 ];
		
		Vertex ( float x, float y, float z, float r, float g, float b ) {
		
			position [ 0 ] = x;
			position [ 1 ] = y;
			position [ 2 ] = z;
			
			color [ 0 ] = r;
			color [ 1 ] = g;
			color [ 2 ] = b;
		}
	};

	ZLStrongPtr < MOAIOneTriBufferVK >	mVertices;
	ZLStrongPtr < MOAIOneTriBufferVK >	mIndices;
	ZLStrongPtr < MOAIOneTriBufferVK >	mUniforms;
	
	uint32_t				mTotalIndices;
	VkDescriptorBufferInfo	mUniformsDescriptor;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	VkPipelineLayout		mPipelineLayout;
	VkPipeline				mPipeline;
	VkDescriptorPool		mDescriptorPool;
	VkDescriptorSetLayout	mDescriptorSetLayout;
	VkDescriptorSet			mDescriptorSet;

	//----------------------------------------------------------------//
	void			PreparePipeline				();
	void			PrepareUniformBuffers		();
	void			PrepareVertices				( bool useStagingBuffers = true );
	void			SetupDescriptorPool			();
	void			SetupDescriptorSet			();
	void			SetupDescriptorSetLayout	();
	void			UpdateUniformBuffers		( u32 width, u32 height );

public:

	//----------------------------------------------------------------//
	void			Draw						( VkCommandBuffer& commandBuffer, u32 width, u32 height );
					MOAIOneTriVK				();
					~MOAIOneTriVK				();
};

#endif
