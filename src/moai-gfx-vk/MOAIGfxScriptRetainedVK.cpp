// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDrawingAPIVK.h>
#include <moai-gfx-vk/MOAIGfxScriptRetainedVK.h>

//================================================================//
// MOAIGfxScriptRetainedVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetainedVK::MOAIGfxScriptRetainedVK () {

	RTTI_BEGIN ( MOAIGfxScriptRetainedVK )
		RTTI_EXTEND ( MOAIGfxScriptRetained )
		RTTI_EXTEND ( MOAIAbstractDrawingObjectVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptRetainedVK::~MOAIGfxScriptRetainedVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetainedVK::MOAIGfxScriptRetained_Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingAPIVK::Execute ( callable, cmd, rawParam );
}
