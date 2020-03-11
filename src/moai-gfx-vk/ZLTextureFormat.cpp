// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/ZLTextureFormat.h>

//================================================================//
// ZLTextureFormat
//================================================================//

//----------------------------------------------------------------//
void ZLTextureFormat::CleanupTexture ( MOAITextureVK& texture ) {

	texture.CleanupOnError ();
}

//----------------------------------------------------------------//
ZLTextureFormat::ZLTextureFormat () {
	
	RTTI_BEGIN ( ZLTextureFormat )
		RTTI_EXTEND ( ZLImageFormat )
	RTTI_END
}

//----------------------------------------------------------------//
ZLTextureFormat::~ZLTextureFormat () {
}

//----------------------------------------------------------------//
void ZLTextureFormat::SetGLTexture ( MOAITextureVK& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetGLTexture ( glTexture, internalFormat, pixelType, textureSize );
}
