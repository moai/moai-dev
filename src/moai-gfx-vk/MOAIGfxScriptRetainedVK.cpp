// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDrawingCommandVK.h>
#include <moai-gfx-vk/MOAIGfxScriptRetainedVK.h>

//================================================================//
// MOAIGfxScriptRetainedVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetainedVK::MOAIGfxScriptRetainedVK () {

	RTTI_BEGIN ( MOAIGfxScriptRetainedVK )
		RTTI_EXTEND ( MOAIGfxScriptRetained )
		RTTI_EXTEND ( MOAIAbstractDrawingAPIObjectVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptRetainedVK::~MOAIGfxScriptRetainedVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetainedVK::MOAIGfxScriptRetained_Execute ( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingCommandVK::Execute ( callable, cmd, rawParam );
}
