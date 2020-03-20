// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMVK_H
#define	MOAISHADERPROGRAMVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIPipelineInputBodyComposerVK;
class MOAILogicalDeviceVK;
class MOAIPipelineInputBodySchemaVK;
class MOAIShaderVK;
class MOAITextureVK;

//================================================================//
// MOAIShaderProgramVK
//================================================================//
/**	@lua	MOAIShaderProgramVK
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShaderVK instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class MOAIShaderProgramVK :
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK >,
	public virtual MOAIGfxResourceVK {
public:

	enum ModuleID {
		FRAGMENT_MODULE,
		VERTEX_MODULE,
		TOTAL_MODULES,
	};

protected:

	VkShaderModule	mModules [ TOTAL_MODULES ];

	ZLLeanArray < VkPipelineShaderStageCreateInfo >		mStageInfos;
	ZLStrongPtr < MOAIPipelineInputBodyComposerVK > 					mComposer;

	//----------------------------------------------------------------//
//	static int					_load						( lua_State* L );
	
	//----------------------------------------------------------------//
	VkShaderStageFlagBits		GetShaderStageBit			( ModuleID moduleID );
	
	//----------------------------------------------------------------//
	void 						_Finalize 					();
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramVK )

	//----------------------------------------------------------------//
	MOAIPipelineInputBodyComposerVK&			GetGfxComposer						();
	MOAIPipelineInputBodySchemaVK&		GetPipelineLayout					();
	void						Initialize							( MOAILogicalDeviceVK& logicalDevice );
	void						LoadModule							( ModuleID moduleID, const void* shaderCode, size_t shaderSize );
								MOAIShaderProgramVK					();
								~MOAIShaderProgramVK				();
	void						SetGfxComposer						( MOAIPipelineInputBodyComposerVK& composer );
	void						UpdatePipelineCreateInfo			( VkGraphicsPipelineCreateInfo& info );
};

#endif
