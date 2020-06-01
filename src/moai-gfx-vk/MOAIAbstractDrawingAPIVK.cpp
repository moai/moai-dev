// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAIAbstractDrawingAPIVK.h>
#include <moai-gfx-vk/MOAIDrawingAPIVK.h>

//================================================================//
// MOAIAbstractDrawingAPIVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIVK::LoadDescriptorElementVK ( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK& element ) {
	
	MOAIDrawingParamVK::LoadDescriptorElement param;
	param.mDescriptorSetID		= descriptorSetID;
	param.mBinding				= binding;
	param.mArrayElement			= arrayElement;
	param.mElement				= &element;
	
	this->SubmitCommand < MOAIDrawingParamVK::LoadDescriptorElement >( MOAIDrawingCmdEnumVK::LOAD_DESCRIPTOR_ELEMENT_VK, param );
	this->RetainObject ( &element );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIVK::LoadDescriptorFromTextureUnitVK ( ZLIndex descriptorSetID, ZLIndex binding, ZLIndex arrayElement, ZLIndex textureUnit ) {

	MOAIDrawingParamVK::LoadDescriptorFromTextureUnit param;
	param.mDescriptorSetID		= descriptorSetID;
	param.mBinding				= binding;
	param.mArrayElement			= arrayElement;
	param.mTextureUnit			= textureUnit;
	
	this->SubmitCommand < MOAIDrawingParamVK::LoadDescriptorFromTextureUnit >( MOAIDrawingCmdEnumVK::LOAD_DESCRIPTOR_FROM_TEXTURE_UNIT_VK, param );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIVK::LoadShaderUniformVK ( ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	MOAIDrawingParamVK::LoadShaderUniform param;
	param.mPipelineGlobalID		= globalID;
	param.mTargetUniformID		= uniformID;
	param.mTargetUniformIndex	= index;
	
	this->SubmitCommand < MOAIDrawingParamVK::LoadShaderUniform >( MOAIDrawingCmdEnumVK::LOAD_SHADER_UNIFORM_VK, param );
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIVK::MOAIAbstractDrawingAPIVK () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIVK::~MOAIAbstractDrawingAPIVK () {
}
