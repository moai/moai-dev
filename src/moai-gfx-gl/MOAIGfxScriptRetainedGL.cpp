// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawingCommandGL.h>
#include <moai-gfx-gl/MOAIGfxScriptRetainedGL.h>

//================================================================//
// MOAIGfxScriptRetainedGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetainedGL::MOAIGfxScriptRetainedGL () {

	RTTI_BEGIN ( MOAIGfxScriptRetainedGL )
		RTTI_EXTEND ( MOAIGfxScriptRetained )
		RTTI_EXTEND ( MOAIAbstractDrawingAPIObjectGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptRetainedGL::~MOAIGfxScriptRetainedGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetainedGL::MOAIGfxScriptRetained_Execute ( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingCommandGL::Execute ( callable, cmd, rawParam );
}
