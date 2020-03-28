// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDrawingCommandVK.h>

//================================================================//
// MOAIDrawingCommand
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawingCommandVK::Execute ( MOAIAbstractDrawingAPICallback* callback, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) {

	if ( cmd < MOAIDrawingCmdEnumVK::BASE ) {
		MOAIDrawingCommandVK::Execute ( callback, cmd, rawParam );
		return;
	}

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawingCmdEnumVK::LOAD_SHADER_UNIFORM_VK: {
			const MOAIDrawingParam::SetUniform* param = ( const MOAIDrawingParam::SetUniform* )rawParam;
			MOAIDrawingCommandVK::ExecuteSetUniformVK ( gfxMgr, *param );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDrawingCommandVK::ExecuteSetUniformVK ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::SetUniform& param ) {
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
