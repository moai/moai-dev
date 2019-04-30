// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLTextureBase.h>
#include <zl-gfx/ZLTextureFormat.h>

//================================================================//
// ZLTextureFormat
//================================================================//

//----------------------------------------------------------------//
void ZLTextureFormat::CleanupTexture ( ZLTextureBase& texture ) {

	texture.CleanupOnError ();
}

//----------------------------------------------------------------//
ZLTextureFormat::ZLTextureFormat () {

	RTTI_SINGLE ( ZLImageFormat )
}

//----------------------------------------------------------------//
ZLTextureFormat::~ZLTextureFormat () {
}

//----------------------------------------------------------------//
void ZLTextureFormat::SetGLTexture ( ZLTextureBase& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
