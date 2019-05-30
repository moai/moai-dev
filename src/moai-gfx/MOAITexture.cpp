// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
u32 MOAITexture::GetHeight () const {

	return this->MOAITexture_GetHeight ();
}

//----------------------------------------------------------------//
u32 MOAITexture::GetWidth () const {

	return this->MOAITexture_GetWidth ();
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () {

	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}
