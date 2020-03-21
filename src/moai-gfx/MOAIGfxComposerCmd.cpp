// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxComposerCallable.h>
#include <moai-gfx/MOAIGfxComposer.h>
#include <moai-gfx/MOAIGfxComposerCmd.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIGfxComposerCmd
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxComposerCmd::Execute ( MOAIAbstractGfxComposerCallable& callable ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( this->mType ) {
	
		case MOAIGfxComposerCmdEnum::CALL:
		case MOAIGfxComposerCmdEnum::CALL_FROM_SHADER:
			MOAIGfxComposerCmd::ExecuteCall ( callable, this->mType );
			return true;
		
		case MOAIGfxComposerCmdEnum::SHADER:
			gfxMgr.SetShader ( this->mShader );
			break;
		
		case MOAIGfxComposerCmdEnum::TEXTURE:
			gfxMgr.SetTexture ( this->mTexture, this->mTextureUnit );
			break;
		
		case MOAIGfxComposerCmdEnum::UNIFORM:
			this->ExecuteUniform ();
			break;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxComposerCmd::ExecuteCall ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {

	switch ( callCommand ) {
	
		case MOAIGfxComposerCmdEnum::CALL:
			callable.Call ();
			break;
		
		case MOAIGfxComposerCmdEnum::CALL_FROM_SHADER: {
			
//			MOAIMaterialMgr::Get ().LoadGfxState ();
		
			MOAIShader* shader = MOAIGfxMgr::Get ().GetShader ();
			MOAIGfxComposer* composer = shader ? shader->GetComposer () : NULL;
		
			if ( composer ) {
				composer->Execute ( callable, MOAIGfxComposerCmdEnum::CALL );
			}
			else {
				callable.Call ();
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerCmd::ExecuteUniform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIShader* shader = gfxMgr.GetShader ();
	if ( !shader ) return;
	
	MOAIUniformHandle uniform = shader->GetUniformHandle ( this->mTargetUniformID, this->mTargetUniformIndex );
	if ( !uniform.IsValid ()) return;

	if ( this->mPipelineGlobalID < MOAIGfxMgr::TOTAL_MATRICES ) {

		uniform.SetValue ( gfxMgr.GetMtx ( this->mPipelineGlobalID ));
	}
	else {

		switch (( ZLSize )this->mPipelineGlobalID ) {

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
MOAIGfxComposerCmd::MOAIGfxComposerCmd () :
	mType ( MOAIGfxComposerCmdEnum::NONE ),
	mAddressingModeFrom ( MOAIGfxComposerAddrModeEnum::NONE ),
	mAddressingModeTo ( MOAIGfxComposerAddrModeEnum::NONE ),
	mTextureUnit ( 0 ),
	mPipelineGlobalID ( ZLIndexOp::INVALID ),
	mTargetUniformID ( ZLIndexOp::INVALID ),
	mTargetUniformIndex ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGfxComposerCmd::MOAIGfxComposerCmd ( MOAIGfxComposerCmdEnum::_ type, MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to ) :
	mType ( type ),
	mAddressingModeFrom ( from ),
	mAddressingModeTo ( to ),
	mTextureUnit ( 0 ),
	mPipelineGlobalID ( ZLIndexOp::INVALID ),
	mTargetUniformID ( ZLIndexOp::INVALID ),
	mTargetUniformIndex ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGfxComposerCmd::~MOAIGfxComposerCmd () {
}
