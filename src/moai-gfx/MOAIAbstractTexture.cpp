// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractTexture.h>

//================================================================//
// MOAIAbstractTexture
//================================================================//

//----------------------------------------------------------------//
u32 MOAIAbstractTexture::GetHeight () const {

	return this->ZLAbstractTexture_GetHeight ();
}

//----------------------------------------------------------------//
u32 MOAIAbstractTexture::GetWidth () const {

	return this->ZLAbstractTexture_GetWidth ();
}

//----------------------------------------------------------------//
MOAIAbstractTexture::MOAIAbstractTexture () {

	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
MOAIAbstractTexture::~MOAIAbstractTexture () {
}
