// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAITextureGL.h>
#include <moai-gfx-gl/ZLTextureFormat.h>

//================================================================//
// ZLTextureFormat
//================================================================//

//----------------------------------------------------------------//
void ZLTextureFormat::CleanupTexture ( MOAITextureGL& texture ) {

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
void ZLTextureFormat::SetGLTexture ( MOAITextureGL& texture, const ZLGfxHandle& glTexture, ZLGfxEnum::_ internalFormat, ZLGfxEnum::_ pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
