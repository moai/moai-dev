// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGCOMMANDGL_H
#define	MOAIDRAWINGCOMMANDGL_H

#include <moai-gfx/MOAIGfxConsts.h>

//================================================================//
// MOAIDrawingCmdEnumGL
//================================================================//
struct MOAIDrawingCmdEnumGL {
	enum {
		BASE = MOAIDrawingCmdEnum::TOTAL_BASE_COMMANDS,
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
// MOAIDrawingCommandGL
//================================================================//
struct MOAIDrawingCommandGL {

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam );
	static void		ExecuteSetUniformGL		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamGL::LoadShaderUniform& param );
};

#endif
