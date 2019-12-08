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
void MOAIGfxBufferSnapshotVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, MOAIGfxBufferVK& buffer, bool useStagingBuffers ) {

	ZLSize size = buffer.GetSize ();
	
	if ( useStagingBuffers ) {
		
		// Static data like vertex and index buffer should be stored on the device memory
		// for optimal (and fastest) access by the GPU

		MOAIGfxBufferSnapshotVK staging;
		staging.Initialize ( logicalDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
		staging.MapAndCopy ( buffer.GetConstBuffer (), size );

		this->Initialize ( logicalDevice, size, buffer.mUsage | VK_BUFFER_USAGE_TRANSFER_DST_BIT, MOAIGfxBufferSnapshotVK::DEVICE_BUFFER_PROPS );
		
		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		MOAIQueueVK& graphicsQueue = logicalDevice.GetGraphicsQueue ();
		
		MOAICommandBufferVK commandBuffer;
		graphicsQueue.CreateCommandBuffer ( commandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY, true );

		// Put buffer region copies into command buffer
		VkBufferCopy copyRegion = {};

		// Vertex buffer
		copyRegion.size = size;
		vkCmdCopyBuffer ( commandBuffer, staging, *this, 1, &copyRegion );

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		graphicsQueue.FlushAndFreeCommandBuffer ( commandBuffer );
	}
	else {
	
		this->Initialize (
			logicalDevice,
			size,
			buffer.mUsage,
			HOST_BUFFER_PROPS
		);
		
		this->MapAndCopy ( buffer.GetConstBuffer (), size );
	}
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
