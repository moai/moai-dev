// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	getSize
//	@text	Returns the width and height of the texture's source image.
//			Avoid using the texture width and height to compute UV
//			coordinates from pixels, as this will prevent texture
//			resolution swapping.
//
//	@in		MOAITextureVK self
//	@out	number width
//	@out	number height
//*/
//int MOAITextureVK::_getSize ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAITextureVK, "U" )
//
//	self->DoCPUCreate ();
//
//	lua_pushnumber ( state, self->mWidth );
//	lua_pushnumber ( state, self->mHeight );
//
//	return 2;
//}
//
////----------------------------------------------------------------//
///**	@lua	release
//	@text	Releases any memory associated with the texture.
//
//	@in		MOAITextureVK self
//	@out	nil
//*/
//int MOAITextureVK::_release ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAITextureVK, "U" )
//
//	self->Destroy ();
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	setDebugName
//	@text	Set a name for the texture to use during memory logging.
//
//	@in		MOAITextureVK self
//	@in		string debugName
//	@out	nil
//*/
//int MOAITextureVK::_setDebugName ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAITextureVK, "U" )
//
//	self->mDebugName = state.GetValue < cc8* >( 2, "" );
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	setFilter
//	@text	Set default filtering mode for texture.
//
//	@in		MOAITextureVK self
//	@in		number min			One of MOAITextureVK.VK_LINEAR, MOAITextureVK.VK_LINEAR_MIPMAP_LINEAR, MOAITextureVK.VK_LINEAR_MIPMAP_NEAREST,
//								MOAITextureVK.VK_NEAREST, MOAITextureVK.VK_NEAREST_MIPMAP_LINEAR, MOAITextureVK.VK_NEAREST_MIPMAP_NEAREST
//	@opt	number mag			Defaults to value passed to 'min'.
//	@out	nil
//*/
//int MOAITextureVK::_setFilter ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAITextureVK, "UN" )
//
//	int min = state.GetValue < int >( 2, ZGL_SAMPLE_LINEAR );
//	int mag = state.GetValue < int >( 3, min );
//
//	MOAITextureVK::CheckFilterModes ( min, mag );
//
//	self->SetFilter ( min, mag );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	setWrap
//	@text	Set wrapping mode for texture.
//
//	@in		MOAITextureVK self
//	@in		boolean wrap		Texture will wrap if true, clamp if not.
//	@out	nil
//*/
//int MOAITextureVK::_setWrap ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAITextureVK, "UB" )
//
//	bool wrap = state.GetValue < bool >( 2, false );
//
//	self->mWrap = wrap ? ZGL_WRAP_MODE_REPEAT : ZGL_WRAP_MODE_CLAMP;
//
//	return 0;
//}

//================================================================//
// MOAITextureVK
//================================================================//

