// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/ZLTexture.h>
#include <moai-sim/strings.h>

//================================================================//
// ZLTexture
//================================================================//

//----------------------------------------------------------------//
void ZLTexture::CheckFilterModes ( int min, int mag ) {
	UNUSED ( min );
	UNUSED ( mag );

	#ifdef _DEBUG
		switch ( min ) {
			case ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR:
			case ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST:
			case ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR:
			case ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST:
		
				if ( !(( mag == ZGL_SAMPLE_LINEAR ) || ( mag == ZGL_SAMPLE_NEAREST ))) {

					ZLLog_Warning ( "WARNING: possibly incompatible filter modes; MIPMAP not supported for mag filter\n" );
				}
				break;
				
			default:
				break;
		}
	#endif
}

//----------------------------------------------------------------//
void ZLTexture::CleanupOnError () {

	this->mTextureSize = 0;
	MOAIGfxMgr::GetDrawingAPI ().DeleteResource ( this->mGLTexture );
	this->mWidth = 0;
	this->mHeight = 0;
	this->mTextureSize = 0;
}

//----------------------------------------------------------------//
bool ZLTexture::CreateTextureFromImage ( MOAIImage& srcImage ) {

	if ( !MOAIGfxMgr::Get ().GetHasContext ()) return false;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	MOAIImage altImage;

	ZLColor::ColorFormat colorFormat = srcImage.GetColorFormat ();
	if ( colorFormat == ZLColor::CLR_FMT_UNKNOWN ) return false;

	if (( colorFormat == ZLColor::A_1 ) || ( colorFormat == ZLColor::A_4 )) {
		colorFormat = ZLColor::A_8;
	}
	
	if (( colorFormat != srcImage.GetColorFormat ()) || ( srcImage.GetPixelFormat () != MOAIImage::TRUECOLOR )) {
		//if ( !altImage.Convert ( srcImage, colorFormat, MOAIImage::TRUECOLOR )); // TODO: what was this? add error handling?
		altImage.Convert ( srcImage, colorFormat, MOAIImage::TRUECOLOR );
	}
	
	MOAIImage& image = altImage.IsOK () ? altImage : srcImage;
	if ( !image.IsOK ()) return false;

	//MOAIGfxMgr::Get ().ClearErrors ();
	this->mGLTexture = gfx.CreateTexture ();
	//if ( !this->mGLTexture ) return false;

	// get the dimensions before trying to get the OpenGL texture ID
	this->mWidth = image.GetWidth ();
	this->mHeight = image.GetHeight ();

	// warn if not a power of two (if we're supposed to generate mipmaps)
	if ( this->ShouldGenerateMipmaps () && !image.IsPow2 ()) {
		MOAILogF ( 0, ZLLog::LOG_WARNING, MOAISTRING_MOAITexture_NonPowerOfTwo_SDD, ( cc8* )this->mDebugName, this->mWidth, this->mHeight );
	}

	// GL_ALPHA
	// GL_RGB
	// GL_RGBA
	// GL_LUMINANCE
	// GL_LUMINANCE_ALPHA

	// GL_UNSIGNED_BYTE
	// GL_UNSIGNED_SHORT_5_6_5
	// GL_UNSIGNED_SHORT_4_4_4_4
	// GL_UNSIGNED_SHORT_5_5_5_1

	switch ( colorFormat ) {
		
		case ZLColor::A_8:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_ALPHA;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::LA_8:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::RGB_888:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGB;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::RGB_565:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGB;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
			break;
		
		case ZLColor::RGBA_5551:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
			break;
		
		case ZLColor::RGBA_4444:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
			break;
		
		case ZLColor::RGBA_8888:
			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
			break;
			
		default:
			this->CleanupOnError ();
			return false;
	}

	gfx.BindTexture ( this->mGLTexture );

	gfx.TexImage2D (
		0,
		this->mGLInternalFormat,
		this->mWidth,  
		this->mHeight,  
		this->mGLInternalFormat,
		this->mGLPixelType,
		image.GetBitmapBuffer ()
	);
	
	this->mTextureSize = image.GetBitmapSize ();

	// TODO: error handling
//	if ( MOAIGfxMgr::Get ().LogErrors ()) {
//		this->CleanupOnError ();
//		return false;
//	}
//	else if ( this->ShouldGenerateMipmaps ()) {
	
	if ( this->ShouldGenerateMipmaps ()) {
	
		u32 mipLevel = 1;
		
		MOAIImage mipmap;
		mipmap.Copy ( image );
		
		while ( mipmap.MipReduce ()) {
			
			gfx.TexImage2D (
				mipLevel++,  
				this->mGLInternalFormat,
				mipmap.GetWidth (),  
				mipmap.GetHeight (),  
				this->mGLInternalFormat,
				this->mGLPixelType,
				mipmap.GetBitmapBuffer ()
			);
			
			if ( MOAIGfxMgr::Get ().LogErrors ()) {
				this->CleanupOnError ();
				return false;
			}
			this->mTextureSize += mipmap.GetBitmapSize ();
		}
	}
	
	//MOAIGfxMgr::Get ().ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
	
	return true;
}

