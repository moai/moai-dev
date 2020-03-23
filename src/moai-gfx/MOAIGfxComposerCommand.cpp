// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxComposer.h>
#include <moai-gfx/MOAIAbstractGfxComposerCallable.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxComposerCommand.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIGfxComposerCommand
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerCommand::Execute ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ cmd, const void* rawParam ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIGfxComposerCmdEnum::CALL:
			callable.Call ();
			break;
		
		case MOAIGfxComposerCmdEnum::CALL_FROM_SHADER: {
		
			MOAIShader* shader = gfxMgr.GetShader ();
			MOAIAbstractGfxComposer* composer = shader ? shader->GetComposer () : NULL;
		
			if ( composer ) {
				composer->Execute ( callable, MOAIGfxComposerCmdEnum::CALL );
			}
			else {
				callable.Call ();
			}
			break;
		}
		
		case MOAIGfxComposerCmdEnum::SET_SHADER: {
			const MOAIGfxComposerParam::SetShader* param = ( const MOAIGfxComposerParam::SetShader* )rawParam;
			gfxMgr.SetShader ( param->mShader );
			break;
		}
		
		case MOAIGfxComposerCmdEnum::SET_TEXTURE: {
			const MOAIGfxComposerParam::SetTexture* param = ( const MOAIGfxComposerParam::SetTexture* )rawParam;
			gfxMgr.SetTexture ( param->mTexture, param->mTextureUnit );
			break;
		}
		
		case MOAIGfxComposerCmdEnum::SET_UNIFORM: {
			const MOAIGfxComposerParam::SetUniform* param = ( const MOAIGfxComposerParam::SetUniform* )rawParam;
			MOAIGfxComposerCommand::ExecuteUniform ( *param );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerCommand::ExecuteUniform ( const MOAIGfxComposerParam::SetUniform& param ) {

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
