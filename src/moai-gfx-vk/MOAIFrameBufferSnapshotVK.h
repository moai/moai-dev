// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERSNAPSHOTVK_H
#define	MOAIFRAMEBUFFERSNAPSHOTVK_H

#include <moai-gfx-vk/MOAIImageBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAILogicalDeviceVK;
class MOAIGfxMgrVK;
class MOAIRenderPassVK;

//================================================================//
// MOAIFrameBufferSnapshotVK
//================================================================//
// TODO: doxygen
class MOAIFrameBufferSnapshotVK :
	public virtual MOAISnapshotVK < MOAIFrameBufferSnapshotVK >,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK >,
	public virtual ZLFinalizable_DependsOn < MOAIRenderPassVK > {
protected:
	
	ZL_FINALIZATION_VISITOR_FRIEND
	
	friend class MOAIFrameBufferVK;
	
	ZLLeanArray < ZLStrongPtr < MOAIImageBufferSnapshotVK > > mAttachmentSnapshots;
	ZLLeanArray < VkImageView > mAttachments;
	VkFramebuffer mFrameBuffer;

	//----------------------------------------------------------------//
	void 							_Finalize								();
	void							MOAIAbstractSnapshotVK_OnPin			( MOAICommandBufferVK& commandBuffer );
	void							MOAIAbstractSnapshotVK_OnUnpin			();
	MOAIFrameBufferSnapshotVK*		MOAISnapshotFactoryVK_GetSnapshot		();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIFrameBufferSnapshotVK )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mFrameBuffer != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkFramebuffer& () {
		return this->mFrameBuffer;
	}
	
	//----------------------------------------------------------------//
					MOAIFrameBufferSnapshotVK			();
					~MOAIFrameBufferSnapshotVK			();
};

#endif
