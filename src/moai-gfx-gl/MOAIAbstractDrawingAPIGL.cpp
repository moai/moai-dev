// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractDrawingAPIGL.h>
#include <moai-gfx-gl/MOAIDrawingCommandGL.h>

//================================================================//
// MOAIAbstractDrawingAPIGL
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPIGL::LoadShaderUniformGL ( ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	MOAIDrawingParamGL::LoadShaderUniform param;
	param.mPipelineGlobalID = globalID;
	param.mTargetUniformID = uniformID;
	param.mTargetUniformIndex = index;
	this->SubmitCommand < MOAIDrawingParamGL::LoadShaderUniform >( MOAIDrawingCmdEnumGL::LOAD_SHADER_UNIFORM_GL, param );
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIGL::MOAIAbstractDrawingAPIGL () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIGL::~MOAIAbstractDrawingAPIGL () {
}
