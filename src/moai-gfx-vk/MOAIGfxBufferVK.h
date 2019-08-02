// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
// TODO: doxygen
class MOAIGfxBufferVK :
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer {
protected:

	VkBuffer		mBuffer;
	VkDeviceMemory	mMemory;
	VkDeviceSize	mAllocationSize;
	
	//----------------------------------------------------------------//
	void					GenericInit									( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags );
	
	//----------------------------------------------------------------//
	bool					MOAIGfxResource_OnCPUCreate					();
	void 					MOAIGfxResource_OnCPUDestroy				();
	void					MOAIGfxResource_OnCPUPurgeRecoverable		();
	void					MOAIGfxResourceVK_OnGPUBind					();
	bool					MOAIGfxResourceVK_OnGPUCreate				();
	void					MOAIGfxResourceVK_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					MOAIGfxResourceVK_OnGPUUnbind				();
	bool					MOAIGfxResourceVK_OnGPUUpdate				();
	void					MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut					( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	static const VkMemoryPropertyFlags HOST_BUFFER_PROPS = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	static const VkMemoryPropertyFlags DEVICE_BUFFER_PROPS = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	GET ( VkBuffer&, Buffer, this->mBuffer );
	
	//----------------------------------------------------------------//
	operator VkBuffer () {
		return this->mBuffer;
	}
	
	//----------------------------------------------------------------//
	void					Bind						();
	void					Cleanup						();
	void					MapAndCopy					( const void* data, size_t size );
							MOAIGfxBufferVK				();
							~MOAIGfxBufferVK			();
};

//================================================================//
// MOAIBufferVK
//================================================================//
template < VkBufferUsageFlags USAGE >
class MOAIGfxBufferUsageVK :
	public virtual MOAIGfxBufferVK {
public:

	//----------------------------------------------------------------//
	void Init ( VkDeviceSize size, VkBufferUsageFlags usage = 0, VkMemoryPropertyFlags memPropFlags = HOST_BUFFER_PROPS ) {

		usage = usage & ( VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT );
		this->GenericInit ( size, USAGE | usage, memPropFlags );
	}
};

typedef MOAIGfxBufferUsageVK < VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT > MOAIUniformBufferVK;

#endif
