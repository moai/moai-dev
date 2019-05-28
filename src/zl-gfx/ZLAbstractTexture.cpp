// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLAbstractTexture.h>

//================================================================//
// ZLAbstractTexture
//================================================================//

//----------------------------------------------------------------//
u32 ZLAbstractTexture::GetHeight () const {

	return this->ZLAbstractTexture_GetHeight ();
}

//----------------------------------------------------------------//
u32 ZLAbstractTexture::GetWidth () const {

	return this->ZLAbstractTexture_GetWidth ();
}

//----------------------------------------------------------------//
ZLAbstractTexture::ZLAbstractTexture () {

	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
ZLAbstractTexture::~ZLAbstractTexture () {
}
