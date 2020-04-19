// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERVK_H
#define	MOAIFRAMEBUFFERVK_H

#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>

class MOAIImageBufferSnapshotVK;
class MOAILogicalDeviceVK;
class MOAIGfxMgrVK;
class MOAIRenderPassVK;

//================================================================//
// MOAIFrameBufferSnapshotVK
//================================================================//
// TODO: doxygen
class MOAIFrameBufferSnapshotVK :
	public virtual MOAIAbstractSnapshotVK,
	public virtual ZLFinalizable,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK >,
	public virtual ZLFinalizable_DependsOn < MOAIRenderPassVK > {
protected:
	
	ZL_FINALIZATION_VISITOR_FRIEND
	
	friend class MOAIFrameBufferVK;
	
	ZLLeanArray < ZLStrongPtr < MOAIImageBufferSnapshotVK > > mAttachmentSnapshots;
	ZLLeanArray < VkImageView > mAttachments;
	VkFramebuffer mFrameBuffer;

	//----------------------------------------------------------------//
	void 			_Finalize							();
	void			MOAIAbstractSnapshotVK_OnPin		( MOAICommandBufferVK& commandBuffer );
	void			MOAIAbstractSnapshotVK_OnUnpin		();

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

//================================================================//
// MOAIFrameBufferVK
//================================================================//
// TODO: doxygen
class MOAIFrameBufferVK :
	public virtual MOAIFrameBuffer,
	public virtual ZLFinalizable,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK >,
	public virtual ZLFinalizable_DependsOn < MOAIRenderPassVK > {
protected:
	
	ZL_FINALIZATION_VISITOR_FRIEND
	
	friend class MOAIGfxMgrVK_GPUCacheVK;

	ZLLeanArray < ZLStrongPtr < MOAIImageBufferSnapshotVK > > mAttachments;
	ZLStrongPtr < MOAIFrameBufferSnapshotVK > mSnapshot;

	//----------------------------------------------------------------//
	void 			_Finalize					();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferVK )
	
	//----------------------------------------------------------------//
	MOAIFrameBufferSnapshotVK*		GetSnapshot					();
									MOAIFrameBufferVK			();
									~MOAIFrameBufferVK			();
	void							ReserveAttachments			( ZLSize count );
	void							SetAttachment				( ZLIndex index, MOAIImageBufferSnapshotVK& imageViewSnapshot );
	void							SetLogicalDevice			( MOAILogicalDeviceVK& logicalDevice );
	void							SetRenderPass				( MOAIRenderPassVK& renderPass );
};

#endif
