// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxConstsGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Returns the width and height of the texture's source image.
			Avoid using the texture width and height to compute UV
			coordinates from pixels, as this will prevent texture
			resolution swapping.
 
	@in		MOAITextureGL self
	@out	number width
	@out	number height
*/
int MOAITextureGL::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )
		
	lua_pushnumber ( state, self->mWidth );
	lua_pushnumber ( state, self->mHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	release
	@text	Releases any memory associated with the texture.
 
	@in		MOAITextureGL self
	@out	nil
*/
int MOAITextureGL::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )
	
	self->Finalize ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDebugName
	@text	Set a name for the texture to use during memory logging.
 
	@in		MOAITextureGL self
	@in		string debugName
	@out	nil
*/
int MOAITextureGL::_setDebugName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )

	self->mDebugName = state.GetValue < cc8* >( 2, "" );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setFilter
	@text	Set default filtering mode for texture.
 
	@in		MOAITextureGL self
	@in		number min			One of MOAITextureGL.GL_LINEAR, MOAITextureGL.GL_LINEAR_MIPMAP_LINEAR, MOAITextureGL.GL_LINEAR_MIPMAP_NEAREST,
								MOAITextureGL.GL_NEAREST, MOAITextureGL.GL_NEAREST_MIPMAP_LINEAR, MOAITextureGL.GL_NEAREST_MIPMAP_NEAREST
	@opt	number mag			Defaults to value passed to 'min'.
	@out	nil
*/
int MOAITextureGL::_setFilter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "UN" )

	MOAITextureFilterEnum::_ min = ( MOAITextureFilterEnum::_ )state.GetValue < u32 >( 2, ZLGfxEnum::SAMPLE_LINEAR );
	MOAITextureFilterEnum::_ mag = ( MOAITextureFilterEnum::_ )state.GetValue < u32 >( 3, min );

	MOAITextureGL::CheckFilterModes ( min, mag );

	self->SetFilter ( min, mag );

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextureGL::_setInternalFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )
	
	self->mGLInternalFormat = state.GetEnum ( 2, ZLGfxEnum::PIXEL_FORMAT_RGBA );
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextureGL::_setPixelType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )
	
	self->mGLPixelType = state.GetEnum ( 2, ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE );
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setWrap
	@text	Set wrapping mode for texture.
 
	@in		MOAITextureGL self
	@in		boolean wrap		Texture will wrap if true, clamp if not.
	@out	nil
*/
int MOAITextureGL::_setWrap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "UB" )
	
	bool wrap = state.GetValue < bool >( 2, false );
	
	self->mWrap = wrap ? MOAITextureWrapEnum::REPEAT : MOAITextureWrapEnum::CLAMP;

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAITextureGL
//================================================================//

//----------------------------------------------------------------//
void MOAITextureGL::CheckFilterModes ( int min, int mag ) {
	UNUSED ( min );
	UNUSED ( mag );

	#ifdef _DEBUG
		switch ( min ) {
			case ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_LINEAR:
			case ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_NEAREST:
			case ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_LINEAR:
			case ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_NEAREST:
			
				if ( !(( mag == ZLGfxEnum::SAMPLE_LINEAR ) || ( mag == ZLGfxEnum::SAMPLE_NEAREST ))) {

					ZLLog_Warning ( "WARNING: possibly incompatible filter modes; MIPMAP not supported for mag filter\n" );
				}
				break;
				
			default:
				break;
		}
	#endif
}

//----------------------------------------------------------------//
void MOAITextureGL::CleanupOnError () {

	this->mGfxMgr->GetDrawingAPI ().DeleteResource ( this->mGLTexture );

	this->mTextureSize = 0;
	this->mWidth = 0;
	this->mHeight = 0;
	this->mTextureSize = 0;
}

//----------------------------------------------------------------//
bool MOAITextureGL::CreateTexture () {

	if ( !( this->mWidth && this->mHeight )) return false;

	u32 pixelSize = 0;
	
	switch ( this->mGLPixelType ) {
		
		case ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_6_5:
		case ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1:
		case ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4:
			pixelSize = 2;
			break;
		
		case ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE:
			pixelSize = 4;
			break;
	}

	if ( !pixelSize ) return false;

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	this->mGLTexture = gfx.CreateTexture ();
	
	gfx.BindTexture ( this->mGLTexture );
	gfx.TexImage2D (
		0,
		this->mGLInternalFormat,
		this->mWidth,
		this->mHeight,
		this->mGLInternalFormat,
		this->mGLPixelType,
		NULL
	);

	this->mTextureSize = this->mWidth * this->mHeight * pixelSize;

	return true;
}

