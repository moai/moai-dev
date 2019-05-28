// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLTextureBaseGL.h>
#include <zl-gfx/ZLTextureFormat.h>

//================================================================//
// ZLTextureFormat
//================================================================//

//----------------------------------------------------------------//
void ZLTextureFormat::CleanupTexture ( ZLTextureBaseGL& texture ) {

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
void ZLTextureFormat::SetGLTexture ( ZLTextureBaseGL& texture, const ZLGfxHandleGL& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
