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
void MOAIQueueVK::CreateCommandBuffer ( MOAICommandBufferVK& commandBuffer, VkCommandBufferLevel level, bool begin ) {

	assert ( commandBuffer == false );

	VkCommandBufferAllocateInfo cmdBufAllocateInfo = MOAIGfxStructVK::commandBufferAllocateInfo ( this->mPool, level );	
	VK_CHECK_RESULT ( vkAllocateCommandBuffers ( this->GetDependency < MOAILogicalDeviceVK >(), &cmdBufAllocateInfo, commandBuffer ));
	
	assert ( commandBuffer != false );
	
	if ( begin ) {
		VkCommandBufferBeginInfo cmdBufInfo = MOAIGfxStructVK::commandBufferBeginInfo ();
		VK_CHECK_RESULT ( vkBeginCommandBuffer ( commandBuffer, &cmdBufInfo ));
	}
	commandBuffer.SetDependency < MOAIQueueVK >( *this );
}

//----------------------------------------------------------------//
void MOAIQueueVK::FlushAndFreeCommandBuffer	( MOAICommandBufferVK& commandBuffer, u64 timeout ) {

	if ( commandBuffer == VK_NULL_HANDLE ) return;

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

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
	
	commandBuffer.UnpinAll (); // unpin any resources
	commandBuffer.Finalize ();
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
VkResult MOAIQueueVK::PresentKHR ( const VkPresentInfoKHR& presentInfo ) {

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	if ( logicalDevice.mQueuePresentKHR ) {
		logicalDevice.mQueuePresentKHR ( this->mQueue, &presentInfo );
	}
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::Submit ( const VkSubmitInfo& submitInfo ) {

	if ( submitInfo.signalSemaphoreCount == 0 ) {
	
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		if ( !this->mFence ) {
			this->mFence.Initialize ( logicalDevice );
		}
		VkResult result = vkQueueSubmit ( this->mQueue, 1, &submitInfo, this->mFence );
		this->mFence.Wait ();
		return result;
	}
	return vkQueueSubmit ( this->mQueue, 1, &submitInfo, VK_NULL_HANDLE );
}

//----------------------------------------------------------------//
VkResult MOAIQueueVK::WaitIdle () {

	return vkQueueWaitIdle ( this->mQueue );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
