// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::AffirmDescriptorSetLayout () {

	if ( this->mImpl ) return;
	this->mImpl = new MOAIDescriptorSetLayoutImplVK ();
	this->mImpl->Initialize (
		this->GetDependency < MOAILogicalDeviceVK >(),
		this->mLayoutBindings.GetBuffer (),
		this->mLayoutBindings.Size ()
	);
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::DeletePool ( MOAIDescriptorPoolVK* pool ) {

	assert ( this->mImpl );
	this->mImpl->DeletePool ( pool );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mLayoutBindings.Init ( totalBindings );
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::MOAIDescriptorSetLayoutVK () :
	mImpl ( NULL ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::~MOAIDescriptorSetLayoutVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK* MOAIDescriptorSetLayoutVK::ProcureDescriptorSet ( const MOAIDescriptorSetStateVK& descriptorSetState ) {
	
	this->AffirmDescriptorSetLayout ();
	return this->mImpl->ProcureDescriptorSet ( descriptorSetState );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::SetBinding ( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount ) {

	assert ( index < this->mLayoutBindings.Size ());
	this->mLayoutBindings [ index ] = MOAIGfxStructVK::descriptorSetLayoutBinding (
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

	this->mImpl = NULL;
}
