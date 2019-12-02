// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>

//================================================================//
// MOAIGfxBufferSnapshotVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferSnapshotVK::Initialize ( MOAIGfxBufferVK& buffer ) {

	MOAILogicalDeviceVK& logicalDevice = buffer.GetDependency < MOAILogicalDeviceVK >();

	// TODO: support staging buffers

	ZLSize size = buffer.GetSize ();

	this->MOAIGfxBufferSnapshotVK::Initialize (
		logicalDevice,
		size,
		buffer.mUsage,
		HOST_BUFFER_PROPS
	);
	
	this->MapAndCopy ( buffer.GetConstBuffer (), size );
}

//----------------------------------------------------------------//
void MOAIGfxBufferSnapshotVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );

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
			logicalDevice.GetDependency < MOAIPhysicalDeviceVK >().mMemoryProperties,
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
void MOAIGfxBufferSnapshotVK::MapAndCopy ( const void* data, size_t size ) {

	assert ( this->mMemPropFlags & HOST_BUFFER_PROPS );

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

	void* mappedAddr;
	VK_CHECK_RESULT ( vkMapMemory ( logicalDevice, this->mMemory, 0, this->mAllocationSize, 0, &mappedAddr ));
	memcpy ( mappedAddr, data, size );
	vkUnmapMemory ( logicalDevice, this->mMemory );
}

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK::MOAIGfxBufferSnapshotVK () :
	mBuffer ( VK_NULL_HANDLE ),
	mMemory ( VK_NULL_HANDLE ),
	mAllocationSize ( 0 ),
	mUsage ( 0 ),
	mMemPropFlags ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK::~MOAIGfxBufferSnapshotVK () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

		vkDestroyBuffer ( logicalDevice, this->mBuffer, NULL );
		vkFreeMemory ( logicalDevice, this->mMemory, NULL );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
