// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawingAPIGL.h>
#include <moai-gfx-gl/MOAIGfxScriptGL.h>

//================================================================//
// MOAIGfxScriptGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptGL::MOAIGfxScriptGL () {

	RTTI_BEGIN ( MOAIGfxScriptGL )
		RTTI_EXTEND ( MOAIGfxScript )
		RTTI_EXTEND ( MOAIAbstractDrawingObjectGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptGL::~MOAIGfxScriptGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptGL::MOAIGfxScript_Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingAPIGL::Execute ( callable, cmd, rawParam );
}
