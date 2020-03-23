// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEINPUTBODYCOMPOSERVK_H
#define MOAIPIPELINEINPUTBODYCOMPOSERVK_H

#include <moai-gfx-vk/MOAIPipelineInputBodyVK.h>

class MOAICommandBufferVK;
class MOAIPipelineInputChunkVK;
class MOAIPipelineInputBodyComposerVK;
class MOAIGfxMgrVK;
class MOAIPipelineInputBodySchemaVK;

//================================================================//
// MOAIGfxScriptCommandVK
//================================================================//
class MOAIGfxScriptCommandVK :
	public ZLRefCountedObject {
protected:

	friend class MOAIPipelineInputBodyComposerVK;

	ZLIndex			mDescriptorSetIndex;
	ZLIndex 		mBindPoint;
	ZLIndex 		mArrayItem;

	//----------------------------------------------------------------//
	virtual void		MOAIGfxScriptCommandVK_Apply			( MOAIGfxMgrVK& gfxMgr, MOAIPipelineInputChunkVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) = 0;

	//----------------------------------------------------------------//
	MOAIGfxScriptCommandVK ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem ) :
		mDescriptorSetIndex ( descriptorSetIndex ),
		mBindPoint ( bindPoint ),
		mArrayItem ( arrayItem ) {
	}

public:

	//----------------------------------------------------------------//
	virtual ~MOAIGfxScriptCommandVK () {
	}
	
	//----------------------------------------------------------------//
	void Apply ( MOAIGfxMgrVK& gfxMgr, MOAIPipelineInputChunkVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) {
	
		this->MOAIGfxScriptCommandVK_Apply ( gfxMgr, descriptorSet, commandBuffer );
	}
};

//================================================================//
// MOAIGfxScriptTextureCommandVK
//================================================================//
class MOAIGfxScriptTextureCommandVK :
	public MOAIGfxScriptCommandVK {
protected:

	friend class MOAIPipelineInputBodyComposerVK;

	ZLIndex		mTextureUnit;

	//----------------------------------------------------------------//
	void		MOAIGfxScriptCommandVK_Apply		( MOAIGfxMgrVK& gfxMgr, MOAIPipelineInputChunkVK& descriptorSet, MOAICommandBufferVK& commandBuffer );
	
	//----------------------------------------------------------------//
				MOAIGfxScriptTextureCommandVK 	( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit );
};

//================================================================//
// MOAIPipelineInputBodyComposerVK
//================================================================//
class MOAIPipelineInputBodyComposerVK :
	public virtual MOAIPipelineInputBodyVK {
protected:

	ZLLeanArray < ZLStrongPtr < MOAIGfxScriptCommandVK > >	mCommands;
	ZLSize														mCommandCount;

public:
	
	//----------------------------------------------------------------//
	void				ComposeAndBind						( MOAIGfxMgrVK& gfxMgr, MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint );
						MOAIPipelineInputBodyComposerVK		();
						~MOAIPipelineInputBodyComposerVK	();
	void				PushTextureCommand					( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit );
	void				Reserve								( ZLSize size );
};

#endif
