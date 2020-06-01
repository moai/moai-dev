// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIVK_H
#define	MOAIDRAWINGAPIVK_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIAbstractDescriptorElementStateVK;

//================================================================//
// MOAIDrawingCmdEnumVK
//================================================================//
struct MOAIDrawingCmdEnumVK {
	enum {
		BASE = MOAIDrawingAPIEnum::TOTAL_BASE_COMMANDS,
		
		LOAD_DESCRIPTOR_ELEMENT_VK,
		LOAD_DESCRIPTOR_FROM_TEXTURE_UNIT_VK,
		
		LOAD_SHADER_UNIFORM_VK,
	};
};

//================================================================//
// MOAIDrawingParamVK
//================================================================//
namespace MOAIDrawingParamVK {

	//----------------------------------------------------------------//
	struct LoadDescriptor {
		ZLIndex								mDescriptorSetID;
		ZLIndex								mBinding;
		ZLIndex								mArrayElement;
	};

	//----------------------------------------------------------------//
	struct LoadDescriptorElement :
		public LoadDescriptor {
		
		MOAIAbstractDescriptorElementStateVK*	mElement;
	};

	//----------------------------------------------------------------//
	struct LoadDescriptorFromTextureUnit :
		public LoadDescriptor {
		
		ZLIndex								mTextureUnit;
	};

	//----------------------------------------------------------------//
	struct LoadShaderUniform {
		ZLIndex								mPipelineGlobalID;
		ZLIndex								mTargetUniformID;
		ZLIndex								mTargetUniformIndex;
	};
};

//================================================================//
// MOAIDrawingAPIVK
//================================================================//
struct MOAIDrawingAPIVK {

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam );
	static void		ExecuteSetUniformVK		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamVK::LoadShaderUniform& param );
};

#endif
