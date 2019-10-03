// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIUtilityBufferVK.h>

//================================================================//
// MOAIUtilityBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAIUtilityBufferVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags ) {

	this->SetProvider < MOAILogicalDeviceVK >( logicalDevice );

	VkBufferCreateInfo vertexBufferInfo = MOAIGfxStructVK::bufferCreateInfo ( size, usage );
	VK_CHECK_RESULT ( vkCreateBuffer ( logicalDevice, &vertexBufferInfo, NULL, &this->mBuffer ));
	
	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements ( logicalDevice, this->mBuffer, &memReqs );
	
	this->mAllocationSize = memReqs.size;
	this->mMemPropFlags = memPropFlags;
	this->mUsage = usage;
	
	VkMemoryAllocateInfo memoryAllocateInfo = MOAIGfxStructVK::memoryAllocateInfo (
		memReqs.size,
		MOAIGfxUtilVK::GetMemoryTypeIndex (
			memReqs.memoryTypeBits,
			logicalDevice.GetProvider < MOAIPhysicalDeviceVK >().mMemoryProperties,
			HOST_BUFFER_PROPS
		)
	);
	VK_CHECK_RESULT ( vkAllocateMemory ( logicalDevice, &memoryAllocateInfo, NULL, &this->mMemory ));
	VK_CHECK_RESULT ( vkBindBufferMemory ( logicalDevice, this->mBuffer, this->mMemory, 0 ));
	
	// Store information in the uniform's descriptor that is used by the descriptor set
	this->mDescriptor.buffer = this->mBuffer;
	this->mDescriptor.offset = 0;
	this->mDescriptor.range = size;
}

//----------------------------------------------------------------//
void MOAIUtilityBufferVK::MapAndCopy ( const void* data, size_t size ) {

	assert ( this->mMemPropFlags & HOST_BUFFER_PROPS );

	MOAILogicalDeviceVK& logicalDevice = this->GetProvider < MOAILogicalDeviceVK >();

	void* mappedAddr;
	VK_CHECK_RESULT ( vkMapMemory ( logicalDevice, this->mMemory, 0, this->mAllocationSize, 0, &mappedAddr ));
	memcpy ( mappedAddr, data, size );
	vkUnmapMemory ( logicalDevice, this->mMemory );
}

//----------------------------------------------------------------//
MOAIUtilityBufferVK::MOAIUtilityBufferVK () :
	mBuffer ( VK_NULL_HANDLE ),
	mMemory ( VK_NULL_HANDLE ),
	mAllocationSize ( 0 ),
	mUsage ( 0 ),
	mMemPropFlags ( 0 ) {
}

//----------------------------------------------------------------//
MOAIUtilityBufferVK::~MOAIUtilityBufferVK () {

	if ( this->HasProvider < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetProvider < MOAILogicalDeviceVK >();

		vkDestroyBuffer ( logicalDevice, this->mBuffer, NULL );
		vkFreeMemory ( logicalDevice, this->mMemory, NULL );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
