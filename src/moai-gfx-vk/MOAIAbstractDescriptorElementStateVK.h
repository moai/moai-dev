// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIABSTRACTDESCRIPTORELEMENTSTATE_H
#define MOAIABSTRACTDESCRIPTORELEMENTSTATE_H

class MOAICommandBufferVK;
class MOAIAbstractDescriptorElementVK;

//================================================================//
// MOAIMutableWriteDescriptorSetVK
//================================================================//
class MOAIMutableWriteDescriptorSetVK {
public:
	VkDescriptorImageInfo*     mImageInfo;
	VkDescriptorBufferInfo*    mBufferInfo;
	VkBufferView*              mTexelBufferView;
	
	//----------------------------------------------------------------//
	MOAIMutableWriteDescriptorSetVK () :
		mImageInfo ( NULL ),
		mBufferInfo ( NULL ),
		mTexelBufferView ( NULL ) {
	}
};

//================================================================//
// MOAIAbstractDescriptorElementStateVK
//================================================================//
class MOAIAbstractDescriptorElementStateVK :
	virtual public ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual MOAIAbstractDescriptorElementVK*		MOAIAbstractDescriptorElementStateVK_GetElement		( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index ) = 0;

public:
	
	//----------------------------------------------------------------//
	MOAIAbstractDescriptorElementVK* GetElement ( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index ) {
	
		return this->MOAIAbstractDescriptorElementStateVK_GetElement ( writeDescriptorSet, index );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractDescriptorElementStateVK () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractDescriptorElementStateVK () {
	}
};

#endif
