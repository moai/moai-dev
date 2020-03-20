// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEINPUTBODYVK_H
#define MOAIPIPELINEINPUTBODYVK_H

class MOAICommandBufferVK;
class MOAIPipelineInputChunkVK;
class MOAIGfxMgrVK;
class MOAIPipelineInputBodySchemaVK;

//================================================================//
// MOAIPipelineInputBodyVK
//================================================================//
class MOAIPipelineInputBodyVK :
	public virtual ZLRefCountedObject,
	public virtual ZLFinalizable {
protected:

	ZLStrongPtr < MOAIPipelineInputBodySchemaVK >				mPipelineLayout;
	ZLLeanArray < MOAIPipelineInputChunkVK >					mDescriptorSets;

public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineInputBodyComposerVK )
	
	//----------------------------------------------------------------//
	MOAIPipelineInputChunkVK&			GetDescriptorSet					( ZLIndex index );
	MOAIPipelineInputBodySchemaVK&		GetPipelineLayout					();
										MOAIPipelineInputBodyVK				();
										~MOAIPipelineInputBodyVK			();
	void								SetPipelineLayout					( MOAIPipelineInputBodySchemaVK& pipelineLayout );
};

#endif
