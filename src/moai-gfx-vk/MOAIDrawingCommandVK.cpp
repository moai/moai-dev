// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetArrayVK.h>
#include <moai-gfx-vk/MOAIDrawingCommandVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIDrawingCommand
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawingCommandVK::Execute ( MOAIAbstractDrawingAPICallback* callback, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) {

	if ( cmd < MOAIDrawingCmdEnumVK::BASE ) {
		MOAIDrawingCommand::Execute ( callback, cmd, rawParam );
		return;
	}

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIShaderVK* shader = MOAICast < MOAIShaderVK >( gfxMgr.GetShader ());
	MOAIDescriptorSetArrayVK* descriptorSetArray = shader ? shader->GetDescriptorSetArray () : NULL;

	switch ( cmd ) {
	
		case MOAIDrawingCmdEnumVK::LOAD_DESCRIPTOR_ELEMENT_VK: {
			if ( descriptorSetArray ) {
				const MOAIDrawingParamVK::LoadDescriptorElement* param = ( const MOAIDrawingParamVK::LoadDescriptorElement* )rawParam;
				MOAIDescriptorSetVK& descriptorSet = descriptorSetArray->GetDescriptorSet ( param->mDescriptorSetID );
				descriptorSet.SetDescriptor ( param->mBinding, param->mArrayElement, param->mElement );
			}
			break;
		}
		
		case MOAIDrawingCmdEnumVK::LOAD_DESCRIPTOR_FROM_TEXTURE_UNIT_VK: {
			if ( descriptorSetArray ) {
				const MOAIDrawingParamVK::LoadDescriptorFromTextureUnit* param = ( const MOAIDrawingParamVK::LoadDescriptorFromTextureUnit* )rawParam;
				MOAITextureVK* texture = MOAICast < MOAITextureVK >( gfxMgr.GetTexture ( param->mTextureUnit ));
				MOAIDescriptorSetVK& descriptorSet = descriptorSetArray->GetDescriptorSet ( param->mDescriptorSetID );
				descriptorSet.SetDescriptor ( param->mBinding, param->mArrayElement, texture );
			}
			break;
		}
	
		case MOAIDrawingCmdEnumVK::LOAD_SHADER_UNIFORM_VK: {
			const MOAIDrawingParamVK::LoadShaderUniform* param = ( const MOAIDrawingParamVK::LoadShaderUniform* )rawParam;
			MOAIDrawingCommandVK::ExecuteSetUniformVK ( gfxMgr, *param );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDrawingCommandVK::ExecuteSetUniformVK ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamVK::LoadShaderUniform& param ) {
	UNUSED ( gfxMgr );
	UNUSED ( param );

//	MOAIShader* shader = gfxMgr.GetShader ();
//	if ( !shader ) return;
//
//	MOAIUniformHandle uniform = shader->GetUniformHandle ( param.mTargetUniformID, param.mTargetUniformIndex );
//	if ( !uniform.IsValid ()) return;
//
//	if ( param.mPipelineGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {
//
//		uniform.SetValue ( gfxMgr.GetMtx ( param.mPipelineGlobalID ));
//	}
//	else {
//
//		switch (( ZLSize )param.mPipelineGlobalID ) {
//
//			case MOAIGfxMgr::PEN_COLOR:
//
//				uniform.SetValue ( gfxMgr.GetFinalColor ());
//				break;
//
//			case MOAIGfxMgr::VIEW_HALF_HEIGHT:
//
//				uniform.SetValue ( gfxMgr.GetViewRect ().Height () * 0.5f );
//				break;
//
//			case MOAIGfxMgr::VIEW_HALF_WIDTH: {
//
//				uniform.SetValue ( gfxMgr.GetViewRect ().Width () * 0.5f );
//				break;
//			}
//			case MOAIGfxMgr::VIEW_HEIGHT:
//
//				uniform.SetValue ( gfxMgr.GetViewRect ().Height ());
//				break;
//
//			case MOAIGfxMgr::VIEW_WIDTH:
//
//				uniform.SetValue ( gfxMgr.GetViewRect ().Width ());
//				break;
//		}
//	}
}
