// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAILogicalDeviceVK;
class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
// TODO: doxygen
class MOAIGfxBufferVK :
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK >,
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer {
protected:

	friend class MOAIUtilityBufferVK;

	VkBufferUsageFlags		mUsage;
	
	//----------------------------------------------------------------//
	void						MOAILuaObject_RegisterLuaClass					( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs					( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_SerializeIn						( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void						MOAILuaObject_SerializeOut						( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	IMPLEMENT_FINALIZABLE ( MOAIGfxBufferVK )
	
	//----------------------------------------------------------------//
	void					Initialize				( MOAILogicalDeviceVK& logicalDevice, ZLSize size, VkBufferUsageFlags usage = 0 );
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
};

typedef MOAIGfxBufferWithUsageVK < VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT > MOAIUniformBufferVK;

#endif
