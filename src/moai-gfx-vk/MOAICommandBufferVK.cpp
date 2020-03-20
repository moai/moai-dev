// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodySchemaVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>

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
void MOAICommandBufferVK::BindDescriptorSet ( VkPipelineBindPoint pipelineBindPoint, MOAIDescriptorSetSnapshotVK& descriptorSet, MOAIPipelineInputBodySchemaVK& pipelineLayout, u32 firstSet ) {

	vkCmdBindDescriptorSets ( this->mCommandBuffer, pipelineBindPoint, pipelineLayout, firstSet, 1, descriptorSet, 0, NULL );
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::BindPipeline ( VkPipelineBindPoint pipelineBindPoint, MOAIPipelineSnapshotVK& pipeline ) {

	vkCmdBindPipeline ( this->mCommandBuffer, pipelineBindPoint, pipeline );
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::End () {

	VK_CHECK_RESULT ( vkEndCommandBuffer ( this->mCommandBuffer ));
}

//----------------------------------------------------------------//
MOAILogicalDeviceVK& MOAICommandBufferVK::GetLogicalDevice () {

	return this->GetDependency < MOAIQueueVK >().GetDependency < MOAILogicalDeviceVK >();
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

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Pin ( MOAIAbstractSnapshotVK& snapshot ) {

	if ( !this->mSnapshots.contains ( &snapshot )) {
		snapshot.Pin ();
		this->mSnapshots [ &snapshot ] = &snapshot;
	}
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::Submit () {

	VkSubmitInfo submitInfo = MOAIGfxStructVK::submitInfo ( *this );

	MOAIQueueVK& queue = this->GetDependency < MOAIQueueVK >();
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
	MOAIQueueVK& queue = this->GetDependency < MOAIQueueVK >();
	VK_CHECK_RESULT ( queue.Submit ( submitInfo ));
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::UnpinAll () {

	SnapshotMapIt snapshotIt = this->mSnapshots.begin ();
	for ( ; snapshotIt != this->mSnapshots.end (); ++snapshotIt ) {
		ZLWeakPtr < MOAIAbstractSnapshotVK >& snapshotPtr = snapshotIt->second;
		if ( snapshotPtr ) {
			snapshotPtr->Unpin ();
		}
	}
	this->mSnapshots.clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAICommandBufferVK::_Finalize () {

	this->UnpinAll ();

	if ( this->HasDependency < MOAIQueueVK >()) {
		MOAIQueueVK& queue = this->GetDependency < MOAIQueueVK >();
		vkFreeCommandBuffers ( queue.GetDependency < MOAILogicalDeviceVK >(), queue.mPool, 1, &this->mCommandBuffer );
	}
}