////----------------------------------------------------------------//
//void MOAITextureVK::CheckFilterModes ( int min, int mag ) {
//	UNUSED ( min );
//	UNUSED ( mag );
//
//	#ifdef _DEBUG
//		switch ( min ) {
//			case ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR:
//			case ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST:
//			case ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR:
//			case ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST:
//
//				if ( !(( mag == ZGL_SAMPLE_LINEAR ) || ( mag == ZGL_SAMPLE_NEAREST ))) {
//
//					ZLLog_Warning ( "WARNING: possibly incompatible filter modes; MIPMAP not supported for mag filter\n" );
//				}
//				break;
//
//			default:
//				break;
//		}
//	#endif
//}
//
////----------------------------------------------------------------//
//void MOAITextureVK::CleanupOnError () {
//
//	this->mGfxMgr->GetDrawingAPI ().DeleteResource ( this->mGLTexture );
//
//	this->mTextureSize = 0;
//	this->mWidth = 0;
//	this->mHeight = 0;
//	this->mTextureSize = 0;
//}
//
////----------------------------------------------------------------//
//bool MOAITextureVK::CreateTextureFromImage ( ZLImage& srcImage ) {
//
//	// TODO: ZLGfx
////	if ( !MOAIGfxMgr::Get ().GetHasContext ()) return false;
//
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	ZLImage altImage;
//
//	ZLColor::ColorFormat colorFormat = srcImage.GetColorFormat ();
//	if ( colorFormat == ZLColor::CLR_FMT_UNKNOWN ) return false;
//
//	if (( colorFormat == ZLColor::A_1 ) || ( colorFormat == ZLColor::A_4 )) {
//		colorFormat = ZLColor::A_8;
//	}
//
//	if (( colorFormat != srcImage.GetColorFormat ()) || ( srcImage.GetPixelFormat () != ZLImage::TRUECOLOR )) {
//		//if ( !altImage.Convert ( srcImage, colorFormat, ZLImage::TRUECOLOR )); // TODO: what was this? add error handling?
//		altImage.Convert ( srcImage, colorFormat, ZLImage::TRUECOLOR );
//	}
//
//	ZLImage& image = altImage.IsOK () ? altImage : srcImage;
//	if ( !image.IsOK ()) return false;
//
//	//MOAIGfxMgr::Get ().ClearErrors ();
//	this->mGLTexture = gfx.CreateTexture ();
//	//if ( !this->mGLTexture ) return false;
//
//	// get the dimensions before trying to get the OpenVK texture ID
//	this->mWidth = image.GetWidth ();
//	this->mHeight = image.GetHeight ();
//
//	// warn if not a power of two (if we're supposed to generate mipmaps)
////	if ( this->ShouldGenerateMipmaps () && !image.IsPow2 ()) {
////		MOAILogF ( 0, ZLLog::LOG_WARNING, MOAISTRING_MOAITexture_NonPowerOfTwo_SDD, ( cc8* )this->mDebugName, this->mWidth, this->mHeight );
////	}
//
//	// VK_ALPHA
//	// VK_RGB
//	// VK_RGBA
//	// VK_LUMINANCE
//	// VK_LUMINANCE_ALPHA
//
//	// VK_UNSIGNED_BYTE
//	// VK_UNSIGNED_SHORT_5_6_5
//	// VK_UNSIGNED_SHORT_4_4_4_4
//	// VK_UNSIGNED_SHORT_5_5_5_1
//
//	switch ( colorFormat ) {
//
//		case ZLColor::A_8:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_ALPHA;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
//			break;
//
//		case ZLColor::LA_8:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
//			break;
//
//		case ZLColor::RGB_888:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGB;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
//			break;
//
//		case ZLColor::RGB_565:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGB;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
//			break;
//
//		case ZLColor::RGBA_5551:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
//			break;
//
//		case ZLColor::RGBA_4444:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
//			break;
//
//		case ZLColor::RGBA_8888:
//			this->mGLInternalFormat = ZGL_PIXEL_FORMAT_RGBA;
//			this->mGLPixelType = ZGL_PIXEL_TYPE_UNSIGNED_BYTE;
//			break;
//
//		default:
//			this->CleanupOnError ();
//			return false;
//	}
//
//	gfx.BindTexture ( this->mGLTexture );
//
//	gfx.TexImage2D (
//		0,
//		this->mGLInternalFormat,
//		this->mWidth,
//		this->mHeight,
//		this->mGLInternalFormat,
//		this->mGLPixelType,
//		image.GetBitmapBuffer ()
//	);
//
//	this->mTextureSize = image.GetBitmapSize ();
//
//	// TODO: error handling
////	if ( MOAIGfxMgr::Get ().LogErrors ()) {
////		this->CleanupOnError ( gfx );
////		return false;
////	}
////	else if ( this->ShouldGenerateMipmaps ()) {
//
//	if ( this->ShouldGenerateMipmaps ()) {
//
//		u32 mipLevel = 1;
//
//		ZLImage mipmap;
//		mipmap.Copy ( image );
//
//		while ( mipmap.MipReduce ()) {
//
//			gfx.TexImage2D (
//				mipLevel++,
//				this->mGLInternalFormat,
//				mipmap.GetWidth (),
//				mipmap.GetHeight (),
//				this->mGLInternalFormat,
//				this->mGLPixelType,
//				mipmap.GetBitmapBuffer ()
//			);
//
//			if ( this->mGfxMgr->LogErrors ()) {
//				this->CleanupOnError ();
//				return false;
//			}
//			this->mTextureSize += mipmap.GetBitmapSize ();
//		}
//	}
//
//	//MOAIGfxMgr::Get ().ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
//
//	return true;
//}

//----------------------------------------------------------------//
MOAITextureVK::MOAITextureVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITexture )
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureVK::~MOAITextureVK () {

//	this->MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( true );
}

