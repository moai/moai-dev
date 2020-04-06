// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>

//================================================================//
// MOAIAbstractDescriptorElementVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDescriptorElementVK::AffirmDescriptorSet ( MOAIDescriptorSetVK& set ) {

	this->mDescriptorSets.affirm ( &set );
}

//----------------------------------------------------------------//
void MOAIAbstractDescriptorElementVK::Invalidate () {

	STLSet < MOAIDescriptorSetVK* >::iterator descriptorSetIt = this->mDescriptorSets.begin ();
	while ( descriptorSetIt != this->mDescriptorSets.end ()) {
		MOAIDescriptorSetVK* descriptorSet = *descriptorSetIt;
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
void MOAIAbstractDescriptorElementVK::RemoveDescriptorSet ( MOAIDescriptorSetVK& set ) {

	if ( this->mDescriptorSets.contains ( &set )) {
		this->mDescriptorSets.erase ( &set );
	}
}
