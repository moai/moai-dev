// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawingAPIGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>

//================================================================//
// MOAIDrawingAPI
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawingAPIGL::Execute ( MOAIAbstractGfxScriptCallback* callback, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) {

	if ( cmd < MOAIDrawingCmdEnumGL::BASE ) {
		MOAIDrawingAPI::Execute ( callback, cmd, rawParam );
		return;
	}

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawingCmdEnumGL::LOAD_SHADER_UNIFORM_GL: {
			const MOAIDrawingParamGL::LoadShaderUniform* param = ( const MOAIDrawingParamGL::LoadShaderUniform* )rawParam;
			MOAIDrawingAPIGL::ExecuteSetUniformGL ( gfxMgr, *param );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDrawingAPIGL::ExecuteSetUniformGL ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamGL::LoadShaderUniform& param ) {

	MOAIShaderGL* shader = MOAICast < MOAIShaderGL >( gfxMgr.GetShader ());
	if ( !shader ) return;
	
	MOAIUniformHandle uniform = shader->GetUniformHandle ( param.mTargetUniformID, param.mTargetUniformIndex );
	if ( !uniform.IsValid ()) return;

	if ( param.mPipelineGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {

		uniform.SetValue ( gfxMgr.GetMtx ( param.mPipelineGlobalID ));
	}
	else {

		switch (( ZLSize )param.mPipelineGlobalID ) {

			case MOAIGfxMgr::PEN_COLOR:

				uniform.SetValue ( gfxMgr.GetFinalColor ());
				break;

			case MOAIGfxMgr::VIEW_HALF_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height () * 0.5f );
				break;

			case MOAIGfxMgr::VIEW_HALF_WIDTH: {

				uniform.SetValue ( gfxMgr.GetViewRect ().Width () * 0.5f );
				break;
			}
			case MOAIGfxMgr::VIEW_HEIGHT:

				uniform.SetValue ( gfxMgr.GetViewRect ().Height ());
				break;

			case MOAIGfxMgr::VIEW_WIDTH:

				uniform.SetValue ( gfxMgr.GetViewRect ().Width ());
				break;
		}
	}
}
