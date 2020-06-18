// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDrawingAPIVK.h>
#include <moai-gfx-vk/MOAIGfxScriptVK.h>

//================================================================//
// MOAIGfxScriptVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptVK::MOAIGfxScriptVK () {

	RTTI_BEGIN ( MOAIGfxScriptVK )
		RTTI_EXTEND ( MOAIGfxScript )
		RTTI_EXTEND ( MOAIAbstractDrawingObjectVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptVK::~MOAIGfxScriptVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptVK::MOAIGfxScript_Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingAPIVK::Execute ( callable, cmd, rawParam );
}
