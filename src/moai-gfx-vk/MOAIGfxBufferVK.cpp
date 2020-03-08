// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// MOAIGfxBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferVK::Initialize ( ZLSize size, VkBufferUsageFlags usage ) {

	this->MOAIGfxBuffer::Initialize ( size );
	this->mUsage = usage;
}

//----------------------------------------------------------------//
MOAIGfxBufferVK::MOAIGfxBufferVK () :
	mUsage ( 0 ) {
		
	RTTI_BEGIN ( MOAIGfxBufferVK )
		RTTI_EXTEND ( MOAIGfxResourceVK )
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxBufferVK::~MOAIGfxBufferVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK* MOAIGfxBufferVK::MOAIAbstractSnapshotFactoryVK_GetSnapshot ( MOAICommandBufferVK& commandBuffer ) {

	MOAIGfxBufferSnapshotVK* snapshot = new MOAIGfxBufferSnapshotVK ();
	snapshot->Initialize ( commandBuffer.GetLogicalDevice (), *this );
	return snapshot;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
