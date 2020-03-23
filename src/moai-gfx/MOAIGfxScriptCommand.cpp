// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractGfxScriptCallable.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxScriptCommand.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIGfxScriptCommand
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptCommand::Execute ( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ cmd, const void* rawParam ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIGfxScriptCmdEnum::CALL:
			callable.Call ();
			break;
		
		case MOAIGfxScriptCmdEnum::CALL_FROM_SHADER: {
		
			MOAIShader* shader = gfxMgr.GetShader ();
			MOAIAbstractGfxScript* composer = shader ? shader->GetComposer () : NULL;
		
			if ( composer ) {
				composer->Execute ( callable, MOAIGfxScriptCmdEnum::CALL );
			}
			else {
				callable.Call ();
			}
			break;
		}
		
		case MOAIGfxScriptCmdEnum::SET_SHADER: {
			const MOAIGfxScriptParam::SetShader* param = ( const MOAIGfxScriptParam::SetShader* )rawParam;
			gfxMgr.SetShader ( param->mShader );
			break;
		}
		
		case MOAIGfxScriptCmdEnum::SET_TEXTURE: {
			const MOAIGfxScriptParam::SetTexture* param = ( const MOAIGfxScriptParam::SetTexture* )rawParam;
			gfxMgr.SetTexture ( param->mTexture, param->mTextureUnit );
			break;
		}
		
		case MOAIGfxScriptCmdEnum::SET_UNIFORM: {
			const MOAIGfxScriptParam::SetUniform* param = ( const MOAIGfxScriptParam::SetUniform* )rawParam;
			MOAIGfxScriptCommand::ExecuteUniform ( *param );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptCommand::ExecuteUniform ( const MOAIGfxScriptParam::SetUniform& param ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIShader* shader = gfxMgr.GetShader ();
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
