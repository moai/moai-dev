// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractCommandBufferMemberVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIAbstractCommandBufferMemberVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCommandBufferMemberVK::ForceRemove () {

	if ( this->mCommandBuffer ) {
		this->mCommandBuffer->Invalidate ();
		this->mLink.Remove ();
		this->mCommandBuffer = NULL;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractCommandBufferMemberVK::ForceUnpin () {

	if ( this->mCommandBuffer ) {
		this->ForceRemove ();
		this->MOAIAbstractCommandBufferMemberVK_Discard ();
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractCommandBufferMemberVK::IsPinned () {
	return ( this->mCommandBuffer != NULL );
}

//----------------------------------------------------------------//
MOAIAbstractCommandBufferMemberVK::MOAIAbstractCommandBufferMemberVK () :
	mLink ( this ),
	mCommandBuffer ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractCommandBufferMemberVK::~MOAIAbstractCommandBufferMemberVK () {
	this->ForceRemove ();
}
