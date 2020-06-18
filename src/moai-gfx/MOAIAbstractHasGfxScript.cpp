// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractHasGfxScript.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIDrawingAPI.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScript.h>

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractHasGfxScript::AffirmGfxScript () {

	return this->MOAIAbstractHasGfxScript_AffirmGfxScript ();
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::MOAIAbstractHasGfxScript () {

	RTTI_BEGIN ( MOAIAbstractHasGfxScript )
		RTTI_EXTEND ( MOAIAbstractDrawingObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::~MOAIAbstractHasGfxScript () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::RunScript ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {
	
	this->AffirmGfxScript ().MOAIAbstractGfxScript_RunScript ( callable, callCommand );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {

	this->AffirmGfxScript ().MOAIAbstractGfxScript_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	this->AffirmGfxScript ().MOAIAbstractGfxScript_SubmitCommand ( cmd, param, size );
}
