// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>

//================================================================//
// MOAIDrawGL
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawGL::ExecuteSetUniformGL ( MOAIGfxMgr& gfxMgr, const MOAIDrawingParamGL::LoadShaderUniform& param ) {

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

//----------------------------------------------------------------//
MOAIDrawGL::MOAIDrawGL () {

	RTTI_BEGIN ( MOAIDrawGL )
		RTTI_EXTEND ( MOAIAbstractCmdHandler )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDrawGL::~MOAIDrawGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawGL::MOAIAbstractCmdHandler_HandleCommand ( u32 cmd, const void* param ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawAPIGL::LOAD_SHADER_UNIFORM_GL: {
			this->ExecuteSetUniformGL ( gfxMgr, *( const MOAIDrawingParamGL::LoadShaderUniform* )param );
			break;
		}
	}
}