//----------------------------------------------------------------//
u32 ZLTexture::GetHeight () {
	return this->mHeight;
}

//----------------------------------------------------------------//
u32 ZLTexture::GetWidth () {
	return this->mWidth;
}

//----------------------------------------------------------------//
bool ZLTexture::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void ZLTexture::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLTexture::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {

	MOAIGfxMgr::Get ().ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
	ZLAbstractGfxResource::ZLGfxListener_OnGfxEvent ( event, userdata );
}

//----------------------------------------------------------------//
void ZLTexture::ZLAbstractGfxResource_OnGPUBind () {

	MOAIGfxMgr::GetDrawingAPI ().BindTexture ( this->mGLTexture );
}

//----------------------------------------------------------------//
void ZLTexture::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	if ( this->mGLTexture.CanBind () && MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().ReportTextureFree ( this->mDebugName, this->mTextureSize );
	}
	ZLGfxResourceClerk::DeleteOrDiscard ( this->mGLTexture, shouldDelete );
	this->mGLTexture = ZLGfxHandle (); // clear out the handle
}

//----------------------------------------------------------------//
void ZLTexture::ZLAbstractGfxResource_OnGPUUnbind () {

	MOAIGfxMgr::GetDrawingAPI ().BindTexture ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool ZLTexture::ZLAbstractGfxResource_OnGPUUpdate () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	gfx.TexParameteri ( ZGL_TEXTURE_WRAP_S, this->mWrap );
	gfx.TexParameteri ( ZGL_TEXTURE_WRAP_T, this->mWrap );
		
	gfx.TexParameteri ( ZGL_TEXTURE_MIN_FILTER, this->mMinFilter );
	gfx.TexParameteri ( ZGL_TEXTURE_MAG_FILTER, this->mMagFilter );
	
	return true;
}

//----------------------------------------------------------------//
void ZLTexture::SetFilter ( int filter ) {

	this->SetFilter ( filter, filter );
}

//----------------------------------------------------------------//
void ZLTexture::SetFilter ( int min, int mag ) {

	this->mMinFilter = min;
	this->mMagFilter = mag;
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void ZLTexture::SetGLTexture ( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {

	this->mGLTexture = glTexture;
	this->mGLInternalFormat = internalFormat;
	this->mGLPixelType = pixelType;
	this->mTextureSize = textureSize;

	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void ZLTexture::SetWrap ( int wrap ) {

	this->mWrap = wrap;
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
bool ZLTexture::ShouldGenerateMipmaps () {

	return (
		( this->mMinFilter == ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR ) ||
		( this->mMinFilter == ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST ) ||
		( this->mMinFilter == ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR ) ||
		( this->mMinFilter == ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST )
	);
}

//----------------------------------------------------------------//
bool ZLTexture::UpdateTextureFromImage ( MOAIImage& image, ZLIntRect rect ) {

	// TODO: what happens when image is an unsupported format?

	// if we need to generate mipmaps or the dimensions have changed, clear out the old texture
	if ( this->ShouldGenerateMipmaps () || ( this->mWidth != image.GetWidth ()) || ( this->mHeight != image.GetHeight ())) {
	
		MOAIGfxMgr::Get ().ReportTextureFree ( this->mDebugName, this->mTextureSize );
		ZLGfxResourceClerk::DeleteOrDiscard ( this->mGLTexture, false );
		
		if ( this->CreateTextureFromImage ( image )) {
			MOAIGfxMgr::Get ().ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
			return true;
		}
		return false;
	}
	
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
	// if the texture exists just update the sub-region
	// otherwise create a new texture from the image
	if ( this->mGLTexture.CanBind ()) {

		gfx.BindTexture ( this->mGLTexture );

		rect.Bless ();
		ZLIntRect imageRect = image.GetRect ();
		imageRect.Clip ( rect );
		
		ZLSharedConstBuffer* bitmapBuffer = image.GetBitmapBuffer ();
		
		MOAIImage subImage;
		if (( this->mWidth != ( u32 )rect.Width ()) || ( this->mHeight != ( u32 )rect.Height ())) {
			subImage.GetSubImage ( image, rect ); // TODO: need to convert to correct format for texture
			bitmapBuffer = subImage.GetBitmapBuffer ();
		}

		gfx.TexSubImage2D (
			0,
			rect.mXMin,
			rect.mYMin,
			rect.Width (),
			rect.Height (),
			this->mGLInternalFormat,
			this->mGLPixelType,  
			bitmapBuffer
		);
		
		MOAIGfxMgr::Get ().LogErrors ();
		
		return true;
	}
	
	return false;
}

//----------------------------------------------------------------//
ZLTexture::ZLTexture () :
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( ZGL_SAMPLE_LINEAR ),
	mMagFilter ( ZGL_SAMPLE_NEAREST ),
	mWrap ( ZGL_WRAP_MODE_CLAMP ),
	mTextureSize ( 0 ) {
}

//----------------------------------------------------------------//
ZLTexture::~ZLTexture () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
}

