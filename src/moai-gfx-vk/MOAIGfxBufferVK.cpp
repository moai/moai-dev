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
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxBufferVK >)
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
