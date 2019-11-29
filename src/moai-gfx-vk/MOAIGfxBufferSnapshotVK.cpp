// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>

//================================================================//
// MOAIGfxBufferSnapshotVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferSnapshotVK::Initialize ( MOAIGfxBufferVK& buffer ) {

	MOAILogicalDeviceVK& logicalDevice = buffer.GetProvider < MOAILogicalDeviceVK >();

	// TODO: support staging buffers

	ZLSize size = buffer.GetSize ();

	this->MOAIUtilityBufferVK::Initialize (
		logicalDevice,
		size,
		buffer.mUsage,
		HOST_BUFFER_PROPS
	);
	
	this->MapAndCopy ( buffer.GetConstBuffer (), size );
}

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK::MOAIGfxBufferSnapshotVK () {
}

//----------------------------------------------------------------//
MOAIGfxBufferSnapshotVK::~MOAIGfxBufferSnapshotVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
//void MOAIGfxBufferSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {
//
//	delete ( this );
//}
