// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ar/MOAIMarkerMgr.h>
#include <moai-ar/MOAIVideoTexture.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_ARTOOLKIT

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIVideoTexture
//================================================================//

//----------------------------------------------------------------//
void MOAIVideoTexture::Init ( u32 videoPlane, AR_PIXEL_FORMAT format ) {

	this->mDebugName.write ( "(stacktrace from MOAIVideoTexture %d)", videoPlane );

	this->mFormat = format;
	this->mVideoPlane = videoPlane;

	MOAIMarkerMgr& markerMgr = MOAIMarkerMgr::Get ();

	this->mWidth = markerMgr.GetVideoWidth ();
	this->mHeight = markerMgr.GetVideoHeight ();

	bool biPlanar;

	switch ( this->mFormat ) {
        case AR_PIXEL_FORMAT_420f:
        case AR_PIXEL_FORMAT_420v:
        case AR_PIXEL_FORMAT_NV21:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
            this->mGLPixelSize						= 1;
            this->mPixelStoreUnpackAlignment		= 1;
			biPlanar								= true;
            break;
			
        case AR_PIXEL_FORMAT_RGBA:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
            this->mGLPixelSize						= 4;
            this->mPixelStoreUnpackAlignment		= 4;
			biPlanar								= false;
            break;
			
        case AR_PIXEL_FORMAT_RGB:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_RGB;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_RGB;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
            this->mGLPixelSize						= 3;
            this->mPixelStoreUnpackAlignment		= 1;
			biPlanar								= false;
            break;
			
        case AR_PIXEL_FORMAT_MONO:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
            this->mGLPixelSize						= 1;
            this->mPixelStoreUnpackAlignment		= 1;
			biPlanar								= false;
            break;

        case AR_PIXEL_FORMAT_RGB_565:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_RGB;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_RGB;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
            this->mGLPixelSize						= 2;
            this->mPixelStoreUnpackAlignment		= 2;
			biPlanar								= false;
            break;

        case AR_PIXEL_FORMAT_RGBA_5551:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
            this->mGLPixelSize						= 2;
            this->mPixelStoreUnpackAlignment		= 2;
			biPlanar								= false;
            break;

        case AR_PIXEL_FORMAT_RGBA_4444:
            this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_RGBA;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
            this->mGLPixelSize						= 2;
            this->mPixelStoreUnpackAlignment		= 2;
			biPlanar								= false;
            break;

        default:
            break;
    }

	if ( this->mVideoPlane == VIDEO_TEXTURE_PLANE_1 ) {
		if ( biPlanar ) {
			this->mWidth							= this->mWidth / 2;
			this->mHeight							= this->mHeight / 2;
			this->mGLInternalFormat					= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE_ALPHA;
			this->mGLFormat							= ZLGfxEnum::PIXEL_FORMAT_LUMINANCE_ALPHA;
            this->mGLPixelType						= ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
			this->mGLPixelSize						= 2;
		}
		else {
			this->mUsePlane = false;
			return;
		}
	}

	this->mTextureSize = this->mWidth * this->mHeight * this->mGLPixelSize;
	this->mBuffer.Alloc ( this->mTextureSize );
	this->mUsePlane = true;
}

//----------------------------------------------------------------//
MOAIVideoTexture::MOAIVideoTexture () :
	mFormat ( AR_PIXEL_FORMAT_INVALID ),
	mUsePlane ( false ) {
	
	RTTI_BEGIN ( MOAIVideoTexture )
		RTTI_EXTEND ( MOAITextureBaseGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVideoTexture::~MOAIVideoTexture () {
}

//----------------------------------------------------------------//
void MOAIVideoTexture::UpdateBuffer ( void* buffer ) {

	this->mBuffer.SetCursor ( 0 );
	this->mBuffer.WriteBytes ( buffer, this->mBuffer.GetSize ());
	this->FinishInit ();
}

//================================================================//
// MOAIVideoTexture
//================================================================//

//----------------------------------------------------------------//
bool MOAIVideoTexture::MOAIGfxResourceGL_OnGPUCreate () {

	if ( !MOAIGfxMgr::Get ().GetHasContext ()) return false;
	if ( !this->mUsePlane ) return false;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mGLTexID = gfx.CreateTexture ();
	if ( !this->mGLTexID ) return false;

	gfx.SetTexture ( this->mGLTexID );

	gfx.TexImage2D (
		0,
		this->mGLInternalFormat,
		this->mWidth,  
		this->mHeight,  
		this->mGLInternalFormat,
		this->mGLPixelType,
		this->mBuffer.GetSharedConstBuffer ()
	);

	MOAIGfxMgr::Get ().LogErrors ();

	return true;
}

//----------------------------------------------------------------//
bool MOAIVideoTexture::MOAIGfxResourceGL_OnGPUUpdate () {

	if ( !MOAIGfxMgr::Get ().GetHasContext ()) return false;
	if ( !this->mUsePlane ) return false;
	if ( !this->mGLTexID ) return false;
	
	AR2VideoBufferT* videoBuffer = MOAIMarkerMgr::Get ().GetVideoBuffer ();

	if ( ! videoBuffer ) return false;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	gfx.SetTexture ( this->mGLTexID );

	gfx.TexSubImage2D (
		0,
		0,
		0,
		this->mWidth,
		this->mHeight,
		this->mGLInternalFormat,
		this->mGLPixelType,  
		this->mBuffer.GetSharedConstBuffer ()
	);

	MOAIGfxMgr::Get ().LogErrors ();

	return MOAITextureBaseGL::MOAIGfxResourceGL_OnGPUUpdate ();
}

#endif
