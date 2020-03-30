// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGCOMMANDVK_H
#define	MOAIDRAWINGCOMMANDVK_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIAbstractDescriptorElementVK;

//================================================================//
// MOAIDrawingCmdEnumVK
//================================================================//
struct MOAIDrawingCmdEnumVK {
	enum {
		BASE = MOAIDrawingCmdEnum::TOTAL_BASE_COMMANDS,
		
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
		
		MOAIAbstractDescriptorElementVK*	mElement;
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
// MOAIDrawingCommandVK
//================================================================//
struct MOAIDrawingCommandVK {

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam );
	static void		ExecuteSetUniformVK		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamVK::LoadShaderUniform& param );
};

#endif
