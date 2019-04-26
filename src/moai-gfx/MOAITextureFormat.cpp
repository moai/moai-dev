// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAITextureBase.h>
#include <moai-gfx/MOAITextureFormat.h>

//================================================================//
// MOAITextureFormat
//================================================================//

//----------------------------------------------------------------//
void MOAITextureFormat::CleanupTexture ( MOAITextureBase& texture ) {

	texture.CleanupOnError ();
}

//----------------------------------------------------------------//
MOAITextureFormat::MOAITextureFormat () {

	RTTI_SINGLE ( MOAIImageFormat )
}

//----------------------------------------------------------------//
MOAITextureFormat::~MOAITextureFormat () {
}

//----------------------------------------------------------------//
void MOAITextureFormat::SetGLTexture ( MOAITextureBase& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
