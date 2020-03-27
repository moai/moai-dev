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

	MOAIDrawingParam::SetUniform param;
	param.mPipelineGlobalID = globalID;
	param.mTargetUniformID = uniformID;
	param.mTargetUniformIndex = index;
	this->SubmitCommand < MOAIDrawingParam::SetUniform >( MOAIDrawingCmdEnumGL::LOAD_SHADER_UNIFORM_GL, param );
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIGL::MOAIAbstractDrawingAPIGL () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPIGL::~MOAIAbstractDrawingAPIGL () {
}
