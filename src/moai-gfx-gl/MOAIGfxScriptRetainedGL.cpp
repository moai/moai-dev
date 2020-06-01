// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawingAPIGL.h>
#include <moai-gfx-gl/MOAIGfxScriptRetainedGL.h>

//================================================================//
// MOAIGfxScriptRetainedGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetainedGL::MOAIGfxScriptRetainedGL () {

	RTTI_BEGIN ( MOAIGfxScriptRetainedGL )
		RTTI_EXTEND ( MOAIGfxScriptRetained )
		RTTI_EXTEND ( MOAIAbstractDrawingLuaAPIGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptRetainedGL::~MOAIGfxScriptRetainedGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetainedGL::MOAIGfxScriptRetained_Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingAPIGL::Execute ( callable, cmd, rawParam );
}
