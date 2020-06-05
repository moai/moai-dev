// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERVK_H
#define	MOAIFRAMEBUFFERVK_H

#include <moai-gfx-vk/MOAIFrameBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIImageBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIRenderPassVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAIFrameBufferSnapshotVK;
class MOAILogicalDeviceVK;
class MOAIGfxMgrVK;
class MOAIRenderPassVK;

//================================================================//
// MOAIFrameBufferVK
//================================================================//
// TODO: doxygen
class MOAIFrameBufferVK :
	public virtual MOAIFrameBuffer,
	public virtual MOAISnapshotFactoryVK < MOAIFrameBufferSnapshotVK >,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK >,
	public virtual ZLFinalizable_DependsOn < MOAIRenderPassVK > {
protected:
	
	ZL_FINALIZATION_VISITOR_FRIEND
	
	friend class MOAIGfxMgrVK_GPUCacheVK;

	ZLLeanArray < MOAISnapshotRefVK < MOAIImageBufferSnapshotVK > > mAttachments;
	ZLStrongPtr < MOAIFrameBufferSnapshotVK > mSnapshot;

	//----------------------------------------------------------------//
	void 							_Finalize								();
	MOAIFrameBufferSnapshotVK*		MOAISnapshotFactoryVK_GetSnapshot		();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferVK )
	
	//----------------------------------------------------------------//
	MOAIRenderPassVK*				GetRenderPass				();
									MOAIFrameBufferVK			();
									~MOAIFrameBufferVK			();
	void							ReserveAttachments			( ZLSize count );
	void							SetAttachment				( ZLIndex index, MOAIImageBufferSnapshotVK::Factory& imageViewSnapshot );
	void							SetLogicalDevice			( MOAILogicalDeviceVK& logicalDevice );
	void							SetRenderPass				( MOAIRenderPassVK& renderPass );
};

#endif