//----------------------------------------------------------------//
bool MOAITextureGL::CreateTextureFromImage ( ZLImage& srcImage ) {

	// TODO: ZLGfx
//	if ( !MOAIGfxMgr::Get ().GetHasContext ()) return false;

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLImage altImage;

	ZLColor::ColorFormat colorFormat = srcImage.GetColorFormat ();
	if ( colorFormat == ZLColor::CLR_FMT_UNKNOWN ) return false;

	if (( colorFormat == ZLColor::A_1 ) || ( colorFormat == ZLColor::A_4 )) {
		colorFormat = ZLColor::A_8;
	}
	
	if (( colorFormat != srcImage.GetColorFormat ()) || ( srcImage.GetPixelFormat () != ZLImage::TRUECOLOR )) {
		//if ( !altImage.Convert ( srcImage, colorFormat, ZLImage::TRUECOLOR )); // TODO: what was this? add error handling?
		altImage.Convert ( srcImage, colorFormat, ZLImage::TRUECOLOR );
	}
	
	ZLImage& image = altImage.IsOK () ? altImage : srcImage;
	if ( !image.IsOK ()) return false;

	//MOAIGfxMgr::Get ().ClearErrors ();
	this->mGLTexture = gfx.CreateTexture ();
	//if ( !this->mGLTexture ) return false;

	// get the dimensions before trying to get the OpenGL texture ID
	this->mWidth = image.GetWidth ();
	this->mHeight = image.GetHeight ();

	// warn if not a power of two (if we're supposed to generate mipmaps)
//	if ( this->ShouldGenerateMipmaps () && !image.IsPow2 ()) {
//		MOAILogF ( 0, ZLLog::LOG_WARNING, MOAISTRING_MOAITexture_NonPowerOfTwo_SDD, ( cc8* )this->mDebugName, this->mWidth, this->mHeight );
//	}

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
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_ALPHA;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::LA_8:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_LUMINANCE_ALPHA;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::RGB_888:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_RGB;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
			break;
		
		case ZLColor::RGB_565:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_RGB;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_6_5;
			break;
		
		case ZLColor::RGBA_5551:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1;
			break;
		
		case ZLColor::RGBA_4444:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4;
			break;
		
		case ZLColor::RGBA_8888:
			this->mGLInternalFormat = ZLGfxEnum::PIXEL_FORMAT_RGBA;
			this->mGLPixelType = ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE;
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
//		this->CleanupOnError ( gfx );
//		return false;
//	}
//	else if ( this->ShouldGenerateMipmaps ()) {
	
	if ( this->ShouldGenerateMipmaps ()) {
	
		u32 mipLevel = 1;
		
		ZLImage mipmap;
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
			
			if ( this->mGfxMgr->LogErrors ()) {
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
MOAITextureGL::MOAITextureGL () :
	mGLInternalFormat ( ZLGfxEnum::PIXEL_FORMAT_RGBA ),
	mGLPixelType ( ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE ) {
	
	RTTI_BEGIN ( MOAITextureGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAITextureGL >)
		RTTI_EXTEND ( MOAITexture )
		RTTI_EXTEND ( MOAIFrameBufferAttachableGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureGL::~MOAITextureGL () {

	this->ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
void MOAITextureGL::SetGLTexture ( const ZLGfxHandle& glTexture, ZLGfxEnum::_ internalFormat, ZLGfxEnum::_ pixelType, size_t textureSize ) {

	this->mGLTexture 			= glTexture;
	this->mGLInternalFormat		= internalFormat;
	this->mGLPixelType			= pixelType;
	this->mTextureSize			= textureSize;

	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
bool MOAITextureGL::ShouldGenerateMipmaps () {

	return (
		( this->mMinFilter == MOAITextureFilterEnum::LINEAR_MIPMAP_LINEAR ) ||
		( this->mMinFilter == MOAITextureFilterEnum::LINEAR_MIPMAP_NEAREST ) ||
		( this->mMinFilter == MOAITextureFilterEnum::NEAREST_MIPMAP_LINEAR ) ||
		( this->mMinFilter == MOAITextureFilterEnum::NEAREST_MIPMAP_NEAREST )
	);
}

//----------------------------------------------------------------//
bool MOAITextureGL::UpdateTextureFromImage ( ZLImage& image, ZLIntRect rect ) {

	// TODO: what happens when image is an unsupported format?

	// if we need to generate mipmaps or the dimensions have changed, clear out the old texture
	if ( this->ShouldGenerateMipmaps () || ( this->mWidth != image.GetWidth ()) || ( this->mHeight != image.GetHeight ())) {
	
		this->mGfxMgr->ReportTextureFree ( this->mDebugName, this->mTextureSize );
		this->mGfxMgr->DeleteOrDiscard ( this->mGLTexture, false );
		
		if ( this->CreateTextureFromImage ( image )) {
			this->mGfxMgr->ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
			return true;
		}
		return false;
	}
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
	// if the texture exists just update the sub-region
	// otherwise create a new texture from the image
	if ( this->mGLTexture.CanBind ()) {

		gfx.BindTexture ( this->mGLTexture );

		rect.Bless ();
		ZLIntRect imageRect = image.GetRect ();
		imageRect.Clip ( rect );
		
		ZLSharedConstBuffer* bitmapBuffer = image.GetBitmapBuffer ();
		
		ZLImage subImage;
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
		
		this->mGfxMgr->LogErrors ();
		
		return true;
	}
	
	return false;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITextureGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	state.SetEnum ( -1, "FILTER_LINEAR",						MOAITextureFilterEnum::LINEAR );
	state.SetEnum ( -1, "FILTER_LINEAR_MIPMAP_LINEAR",			MOAITextureFilterEnum::LINEAR_MIPMAP_LINEAR );
	state.SetEnum ( -1, "FILTER_LINEAR_MIPMAP_NEAREST",			MOAITextureFilterEnum::LINEAR_MIPMAP_NEAREST );
	
	state.SetEnum ( -1, "FILTER_NEAREST",						MOAITextureFilterEnum::NEAREST );
	state.SetEnum ( -1, "FILTER_NEAREST_MIPMAP_LINEAR",			MOAITextureFilterEnum::NEAREST_MIPMAP_LINEAR );
	state.SetEnum ( -1, "FILTER_NEAREST_MIPMAP_NEAREST",		MOAITextureFilterEnum::NEAREST_MIPMAP_NEAREST );
	
	state.SetEnum ( -1, "PIXEL_FORMAT_RGBA4",					ZLGfxEnum::PIXEL_FORMAT_RGBA4 );
	state.SetEnum ( -1, "PIXEL_FORMAT_RGB5_A1",					ZLGfxEnum::PIXEL_FORMAT_RGB5_A1 );
	state.SetEnum ( -1, "PIXEL_FORMAT_DEPTH_COMPONENT16",		ZLGfxEnum::PIXEL_FORMAT_DEPTH_COMPONENT16 );
	//state.SetEnum ( -1, "PIXEL_FORMAT_DEPTH_COMPONENT24",		GL_DEPTH_COMPONENT24 );
	//state.SetEnum ( -1, "PIXEL_FORMAT_STENCIL_INDEX1",		GL_STENCIL_INDEX1 );
	//state.SetEnum ( -1, "PIXEL_FORMAT_STENCIL_INDEX4",		GL_STENCIL_INDEX4 );
	state.SetEnum ( -1, "PIXEL_FORMAT_STENCIL_INDEX8",			ZLGfxEnum::PIXEL_FORMAT_STENCIL_INDEX8 );
	//state.SetEnum ( -1, "PIXEL_FORMAT_STENCIL_INDEX16",		GL_STENCIL_INDEX16 );
	
	state.SetEnum ( -1, "PIXEL_TYPE_BYTE",										ZLGfxEnum::PIXEL_TYPE_BYTE );
	state.SetEnum ( -1, "PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG",			ZLGfxEnum::PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG );
	state.SetEnum ( -1, "PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG",			ZLGfxEnum::PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG );
	state.SetEnum ( -1, "PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG",			ZLGfxEnum::PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG );
	state.SetEnum ( -1, "PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG",			ZLGfxEnum::PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG );
	state.SetEnum ( -1, "PIXEL_TYPE_FLOAT",										ZLGfxEnum::PIXEL_TYPE_FLOAT );
	state.SetEnum ( -1, "PIXEL_TYPE_INT",										ZLGfxEnum::PIXEL_TYPE_INT );
	state.SetEnum ( -1, "PIXEL_TYPE_SHORT",										ZLGfxEnum::PIXEL_TYPE_SHORT );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_BYTE",								ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_BYTE_3_3_2",						ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE_3_3_2 );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_INT",								ZLGfxEnum::PIXEL_TYPE_UNSIGNED_INT );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_INT_8_8_8_8",						ZLGfxEnum::PIXEL_TYPE_UNSIGNED_INT_8_8_8_8 );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV",				ZLGfxEnum::PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_INT_10_10_10_2",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_INT_10_10_10_2 );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT",							ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_5_6_5",						ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_6_5 );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4 );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV",				ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV",				ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV );
	state.SetEnum ( -1, "PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1",					ZLGfxEnum::PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1 );
	
	// TODO:
	#ifdef MOAI_OS_ANDROID
		state.SetEnum ( -1, "PIXEL_FORMAT_RGB565",				ZLGfxEnum::PIXEL_FORMAT_RGB565 );
	#else
		state.SetEnum ( -1, "PIXEL_FORMAT_RGBA8",				ZLGfxEnum::PIXEL_FORMAT_RGBA8 );
	#endif
}

//----------------------------------------------------------------//
void MOAITextureGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getSize",				_getSize },
		{ "release",				_release },
		{ "setDebugName",			_setDebugName },
		{ "setFilter",				_setFilter },
		{ "setInternalFormat",		_setInternalFormat },
		{ "setPixelType",			_setPixelType },
		{ "setWrap",				_setWrap },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAITextureGL::MOAIFrameBufferAttachableGL_Attach ( ZLGfx& gfx, ZLGfxEnum::_ attachment, ZLGfxEnum::_ target, s32 level, s32 layer ) {

	this->Affirm ();

	if ( !this->mGLTexture ) return false;
	
	gfx.FramebufferTexture2D ( target, attachment, this->mGLTexture, 0 );
	return true;
}

//----------------------------------------------------------------//
void MOAITextureGL::MOAIGfxResourceGL_OnGPUBind () {

	this->mGfxMgr->GetDrawingAPI ().BindTexture ( this->mGLTexture );
}

//----------------------------------------------------------------//
void MOAITextureGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	if ( this->mGLTexture.CanBind ()) {
		this->mGfxMgr->ReportTextureFree ( this->mDebugName, this->mTextureSize );
	}
	this->mGfxMgr->DeleteOrDiscard ( this->mGLTexture, shouldDelete );
	this->mGLTexture = ZLGfxHandle (); // clear out the handle
}

//----------------------------------------------------------------//
void MOAITextureGL::MOAIGfxResourceGL_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().BindTexture ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAITextureGL::MOAIGfxResourceGL_OnGPUUpdate () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	gfx.TexParameteri ( ZLGfxEnum::TEXTURE_WRAP_S, MOAIGfxConstsGL::Remap ( this->mWrap ));
	gfx.TexParameteri ( ZLGfxEnum::TEXTURE_WRAP_T, MOAIGfxConstsGL::Remap ( this->mWrap ));
	
	gfx.TexParameteri ( ZLGfxEnum::TEXTURE_MIN_FILTER, MOAIGfxConstsGL::Remap ( this->mMinFilter ));
	gfx.TexParameteri ( ZLGfxEnum::TEXTURE_MAG_FILTER, MOAIGfxConstsGL::Remap ( this->mMagFilter ));
	
	return true;
}

//----------------------------------------------------------------//
void MOAITextureGL::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {

	this->mGfxMgr->ReportTextureAlloc ( this->mDebugName, this->mTextureSize );
	MOAIGfxResourceGL::ZLGfxListener_OnGfxEvent ( event, userdata );
}
