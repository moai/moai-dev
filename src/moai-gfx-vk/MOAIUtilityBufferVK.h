// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUTILITYBUFFERVK_H
#define	MOAIUTILITYBUFFERVK_H

#include <moai-gfx-vk/MOAILogicalDeviceClientVK.h>

class MOAILogicalDeviceVK;
class MOAIVertexFormatVK;

//================================================================//
// MOAIUtilityBufferVK
//================================================================//
// TODO: doxygen
class MOAIUtilityBufferVK :
	public ZLRefCountedObject,
	public MOAILogicalDeviceClientVK {
protected:

	VkBuffer				mBuffer;
	VkDeviceMemory			mMemory;
	VkDeviceSize			mAllocationSize;
	VkBufferUsageFlags		mUsage;
	VkMemoryPropertyFlags	mMemPropFlags;
	
	VkDescriptorBufferInfo	mDescriptor;
	
	//----------------------------------------------------------------//
	void				MOAIAbstractLifecycleClientVK_Finalize			();

public:
	
	static const VkMemoryPropertyFlags HOST_BUFFER_PROPS = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	static const VkMemoryPropertyFlags DEVICE_BUFFER_PROPS = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	GET ( VkBuffer&, Buffer, this->mBuffer );
	
	//----------------------------------------------------------------//
	operator VkBuffer () {
		return this->mBuffer;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorBufferInfo* () {
		return &this->mDescriptor;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorBufferInfo& () {
		return this->mDescriptor;
	}
	
	//----------------------------------------------------------------//
	void				Initialize					( MOAILogicalDeviceVK& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags = HOST_BUFFER_PROPS );
	void				MapAndCopy					( const void* data, size_t size );
						MOAIUtilityBufferVK			();
						~MOAIUtilityBufferVK		();
};

#endif
