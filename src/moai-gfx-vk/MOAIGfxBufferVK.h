// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
/**	@lua	MOAIGfxBufferVK
	@text	Base class for MOAIVertexBufferVK and MOAIIndexBufferVK.
*/
class MOAIGfxBufferVK :
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer {
protected:
	
	static const VkMemoryPropertyFlags HOST_BUFFER_PROPS = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	static const VkMemoryPropertyFlags DEVICE_BUFFER_PROPS = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkBuffer		mBuffer;
	VkDeviceMemory	mMemory;
	VkDeviceSize	mAllocationSize;
	
	//----------------------------------------------------------------//
	operator VkBuffer () {
		return this->mBuffer;
	}
	
	//----------------------------------------------------------------//
	void					Bind										( VkDevice device );
	void					Cleanup										( VkDevice device );
	void					Init										( VkDevice device, VkPhysicalDeviceMemoryProperties memProps, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags = HOST_BUFFER_PROPS );
	void					MapAndCopy									( VkDevice device, const void* data, size_t size );
	
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
	
//	GET ( size_t, BufferCount, mVBOs.Size ())
//	GET ( u32, Target, mTarget )
//	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
//
//	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
//	size_t					CountVBOs					();
							MOAIGfxBufferVK				();
							~MOAIGfxBufferVK			();
//	void					ReserveVBOs					( ZLSize gpuBuffers );
};

#endif