////----------------------------------------------------------------//
//void MOAITextureVK::SetGLTexture ( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize ) {
//
//	this->mGLTexture = glTexture;
//	this->mGLInternalFormat = internalFormat;
//	this->mGLPixelType = pixelType;
//	this->mTextureSize = textureSize;
//
//	this->ScheduleForGPUUpdate ();
//}
//
////----------------------------------------------------------------//
//bool MOAITextureVK::ShouldGenerateMipmaps () {
//
//	return (
//		( this->mMinFilter == ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR ) ||
//		( this->mMinFilter == ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST ) ||
//		( this->mMinFilter == ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR ) ||
//		( this->mMinFilter == ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST )
//	);
//}
//
////----------------------------------------------------------------//
//bool MOAITextureVK::UpdateTextureFromImage ( ZLImage& image, ZLIntRect rect ) {
//
//	// TODO: what happens when image is an unsupported format?
//
//	// if we need to generate mipmaps or the dimensions have changed, clear out the old texture
//	if ( this->ShouldGenerateMipmaps () || ( this->mWidth != image.GetWidth ()) || ( this->mHeight != image.GetHeight ())) {
//
//		this->mGfxMgr->ReportTextureFree ( this->mDebugName, this->mTextureSize );
//		this->mGfxMgr->DeleteOrDiscard ( this->mGLTexture, false );
//
//		if ( this->CreateTextureFromImage ( image )) {
//			this->mGfxMgr->ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
//			return true;
//		}
//		return false;
//	}
//
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	// if the texture exists just update the sub-region
//	// otherwise create a new texture from the image
//	if ( this->mGLTexture.CanBind ()) {
//
//		gfx.BindTexture ( this->mGLTexture );
//
//		rect.Bless ();
//		ZLIntRect imageRect = image.GetRect ();
//		imageRect.Clip ( rect );
//
//		ZLSharedConstBuffer* bitmapBuffer = image.GetBitmapBuffer ();
//
//		ZLImage subImage;
//		if (( this->mWidth != ( u32 )rect.Width ()) || ( this->mHeight != ( u32 )rect.Height ())) {
//			subImage.GetSubImage ( image, rect ); // TODO: need to convert to correct format for texture
//			bitmapBuffer = subImage.GetBitmapBuffer ();
//		}
//
//		gfx.TexSubImage2D (
//			0,
//			rect.mXMin,
//			rect.mYMin,
//			rect.Width (),
//			rect.Height (),
//			this->mGLInternalFormat,
//			this->mGLPixelType,
//			bitmapBuffer
//		);
//
//		this->mGfxMgr->LogErrors ();
//
//		return true;
//	}
//
//	return false;
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITextureVK::MOAIGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAIGfxResourceVK_OnGPUBind () {

//	this->mGfxMgr->GetDrawingAPI ().BindTexture ( this->mGLTexture );
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

//	if ( this->mGLTexture.CanBind ()) {
//		this->mGfxMgr->ReportTextureFree ( this->mDebugName, this->mTextureSize );
//	}
//	this->mGfxMgr->DeleteOrDiscard ( this->mGLTexture, shouldDelete );
//	this->mGLTexture = ZLGfxHandle (); // clear out the handle
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAIGfxResourceVK_OnGPUUnbind () {

//	this->mGfxMgr->GetDrawingAPI ().BindTexture ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAITextureVK::MOAIGfxResourceVK_OnGPUUpdate () {

//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	gfx.TexParameteri ( ZGL_TEXTURE_WRAP_S, this->mWrap );
//	gfx.TexParameteri ( ZGL_TEXTURE_WRAP_T, this->mWrap );
//
//	gfx.TexParameteri ( ZGL_TEXTURE_MIN_FILTER, this->mMinFilter );
//	gfx.TexParameteri ( ZGL_TEXTURE_MAG_FILTER, this->mMagFilter );
	
	return true;
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
//	state.SetField ( -1, "VK_LINEAR",					( u32 )ZGL_SAMPLE_LINEAR );
//	state.SetField ( -1, "VK_LINEAR_MIPMAP_LINEAR",		( u32 )ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR );
//	state.SetField ( -1, "VK_LINEAR_MIPMAP_NEAREST",	( u32 )ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST );
//
//	state.SetField ( -1, "VK_NEAREST",					( u32 )ZGL_SAMPLE_NEAREST );
//	state.SetField ( -1, "VK_NEAREST_MIPMAP_LINEAR",	( u32 )ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR );
//	state.SetField ( -1, "VK_NEAREST_MIPMAP_NEAREST",	( u32 )ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST );
//
//	state.SetField ( -1, "VK_RGBA4",					( u32 )ZGL_PIXEL_FORMAT_RGBA4 );
//	state.SetField ( -1, "VK_RGB5_A1",					( u32 )ZGL_PIXEL_FORMAT_RGB5_A1 );
//	state.SetField ( -1, "VK_DEPTH_COMPONENT16",		( u32 )ZGL_PIXEL_FORMAT_DEPTH_COMPONENT16 );
//	//***state.SetField ( -1, "VK_DEPTH_COMPONENT24",	( u32 )VK_DEPTH_COMPONENT24 );
//	//***state.SetField ( -1, "VK_STENCIL_INDEX1",		( u32 )VK_STENCIL_INDEX1 );
//	//***state.SetField ( -1, "VK_STENCIL_INDEX4",		( u32 )VK_STENCIL_INDEX4 );
//	state.SetField ( -1, "VK_STENCIL_INDEX8",			( u32 )ZGL_PIXEL_FORMAT_STENCIL_INDEX8 );
//	//***state.SetField ( -1, "VK_STENCIL_INDEX16",		( u32 )VK_STENCIL_INDEX16 );
//
//	// TODO:
//	#ifdef MOAI_OS_ANDROID
//		state.SetField ( -1, "VK_RGB565",				( u32 )ZGL_PIXEL_FORMAT_RGB565 );
//	#else
//		state.SetField ( -1, "VK_RGBA8",				( u32 )ZGL_PIXEL_FORMAT_RGBA8 );
//	#endif
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

//	luaL_Reg regTable [] = {
//		{ "getSize",				_getSize },
//		{ "release",				_release },
//		{ "setDebugName",			_setDebugName },
//		{ "setFilter",				_setFilter },
//		{ "setWrap",				_setWrap },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

////----------------------------------------------------------------//
//void MOAITextureVK::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
//
//	this->mGfxMgr->ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
//	MOAIGfxResourceVK::ZLGfxListener_OnGfxEvent ( event, userdata );
//}
