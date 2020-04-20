// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::AffirmDescriptorSetLayout () {

	if ( this->mImpl ) return;
	
	assert ( this->mLayoutBindings );
	
	this->mImpl = MOAIGfxMgrVK::Get ().ProcureDescriptorSetLayoutImpl ( MOAIDescriptorSetLayoutKeyVK ( *this->mLayoutBindings ));
	
	delete this->mLayoutBindings;
	this->mLayoutBindings = &this->mImpl->mLayoutBindings;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::DeletePool ( MOAIDescriptorPoolVK* pool ) {

	assert ( this->mImpl );
	this->mImpl->DeletePool ( pool );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings ) {

	assert ( !this->mLayoutBindings );

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mLayoutBindings = new ZLLeanArray < VkDescriptorSetLayoutBinding >();
	this->mLayoutBindings->Init ( totalBindings );
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::MOAIDescriptorSetLayoutVK () :
	mLayoutBindings ( NULL ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::~MOAIDescriptorSetLayoutVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetLayoutVK::ProcureDescriptorSet ( const MOAIDescriptorSetVK& descriptorSetState ) {
	
	this->AffirmDescriptorSetLayout ();
	return this->mImpl->ProcureDescriptorSet ( descriptorSetState );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::SetBinding ( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount ) {

	assert ( index < this->mLayoutBindings->Size ());
	( *this->mLayoutBindings )[ index ] = MOAIGfxStructVK::descriptorSetLayoutBinding (
		( u32 )index,
		descriptorType,
		stageFlags,
		( u32 )descriptorCount,
		NULL
	);
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::_Finalize () {

	if ( this->mLayoutBindings && ( this->mImpl == NULL )) {
		delete this->mLayoutBindings;
	}
	this->mImpl = NULL;
}
