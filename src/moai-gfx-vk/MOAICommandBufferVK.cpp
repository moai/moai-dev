// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractPinnableVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineVK.h>

//================================================================//
// MOAICommandBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAICommandBufferVK::Begin () {

	this->UnpinAll ();
	VkCommandBufferBeginInfo cmdBufInfo = MOAIGfxStructVK::commandBufferBeginInfo ();
	VK_CHECK_RESULT ( vkBeginCommandBuffer ( this->mCommandBuffer, &cmdBufInfo ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::BindDescriptorSet ( VkPipelineBindPoint pipelineBindPoint, MOAIDescriptorSetVK& descriptorSet, MOAIPipelineLayoutVK& pipelineLayout, u32 firstSet ) {

	this->Pin ( descriptorSet );
	this->Pin ( pipelineLayout );
	vkCmdBindDescriptorSets ( this->mCommandBuffer, pipelineBindPoint, pipelineLayout, firstSet, 1, descriptorSet, 0, NULL );
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::BindPipeline ( VkPipelineBindPoint pipelineBindPoint, MOAIPipelineVK& pipeline ) {

	this->Pin ( pipeline );
	vkCmdBindPipeline ( this->mCommandBuffer, pipelineBindPoint, pipeline );
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

	this->UnpinAll ();

	if ( this->HasProvider < MOAIQueueVK >()) {
		MOAIQueueVK& queue = this->GetProvider < MOAIQueueVK >();
		vkFreeCommandBuffers ( queue.GetProvider < MOAILogicalDeviceVK >(), queue.mPool, 1, &this->mCommandBuffer );
	}
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Pin ( MOAIAbstractPinnableVK& snapshot ) {

	if ( !this->mSnapshots.contains ( &snapshot )) {
		snapshot.Pin ();
		this->mSnapshots.insert ( &snapshot );
	}
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
void MOAICommandBufferVK::UnpinAll () {

	STLSet < MOAIAbstractPinnableVK* >::iterator snapshotIt = this->mSnapshots.begin ();
	for ( ; snapshotIt != this->mSnapshots.end (); ++snapshotIt ) {
		( *snapshotIt )->Unpin ();
	}
	this->mSnapshots.clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
