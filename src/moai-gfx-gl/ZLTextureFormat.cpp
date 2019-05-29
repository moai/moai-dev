// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAITextureBaseGL.h>
#include <moai-gfx-gl/ZLTextureFormat.h>

//================================================================//
// ZLTextureFormat
//================================================================//

//----------------------------------------------------------------//
void ZLTextureFormat::CleanupTexture ( MOAITextureBaseGL& texture ) {

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
void ZLTextureFormat::SetGLTexture ( MOAITextureBaseGL& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
