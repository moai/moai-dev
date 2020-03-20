// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineInputBodyComposerVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodySchemaVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	load
//	@text	Load a shader program.
//
//	@in		MOAIShaderProgramVK self
//	@in		string vertexShaderSource
//	@in		string fragmentShaderSource
//	@out	nil
//*/
//int MOAIShaderProgramVK::_load ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIShaderProgramVK, "USS" )
//
//	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
//	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );
//
//	self->Load ( vtxSource, frgSource );
//
//	return 0;
//}

//================================================================//
// MOAIShaderProgramVK
//================================================================//

//----------------------------------------------------------------//
MOAIPipelineInputBodyComposerVK& MOAIShaderProgramVK::GetGfxComposer () {

	assert ( this->mComposer );
	return *this->mComposer;
}

//----------------------------------------------------------------//
MOAIPipelineInputBodySchemaVK& MOAIShaderProgramVK::GetPipelineLayout () {

	return this->GetGfxComposer ().GetPipelineLayout ();
}

//----------------------------------------------------------------//
VkShaderStageFlagBits MOAIShaderProgramVK::GetShaderStageBit ( ModuleID moduleID ) {

	switch ( moduleID ) {
		case FRAGMENT_MODULE:	return VK_SHADER_STAGE_FRAGMENT_BIT;
		case VERTEX_MODULE:		return VK_SHADER_STAGE_VERTEX_BIT;
	}
	return ( VkShaderStageFlagBits )0;
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::Initialize ( MOAILogicalDeviceVK& logicalDevice ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::LoadModule ( ModuleID moduleID, const void* shaderCode, size_t shaderSize ) {

	MOAILogicalDeviceVK& logicalDevice = MOAIGfxMgrVK::Get ().GetLogicalDevice ();

	// Create a new shader module that will be used for pipeline creation
	VkShaderModuleCreateInfo moduleCreateInfo = MOAIGfxStructVK::shaderModuleCreateInfo (( uint32_t* )shaderCode, shaderSize );

	VkShaderModule module;
	VK_CHECK_RESULT ( vkCreateShaderModule ( logicalDevice, &moduleCreateInfo, NULL, &module ));
	this->mModules [ moduleID ] = module;
}

//----------------------------------------------------------------//
MOAIShaderProgramVK::MOAIShaderProgramVK () {
	
	RTTI_BEGIN ( MOAIShaderProgramVK )
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
	
	memset ( this->mModules, NULL, sizeof ( this->mModules ));
}


//----------------------------------------------------------------//
MOAIShaderProgramVK::~MOAIShaderProgramVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::SetGfxComposer ( MOAIPipelineInputBodyComposerVK& composer ) {

	this->mComposer = &composer;
}

//----------------------------------------------------------------//
void MOAIShaderProgramVK::UpdatePipelineCreateInfo ( VkGraphicsPipelineCreateInfo& info ) {

	if ( this->mStageInfos.Size () == 0 ) {
	
		size_t moduleCount = 0;
		for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
			if ( this->mModules [( ModuleID )i ]) moduleCount++;
		}
		this->mStageInfos.Init ( moduleCount );
		
		size_t cursor = 0;
		for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
		
			ModuleID moduleID = ( ModuleID )i;
			VkShaderModule module = this->mModules [ i ];
			if ( module ) {
			
				this->mStageInfos [ cursor++ ] = MOAIGfxStructVK::pipelineShaderStageCreateInfo (
					this->GetShaderStageBit ( moduleID ),
					module,
					"main"
				);
			}
		}
	}
	
	info.pStages = this->mStageInfos.GetBuffer ();
	info.stageCount = ( u32 )this->mStageInfos.Size ();
	info.layout = this->GetPipelineLayout ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderProgramVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		
		for ( size_t i = 0; i < TOTAL_MODULES; ++i ) {
			ModuleID moduleID = ( ModuleID )i;
			if ( this->mModules [ moduleID ]) {
				vkDestroyShaderModule ( logicalDevice, this->mModules [ moduleID ], NULL );
				this->mModules [ moduleID ] = NULL;
			}
		}
	}
}
