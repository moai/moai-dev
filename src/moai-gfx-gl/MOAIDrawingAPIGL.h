// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIGL_H
#define	MOAIDRAWINGAPIGL_H

#include <moai-gfx/MOAIGfxConsts.h>

//================================================================//
// MOAIDrawingCmdEnumGL
//================================================================//
struct MOAIDrawingCmdEnumGL {
	enum {
		BASE = MOAIDrawingAPIEnum::TOTAL_BASE_COMMANDS,
		LOAD_SHADER_UNIFORM_GL,
	};
};

//================================================================//
// MOAIDrawingParamGL
//================================================================//
namespace MOAIDrawingParamGL {

	//----------------------------------------------------------------//
	struct LoadShaderUniform {
		ZLIndex				mPipelineGlobalID;
		ZLIndex				mTargetUniformID;
		ZLIndex				mTargetUniformIndex;
	};
};

//================================================================//
// MOAIDrawingAPIGL
//================================================================//
struct MOAIDrawingAPIGL {

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam );
	static void		ExecuteSetUniformGL		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamGL::LoadShaderUniform& param );
};

#endif
