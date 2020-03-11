// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAITextureVK
//================================================================//

//----------------------------------------------------------------//
MOAITextureVK::MOAITextureVK () {
	
	RTTI_BEGIN ( MOAITextureVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAITextureVK >)
		RTTI_EXTEND ( MOAITexture )
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureVK::~MOAITextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
