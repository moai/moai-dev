// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIGfxBufferSnapshotVK;
class MOAILogicalDeviceVK;
class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
// TODO: doxygen
class MOAIGfxBufferVK :
	public MOAIAbstractSnapshotFactoryVK < MOAIGfxBufferSnapshotVK >,
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIGfxBufferSnapshotVK;

	VkBufferUsageFlags		mUsage;
	
	//----------------------------------------------------------------//
	MOAIGfxBufferSnapshotVK*	MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer );
	void						MOAILuaObject_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	void						MOAILuaObject_SerializeIn						( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void						MOAILuaObject_SerializeOut						( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	IMPLEMENT_FINALIZABLE ( MOAIGfxBufferVK )
	
	//----------------------------------------------------------------//
	void						Initialize				( ZLSize size, VkBufferUsageFlags usage = 0 );
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
	void Initialize ( VkDeviceSize size, VkBufferUsageFlags usage = 0 ) {

		this->MOAIGfxBufferVK::Initialize ( size, USAGE | usage );
	}
};

typedef MOAIGfxBufferWithUsageVK < VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT > MOAIUniformBufferVK;

#endif
