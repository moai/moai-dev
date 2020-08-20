// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>

//================================================================//
// MOAIAbstractGfxScriptCallback
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptCallback::Flush () {

	if ( !this->mDidDraw ) {
		this->Invoke ();
	}
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptCallback::Invoke () {

	switch ( this->mPhase ) {
	
		case 0: {
			MOAIShader* shader = MOAIGfxMgr::Get ().GetShader ();
			if ( shader ) {
			
				this->mPhase = 1;
				
				MOAIGfxScript* gfxScript = shader->GetGfxScript ();
				if ( gfxScript ) {
					gfxScript->ExecuteBytecode ( this );
				}
				else {
					this->Invoke ();
				}
				this->mPhase = 0;
			}
			break;
		}
		
		case 1: {
			this->MOAIAbstractGfxScriptCallback_Call ();
			this->mDidDraw = true;
			break;
		}
	}
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptCallback::MOAIAbstractGfxScriptCallback () :
	mPhase ( 0 ),
	mDidDraw ( false ) {
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptCallback::~MOAIAbstractGfxScriptCallback () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptCallback::Reset () {

	this->mPhase = 0;
	this->mDidDraw = false;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractGfxScriptCallback::MOAIAbstractCmdStreamFilter_FilterCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
	UNUSED ( param );
	UNUSED ( paramSize );

	if ( cmd == MOAIDrawAPI::DRAW ) {
		this->Invoke ();
		return true;
	}
	return false;
}
