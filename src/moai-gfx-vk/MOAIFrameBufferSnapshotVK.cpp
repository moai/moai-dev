// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFrameBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIImageBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

//================================================================//
// MOAIFrameBufferSnapshotVK
//================================================================//
	
//----------------------------------------------------------------//
MOAIFrameBufferSnapshotVK::MOAIFrameBufferSnapshotVK () {

	RTTI_BEGIN ( MOAIFrameBufferSnapshotVK )
		RTTI_EXTEND ( ZLFinalizable )
		RTTI_VISITOR ( ZLAbstractFinalizationVisitor, ZLFinalizationVisitor < MOAIFrameBufferSnapshotVK >)
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferSnapshotVK::~MOAIFrameBufferSnapshotVK () {

	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferSnapshotVK::_Finalize () {

	if ( this->mFrameBuffer ) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyFramebuffer ( logicalDevice, this->mFrameBuffer, NULL );
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferSnapshotVK::MOAIAbstractSnapshotVK_OnPin ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );
	
	ZLSize nAttachments = this->mAttachmentSnapshots.Size ();
	for ( ZLSize i = 0; i < nAttachments; ++i ) {
		commandBuffer.Pin ( *this->mAttachmentSnapshots [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {
}

//----------------------------------------------------------------//
MOAIFrameBufferSnapshotVK* MOAIFrameBufferSnapshotVK::MOAISnapshotFactoryVK_GetSnapshot () {

	return this;
}
