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
void MOAIGfxBufferVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize size, VkBufferUsageFlags usage ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
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
MOAIGfxBufferSnapshotVK* MOAIGfxBufferVK::MOAISnapshotFactoryVK_GetSnapshot () {

	// TODO: re-use snapshot
	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAIGfxBufferSnapshotVK* snapshot = new MOAIGfxBufferSnapshotVK ();
		snapshot->Initialize ( this->GetDependency < MOAILogicalDeviceVK >(), *this );
		return snapshot;
	}
	return NULL;
}
