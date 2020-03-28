// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDrawingAPIVK.h>
#include <moai-gfx-vk/MOAIDrawingCommandVK.h>

//================================================================//
// MOAIAbstractDrawingAPIVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIVK::LoadShaderUniformVK ( ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	MOAIDrawingParam::SetUniform param;
	param.mPipelineGlobalID = globalID;
	param.mTargetUniformID = uniformID;
	param.mTargetUniformIndex = index;
	this->SubmitCommand < MOAIDrawingParam::SetUniform >( MOAIDrawingCmdEnumVK::LOAD_SHADER_UNIFORM_VK, param );
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIVK::MOAIAbstractDrawingAPIVK () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIVK::~MOAIAbstractDrawingAPIVK () {
}
