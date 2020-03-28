// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGCOMMANDVK_H
#define	MOAIDRAWINGCOMMANDVK_H

#include <moai-gfx/MOAIGfxConsts.h>

//================================================================//
// MOAIDrawingCmdEnumVK
//================================================================//
struct MOAIDrawingCmdEnumVK {
	enum {
		BASE = MOAIDrawingCmdEnum::TOTAL_BASE_COMMANDS,
		LOAD_SHADER_UNIFORM_VK,
	};
};

//================================================================//
// MOAIDrawingParamVK
//================================================================//
namespace MOAIDrawingParamVK {

	//----------------------------------------------------------------//
	struct LoadShaderUniform {
		ZLIndex				mPipelineGlobalID;
		ZLIndex				mTargetUniformID;
		ZLIndex				mTargetUniformIndex;
	};
};

//================================================================//
// MOAIDrawingCommandVK
//================================================================//
struct MOAIDrawingCommandVK {

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam );
	static void		ExecuteSetUniformVK		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::SetUniform& param );
};

#endif
