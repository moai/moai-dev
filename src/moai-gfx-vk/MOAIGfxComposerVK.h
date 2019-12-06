// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXCOMPOSERVK_H
#define MOAIGFXCOMPOSERVK_H

class MOAICommandBufferVK;
class MOAIDescriptorSetVK;
class MOAIGfxComposerVK;
class MOAIPipelineLayoutVK;

//================================================================//
// MOAIGfxComposerCommandVK
//================================================================//
class MOAIGfxComposerCommandVK :
	public ZLRefCountedObject {
protected:

	friend class MOAIGfxComposerVK;

	ZLIndex			mDescriptorSetIndex;
	ZLIndex 		mBindPoint;
	ZLIndex 		mArrayItem;

	//----------------------------------------------------------------//
	virtual void		MOAIGfxComposerCommandVK_Apply			( MOAIDescriptorSetVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) = 0;

	//----------------------------------------------------------------//
	MOAIGfxComposerCommandVK ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem ) :
		mDescriptorSetIndex ( descriptorSetIndex ),
		mBindPoint ( bindPoint ),
		mArrayItem ( arrayItem ) {
	}

public:

	//----------------------------------------------------------------//
	virtual ~MOAIGfxComposerCommandVK () {
	}
	
	//----------------------------------------------------------------//
	void Apply ( MOAIDescriptorSetVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) {
	
		this->MOAIGfxComposerCommandVK_Apply ( descriptorSet, commandBuffer );
	}
};

//================================================================//
// MOAIGfxComposerTextureCommandVK
//================================================================//
class MOAIGfxComposerTextureCommandVK :
	public MOAIGfxComposerCommandVK {
protected:

	friend class MOAIGfxComposerVK;

	ZLIndex			mTextureUnit;

	//----------------------------------------------------------------//
	void		MOAIGfxComposerCommandVK_Apply		( MOAIDescriptorSetVK& descriptorSet, MOAICommandBufferVK& commandBuffer );
	
	//----------------------------------------------------------------//
				MOAIGfxComposerTextureCommandVK 	( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit );
};


//================================================================//
// MOAIGfxComposerVK
//================================================================//
class MOAIGfxComposerVK :
	public virtual ZLRefCountedObject,
	public virtual ZLAbstractFinalizable {
protected:

	ZLStrongPtr < MOAIPipelineLayoutVK >							mPipelineLayout;
	ZLLeanArray < MOAIDescriptorSetVK >								mDescriptorSets;
	ZLLeanArray < ZLStrongPtr < MOAIGfxComposerCommandVK > > 		mCommands;
	ZLSize															mCommandCount;

public:

	IMPLEMENT_FINALIZABLE ( MOAIGfxComposerVK )
	
	//----------------------------------------------------------------//
	void						ApplyAndBind						( MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint );
	MOAIDescriptorSetVK&		GetDescriptorSet					( ZLIndex index );
	MOAIPipelineLayoutVK&		GetPipelineLayout					();
								MOAIGfxComposerVK					();
								~MOAIGfxComposerVK					();
	void						PushTextureCommand					( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit );
	void						Reserve								( ZLSize size );
	void						SetPipelineLayout					( MOAIPipelineLayoutVK& pipelineLayout );
};

#endif
