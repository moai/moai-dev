// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSIGNATUREVK_H
#define MOAIDESCRIPTORSETSIGNATUREVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetVK;

//================================================================//
// MOAIDescriptorSetSignatureVK
//================================================================//
class MOAIDescriptorSetSignatureVK :
	public virtual ZLRefCountedObject,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAIDescriptorSetLayoutVK >,
	public ZLLeanArray < VkWriteDescriptorSet > {
private:

	friend class MOAIDescriptorSetLayoutVK;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*		GetWriteDescriptorSet 		( ZLIndex binding, ZLIndex arrayElement );
	
public:
	
	IMPLEMENT_FINALIZABLE ( MOAIDescriptorSetSignatureVK )
	
	//----------------------------------------------------------------//
	void			Initialize						( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
					MOAIDescriptorSetSignatureVK	();
					~MOAIDescriptorSetSignatureVK	();
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo );
};

#endif
