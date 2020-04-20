// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAISnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIGfxBufferSnapshotVK;
class MOAILogicalDeviceVK;
class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
// TODO: doxygen
class MOAIGfxBufferVK :
	public MOAISnapshotFactoryVK < MOAIGfxBufferSnapshotVK >,
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer,
	public virtual ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIGfxBufferSnapshotVK;

	VkBufferUsageFlags		mUsage;
	
	//----------------------------------------------------------------//
	MOAIGfxBufferSnapshotVK*	MOAISnapshotFactoryVK_GetSnapshot		();

public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIGfxBufferVK )
	
	//----------------------------------------------------------------//
	void						Initialize				( MOAILogicalDeviceVK& logicalDevice, ZLSize size, VkBufferUsageFlags usage = 0 );
								MOAIGfxBufferVK			();
								~MOAIGfxBufferVK		();
};

//================================================================//
// MOAIGfxBufferWithUsageVK
//================================================================//
template < VkBufferUsageFlags USAGE >
class MOAIGfxBufferWithUsageVK :
	public virtual MOAIGfxBufferVK {
public:

	//----------------------------------------------------------------//
	void Initialize ( MOAILogicalDeviceVK& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage = 0 ) {

		this->MOAIGfxBufferVK::Initialize ( logicalDevice, size, USAGE | usage );
	}
	
	//----------------------------------------------------------------//
	MOAIGfxBufferWithUsageVK () {
	
		this->mUsage = USAGE;
	}
};

typedef MOAIGfxBufferWithUsageVK < VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT > MOAIUniformBufferVK;

#endif
