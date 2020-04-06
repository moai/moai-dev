// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Invalidate () {

	if ( !this->mIsValid ) return;

	ZLSize sigSize = this->mSignature.Size ();
	for ( ZLIndex index = 0; index < sigSize; ++index ) {
		this->mSignature [ index ]->RemoveDescriptorSet ( *this );
	}
	this->mIsValid = false;

	if ( this->HasDependency < MOAIDescriptorSetLayoutVK >()) {
		MOAIDescriptorSetLayoutVK& descriptorSetLayout = this->GetDependency < MOAIDescriptorSetLayoutVK >();
		descriptorSetLayout.InvalidateDescriptorSet ( *this );
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::MOAIDescriptorSetVK () :
	mDescriptorSet ( VK_NULL_HANDLE ),
	mIsValid ( false ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {

	this->Invalidate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::MOAIAbstractSnapshotVK_OnPin ( MOAICommandBufferVK& commandBuffer ) {
	
	assert ( this->mIsValid );
	
	ZLSize sigSize = this->mSignature.Size ();
	for ( ZLIndex index = 0; index < sigSize; ++index ) {
		commandBuffer.Pin ( *this->mSignature [ index ]);
	}
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetDependency < MOAIDescriptorSetLayoutVK >().RetireDescriptorSet ( *this );
}
