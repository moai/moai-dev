// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>

//================================================================//
// MOAIAbstractDescriptorElementVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDescriptorElementVK::AffirmDescriptorSet ( MOAIDescriptorSetSnapshotVK& set ) {

	this->mDescriptorSets.affirm ( &set );
}

//----------------------------------------------------------------//
void MOAIAbstractDescriptorElementVK::Invalidate () {

	STLSet < MOAIDescriptorSetSnapshotVK* >::iterator descriptorSetIt = this->mDescriptorSets.begin ();
	while ( descriptorSetIt != this->mDescriptorSets.end ()) {
		MOAIDescriptorSetSnapshotVK* descriptorSet = *descriptorSetIt;
		++descriptorSetIt;
		descriptorSet->Invalidate ();
	}
	this->mDescriptorSets.clear ();
}

//----------------------------------------------------------------//
MOAIAbstractDescriptorElementVK::MOAIAbstractDescriptorElementVK () {
}

//----------------------------------------------------------------//
MOAIAbstractDescriptorElementVK::~MOAIAbstractDescriptorElementVK () {

	this->Invalidate ();
}

//----------------------------------------------------------------//
void MOAIAbstractDescriptorElementVK::RemoveDescriptorSet ( MOAIDescriptorSetSnapshotVK& set ) {

	if ( this->mDescriptorSets.contains ( &set )) {
		this->mDescriptorSets.erase ( &set );
	}
}
