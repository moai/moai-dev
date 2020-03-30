// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTDESCRIPTORELEMENT_H
#define MOAIABSTRACTDESCRIPTORELEMENT_H

class MOAICommandBufferVK;

//================================================================//
// MOAIAbstractDescriptorElementVK
//================================================================//
class MOAIAbstractDescriptorElementVK :
	virtual public ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractDescriptorElementVK_GetPinnedData		( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer ) = 0;

public:
	
	//----------------------------------------------------------------//
	void GetPinnedData ( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer ) {
	
		this->MOAIAbstractDescriptorElementVK_GetPinnedData ( writeDescriptorSet, index, commandBuffer );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractDescriptorElementVK () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractDescriptorElementVK () {
	}
};

#endif
