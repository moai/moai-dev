// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIQueueVK.h>

//================================================================//
// MOAIQueueVK
//================================================================//

//----------------------------------------------------------------//
void MOAIQueueVK::CreateCommandBuffer ( MOAILogicalDeviceVK& logicalDevice, MOAICommandBufferVK& commandBuffer, bool begin, VkCommandBufferLevel level ) {

	assert ( commandBuffer == false );

	VkCommandBufferAllocateInfo cmdBufAllocateInfo = MOAIGfxStructVK::commandBufferAllocateInfo (
    	this->mPool,
    	VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    	1
	);
    logicalDevice.AllocateCommandBuffers ( cmdBufAllocateInfo, commandBuffer );
	
	assert ( commandBuffer != false );
	
	if ( begin ) {
		VkCommandBufferBeginInfo cmdBufInfo = MOAIGfxStructVK::commandBufferBeginInfo ();
		VK_CHECK_RESULT ( vkBeginCommandBuffer ( commandBuffer, &cmdBufInfo ));
	}
}

//----------------------------------------------------------------//
void MOAIQueueVK::FlushAndFreeCommandBuffer	( MOAILogicalDeviceVK& logicalDevice, MOAICommandBufferVK& commandBuffer, u64 timeout ) {

	if ( commandBuffer == VK_NULL_HANDLE ) return;

	VK_CHECK_RESULT ( vkEndCommandBuffer ( commandBuffer ));

	VkSubmitInfo submitInfo = MOAIGfxStructVK::submitInfo ( commandBuffer, 1 );

	// Create fence to ensure that the command buffer has finished executing
	VkFenceCreateInfo fenceInfo = MOAIGfxStructVK::fenceCreateInfo ( 0 );

	VkFence fence;
	VK_CHECK_RESULT ( vkCreateFence ( logicalDevice, &fenceInfo, NULL, &fence ));

	// Submit to the queue
	VK_CHECK_RESULT ( vkQueueSubmit ( this->mQueue, 1, &submitInfo, fence ));

	// Wait for the fence to signal that command buffer has finished executing
	VK_CHECK_RESULT ( vkWaitForFences ( logicalDevice, 1, &fence, VK_TRUE, timeout ));

	vkDestroyFence ( logicalDevice, fence, nullptr );

	vkFreeCommandBuffers ( logicalDevice, this->mPool, 1, commandBuffer );
}

//----------------------------------------------------------------//
MOAIQueueVK::MOAIQueueVK () :
	mIndex (( u32 )-1 ),
	mPool ( VK_NULL_HANDLE ),
	mQueue ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIQueueVK::~MOAIQueueVK () {
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::PresentKHR ( MOAILogicalDeviceVK& logicalDevice, const VkPresentInfoKHR& presentInfo ) {

	if ( logicalDevice.mQueuePresentKHR ) {
		logicalDevice.mQueuePresentKHR ( this->mQueue, &presentInfo );
	}
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::Submit ( const VkSubmitInfo* submits, u32 submitCount, VkFence fence ) {

	return vkQueueSubmit ( this->mQueue, submitCount, submits, fence );
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::WaitIdle () {

	return vkQueueWaitIdle ( this->mQueue );
}
