// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

//================================================================//
// MOAICommandBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAICommandBufferVK::Begin () {

	this->UnpinSnapshots ();
	VkCommandBufferBeginInfo cmdBufInfo = MOAIGfxStructVK::commandBufferBeginInfo ();
	VK_CHECK_RESULT ( vkBeginCommandBuffer ( this->mCommandBuffer, &cmdBufInfo ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::BindDescriptorSet ( VkPipelineBindPoint pipelineBindPoint, MOAIDescriptorSetVK& descriptorSet, MOAIPipelineLayoutVK& pipelineLayout, u32 firstSet ) {

	MOAIDescriptorSetSnapshotVK* descriptorSetSnapshot = this->MakeSnapshot < MOAIDescriptorSetSnapshotVK >( descriptorSet );
	vkCmdBindDescriptorSets ( this->mCommandBuffer, pipelineBindPoint, pipelineLayout, 0, 1, *descriptorSetSnapshot, 0, NULL );
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::End () {
	VK_CHECK_RESULT ( vkEndCommandBuffer ( this->mCommandBuffer ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Invalidate () {
	this->mIsValid = false;
}

//----------------------------------------------------------------//
MOAICommandBufferVK::MOAICommandBufferVK () :
	mCommandBuffer ( VK_NULL_HANDLE ),
	mIsValid ( false ) {
}

//----------------------------------------------------------------//
MOAICommandBufferVK::~MOAICommandBufferVK () {

	if ( this->HasProvider < MOAIQueueVK >()) {
		MOAIQueueVK& queue = this->GetProvider < MOAIQueueVK >();
		vkFreeCommandBuffers ( queue.GetProvider < MOAILogicalDeviceVK >(), queue.mPool, 1, &this->mCommandBuffer );
	}
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::PinSnapshot ( MOAIAbstractSnapshotVK& snapshot ) {

	snapshot.Pin ( *this );
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Submit () {

	VkSubmitInfo submitInfo = MOAIGfxStructVK::submitInfo ( *this );

	MOAIQueueVK& queue = this->GetProvider < MOAIQueueVK >();
	VK_CHECK_RESULT ( queue.Submit ( submitInfo ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Submit ( VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkPipelineStageFlags waitStageMask ) {
		
	VkSubmitInfo submitInfo = MOAIGfxStructVK::submitInfo (
		*this,				// Command buffers to submit in this batch
		1,					// One command buffer
		&waitSemaphore,		// Semaphore(s) to wait upon before the submitted command buffer starts executing
		1,					// One wait semaphore
		&signalSemaphore,	// Semaphore(s) to be signaled when command buffers have completed
		1,					// One signal semaphore
		&waitStageMask		// Pointer to the list of pipeline stages that the semaphore waits will occur at
	);
	MOAIQueueVK& queue = this->GetProvider < MOAIQueueVK >();
	VK_CHECK_RESULT ( queue.Submit ( submitInfo ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::UnpinSnapshots () {

	while ( this->mSnapshots.Count ()) {
		this->mSnapshots.Front ()->Unpin ();
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
