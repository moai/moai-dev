// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPvrHeader.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITexture.h>

#if MOAI_OS_NACL
#include "moai_nacl.h"

bool g_blockOnMainThreadTexLoad;
bool g_blockOnMainThreadTexUnload;

//----------------------------------------------------------------//
void NaClLoadTexture ( void* userData, int32_t result ) {

	MOAITexture *texture = ( MOAITexture * ) userData;

	texture->CreateTexture ();

	g_blockOnMainThreadTexLoad = false;
}

//----------------------------------------------------------------//
void NaClUnLoadTexture ( void* userData, int32_t result ) {

	MOAITexture *texture = ( MOAITexture * ) userData;

	texture->DeleteTexture ();

	g_blockOnMainThreadTexUnload = false;
}

#endif

//================================================================//
// MOAITextureLoader
//================================================================//
class MOAITextureLoader {
public:

	enum {
		TYPE_MOAI_IMAGE,
		TYPE_PVR,
		TYPE_UNKNOWN,
		TYPE_FAIL,
	};

	STLString		mFilename;
	void*			mFileData;
	size_t			mFileDataSize;
	MOAIImage		mImage;
	u32				mTransform;
	u32				mType;
	
	//----------------------------------------------------------------//
	void Load ( u32 transform = 0 ) {
	
		if ( this->mType != TYPE_UNKNOWN ) {
			return;
		}
		
		this->mTransform |= transform;
		
		if ( !this->mImage.IsOK ()) {
	
			if ( this->mFileData ) {
				this->mImage.Load ( this->mFileData, ( u32 )this->mFileDataSize, this->mTransform );
				free ( this->mFileData );
				this->mFileData = 0;
			}
			else if ( mFilename.size ()) {
				this->mImage.Load ( this->mFilename, this->mTransform );
			}
		}
		
		if ( this->mImage.IsOK ()) {
			this->mType = TYPE_MOAI_IMAGE;
		}
		#ifdef MOAI_TEST_PVR
			else {
				// get file data, check if PVR
				USFileStream stream;
				stream.OpenRead ( this->mFilename );
	
				if ( this->mFileData ) {
					free ( this->mFileData );
					this->mFileData = 0;
				}
				
				this->mFileDataSize = stream.GetLength ();
				this->mFileData = malloc ( this->mFileDataSize );
				stream.ReadBytes ( this->mFileData, this->mFileDataSize );
	
				stream.Close ();
				
				if ( MOAIPvrHeader::GetHeader( this->mFileData, this->mFileDataSize )) {				
					this->mType = TYPE_PVR;
				}
			}
		#endif
		
		if ( this->mType == TYPE_UNKNOWN ) {
			this->mType = TYPE_FAIL;
			this->Release ();
		}
	}
	
	//----------------------------------------------------------------//
	MOAITextureLoader () :
		mFileData ( 0 ),
		mFileDataSize ( 0 ),
		mType ( TYPE_UNKNOWN ) {
	}
	
	//----------------------------------------------------------------//
	~MOAITextureLoader () {
	
		if ( this->mFileData ) {
			free ( this->mFileData );
		}
	}
	
	//----------------------------------------------------------------//
	void Release () {
		if ( this->mFileData ) {
			free ( this->mFileData );
			this->mFileData = 0;
		}
		this->mImage.Release ();
	}
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	bind
	@text	Try to force the texture to perform its load.
	
	@in		MOAITexture self
	@out	nil
*/
int MOAITexture::_bind ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )
	
	self->Bind ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the width and height of the texture's source image.
			Avoid using the texture width and height to compute UV
			coordinates from pixels, as this will prevent texture
			resolution swapping.
	
	@in		MOAITexture self
	@out	number width
	@out	number height
*/
int MOAITexture::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )
	
	lua_pushnumber ( state, self->mWidth );
	lua_pushnumber ( state, self->mHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	initFrameBuffer
	@text	Initializes texture as a frame buffer.
	
	@in		MOAITexture self
	@in		number width
	@in		number height
	@out	nil
*/
int MOAITexture::_initFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "UNN" )
	
	u32 width				= state.GetValue < u32 >( 2, 0 );
	u32 height				= state.GetValue < u32 >( 3, 0 );
	
	// TODO: fix me
	#ifdef MOAI_OS_ANDROID
		GLenum colorFormat		= state.GetValue < GLenum >( 4, GL_RGB565 );
	#else
		GLenum colorFormat		= state.GetValue < GLenum >( 4, GL_RGBA8 );
	#endif
	
	GLenum depthFormat		= state.GetValue < GLenum >( 5, 0 );
	GLenum stencilFormat	= state.GetValue < GLenum >( 6, 0 );
	
	self->InitFrameBuffer ( width, height, colorFormat, depthFormat, stencilFormat );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads a texture from a data buffer or a file. Optionally pass
			in an image transform (affects .png images only).
	
	@in		MOAITexture self
	@in		variant texture		Either a MOAIDataBuffer containing a binary texture or a path to a texture file.
	@opt	number transform	Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
	@out	nil
*/
int MOAITexture::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );
	u32 transform = state.GetValue < u32 >( 3, DEFAULT_TRANSFORM );

	if ( data ) {
		self->Init ( *data, transform );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename = lua_tostring ( state, 2 );
		if ( MOAILogMessages::CheckFileExists ( filename, L )) {
			self->Init ( filename, transform );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases any memory associated with the texture.
	
	@in		MOAITexture self
	@out	nil
*/
int MOAITexture::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )
	
	self->MOAITexture::Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFilter
	@text	Set default filtering mode for texture.
	
	@in		MOAITexture self
	@in		number min			One of MOAITexture.GL_LINEAR, MOAITexture.GL_LINEAR_MIPMAP_LINEAR, MOAITexture.GL_LINEAR_MIPMAP_NEAREST,
								MOAITexture.GL_NEAREST, MOAITexture.GL_NEAREST_MIPMAP_LINEAR, MOAITexture.GL_NEAREST_MIPMAP_NEAREST
	@opt	number mag			Defaults to value passed to 'min'.
	@out	nil
*/
int MOAITexture::_setFilter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "UN" )

	int min = state.GetValue < int >( 2, GL_LINEAR );
	int mag = state.GetValue < int >( 3, min );

	self->SetFilter ( min, mag );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWrap
	@text	Set wrapping mode for texture.
	
	@in		MOAITexture self
	@in		boolean wrap		Texture will wrap if true, clamp if not.
	@out	nil
*/
int MOAITexture::_setWrap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "UB" )
	
	bool wrap = state.GetValue < bool >( 2, false );
	
	self->mWrap = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

	return 0;
}

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAITexture* MOAITexture::AffirmTexture ( MOAILuaState& state, int idx ) {

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( idx );
	if ( !texture ) {

		u32 transform = state.GetValue < u32 >( idx + 1, DEFAULT_TRANSFORM );

		if ( state.IsType ( idx, LUA_TUSERDATA )) {
			
			texture = state.GetLuaObject < MOAITexture >( idx );
			MOAIImage* image = state.GetLuaObject < MOAIImage >( idx );
			
			if ( image ) {
				texture = new MOAITexture ();
				texture->Init ( *image );
			}
			else {
			
				MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( idx );
				
				if ( data ) {
					texture = new MOAITexture ();
					texture->Init ( *data, transform );
				}
			}
		}
		else if ( state.IsType ( idx, LUA_TSTRING )) {
			
			cc8* filename = lua_tostring ( state, idx );
			texture = new MOAITexture ();
			texture->Init ( filename, transform );
		}
	}
	return texture;
}

//----------------------------------------------------------------//
void MOAITexture::CreateTexture	() {

	switch ( this->mLoader->mType ) {
			
		case MOAITextureLoader::TYPE_MOAI_IMAGE: {
			this->CreateTextureFromImage ( this->mLoader->mImage );
			break;
		}
		case MOAITextureLoader::TYPE_PVR: {
			this->CreateTextureFromPVR ( this->mLoader->mFileData, this->mLoader->mFileDataSize );
			break;
		}
		default:
			delete this->mLoader;
			this->mLoader = 0;
			this->SetError ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::CreateTextureFromImage ( MOAIImage& image ) {

	bool error = false;
	if ( !image.IsOK ()) return;
	if ( !MOAIGfxDevice::Get ().GetHasContext ()) return;

	MOAIGfxDevice::Get ().ClearErrors ();

	// get the dimensions before trying to get the OpenGL texture ID
	this->mWidth = image.GetWidth ();
	this->mHeight = image.GetHeight ();

	// warn if not a power of two
	if ( !image.IsPow2 ()) {
		MOAILog ( 0, MOAILogMessages::MOAITexture_NonPowerOfTwo_SDD, ( cc8* )this->mFilename, this->mWidth, this->mHeight );
	}

	glGenTextures ( 1, &this->mGLTexID );
	if ( !this->mGLTexID ) return;

	glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );

	USPixel::Format pixelFormat = image.GetPixelFormat ();
	USColor::Format colorFormat = image.GetColorFormat ();

	// generate mipmaps if set up to use them
	bool genMipMaps = (
		( this->mMinFilter == GL_LINEAR_MIPMAP_LINEAR ) ||
		( this->mMinFilter == GL_LINEAR_MIPMAP_NEAREST ) ||
		( this->mMinFilter == GL_NEAREST_MIPMAP_LINEAR ) ||
		( this->mMinFilter == GL_NEAREST_MIPMAP_NEAREST )
	);

	if ( pixelFormat == USPixel::TRUECOLOR ) {

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
			
			case USColor::A_8:
				this->mGLInternalFormat = GL_ALPHA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USColor::RGB_888:
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USColor::RGB_565:
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_SHORT_5_6_5;
				break;
			
			case USColor::RGBA_4444:
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_SHORT_4_4_4_4;
				break;
			
			case USColor::RGBA_8888:
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
				
			default: return;
		}

		glTexImage2D (
			GL_TEXTURE_2D,
			0,  
			this->mGLInternalFormat,
			this->mWidth,  
			this->mHeight,  
			0,
			this->mGLInternalFormat,
			this->mGLPixelType,  
			image.GetBitmap ()
		);
		
		this->mDataSize = image.GetBitmapSize ();
		
		if ( MOAIGfxDevice::Get ().LogErrors ()) {
			error = true;
		}
		else if ( genMipMaps ) {
		
			u32 mipLevel = 1;
			
			MOAIImage mipmap;
			mipmap.Copy ( image );
			
			while ( mipmap.MipReduce ()) {
				
				glTexImage2D (
					GL_TEXTURE_2D,
					mipLevel++,  
					this->mGLInternalFormat,
					mipmap.GetWidth (),  
					mipmap.GetHeight (),  
					0,
					this->mGLInternalFormat,
					this->mGLPixelType,  
					mipmap.GetBitmap ()
				);
				
				if ( MOAIGfxDevice::Get ().LogErrors ()) {
					error = true;
					break;
				}
				this->mDataSize += mipmap.GetBitmapSize ();
			}
		}
		
		if ( error ) {
			this->mDataSize = 0;
			glDeleteTextures ( 1, &this->mGLTexID );
			this->mGLTexID = 0;
			this->Clear ();
			return;
		}
	}
	else {
	
		// GL_PALETTE4_RGB8_OES
		// GL_PALETTE4_R5_G6_B5_OES
		// GL_PALETTE4_RGBA4_OES
		// GL_PALETTE4_RGB5_A1_OES
		// GL_PALETTE4_RGBA8_OES

		// GL_PALETTE8_RGB8_OES
		// GL_PALETTE8_R5_G6_B5_OES
		// GL_PALETTE8_RGBA4_OES
		// GL_PALETTE8_RGB5_A1_OES
		// GL_PALETTE8_RGBA8_OES

// TODO: check opengl version
//		this->mGLPixelType = 0;
//	
//		if ( pixelFormat == USPixel::INDEX_4 ) {
//		
//			switch ( colorFormat ) {
//			
//				case USColor::RGB_888:
//					this->mGLInternalFormat = GL_PALETTE4_RGB8_OES;
//					break;
//				
//				case USColor::RGBA_8888:
//					this->mGLInternalFormat = GL_PALETTE4_RGBA8_OES;
//					break;
//					
//				default: return;
//			}
//		}
//		else {
//			switch ( colorFormat ) {
//			
//				case USColor::RGB_888:
//					this->mGLInternalFormat = GL_PALETTE8_RGB8_OES;
//					break;
//				
//				case USColor::RGBA_8888:
//					this->mGLInternalFormat = GL_PALETTE8_RGBA8_OES;
//					break;
//					
//				default: return;
//			}
//		}
//		
//		glCompressedTexImage2D (
//			GL_TEXTURE_2D,
//			0,  
//			this->mGLInternalFormat,
//			this->mDevWidth,
//			this->mDevHeight,
//			0,  
//			image.GetDataSize (),
//			image.GetData ()
//		);
	}
}

//----------------------------------------------------------------//
void MOAITexture::CreateTextureFromPVR ( void* data, size_t size ) {
	UNUSED ( data );
	UNUSED ( size );

	#ifdef MOAI_TEST_PVR

		if ( !MOAIGfxDevice::Get ().GetHasContext ()) return;
		MOAIGfxDevice::Get ().ClearErrors ();

		MOAIPvrHeader* header = MOAIPvrHeader::GetHeader ( data, size );
		if ( !header ) return;
		
		bool compressed = false;
		bool hasAlpha = header->mAlphaBitMask ? true : false;
		
		switch ( header->mPFFlags & MOAIPvrHeader::PF_MASK ) {
			
			case MOAIPvrHeader::OGL_RGBA_4444:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_SHORT_4_4_4_4;
				break;
		
			case MOAIPvrHeader::OGL_RGBA_5551:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			
			case MOAIPvrHeader::OGL_RGBA_8888:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_RGB_565:
				compressed = false;
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_SHORT_5_6_5;
				break;
			
			// NO IMAGE FOR THIS
//			case MOAIPvrHeader::OGL_RGB_555:
//				break;
			
			case MOAIPvrHeader::OGL_RGB_888:
				compressed = false;
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_I_8:
				compressed = false;
				this->mGLInternalFormat = GL_LUMINANCE;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_AI_88:
				compressed = false;
				this->mGLInternalFormat = GL_LUMINANCE_ALPHA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_PVRTC2:
				compressed = true;
				this->mGLInternalFormat = hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;
			
			case MOAIPvrHeader::OGL_PVRTC4:
				compressed = true;
				this->mGLInternalFormat = hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;
			
			case MOAIPvrHeader::OGL_BGRA_8888:
				compressed = false;
				this->mGLInternalFormat = GL_BGRA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case MOAIPvrHeader::OGL_A_8:
				compressed = false;
				this->mGLInternalFormat = GL_ALPHA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
		}
		
		
		glGenTextures ( 1, &this->mGLTexID );
		if ( !this->mGLTexID ) return;

		glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
		
		this->mDataSize = 0;
		
		int width = header->mWidth;
		int height = header->mHeight;
		char* imageData = (char*)(header->GetFileData ( data, size));
		if ( header->mMipMapCount == 0 ) {
			
			GLsizei currentSize = (GLsizei) USFloat::Max ( (float)(32), (float)(width * height * header->mBitCount / 8) );
			this->mDataSize += currentSize;
			
			if ( compressed ) {
				glCompressedTexImage2D ( GL_TEXTURE_2D, 0, this->mGLInternalFormat, width, height, 0, currentSize, imageData );
			}
			else {
				glTexImage2D( GL_TEXTURE_2D, 0, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData );	
			}
			
			if ( glGetError () != 0 ) {
				this->Clear ();
				return;
			}
		}
		else {
			for ( int level = 0; width > 0 && height > 0; ++level ) {
				GLsizei currentSize = (GLsizei) USFloat::Max ( (float)(32), (float)(width * height * header->mBitCount / 8) );
			
				if ( compressed ) {
					glCompressedTexImage2D ( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, currentSize, imageData );
				}
				else {
					glTexImage2D( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData );
				}
				
				if ( glGetError () != 0 ) {
					this->Clear ();
					return;
				}
				
				imageData += currentSize;
				this->mDataSize += currentSize;
				
				width >>= 1;
				height >>= 1;
			}	
		}			

	#endif
}

//----------------------------------------------------------------//
void MOAITexture::DeleteTexture	() {
	glDeleteTextures ( 1, &this->mGLTexID );
}

//----------------------------------------------------------------//
u32 MOAITexture::GetHeight () {
	return this->mHeight;
}

//----------------------------------------------------------------//
u32 MOAITexture::GetWidth () {
	return this->mWidth;
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, cc8* debugname ) {

	this->Clear ();
	
	this->mLoader = new MOAITextureLoader ();
	this->mLoader->mFilename = debugname ? debugname : "";
	this->mLoader->mTransform = 0;
	this->mLoader->mImage.Copy ( image );
	
	this->mTransform = 0;
	
	this->OnLoad ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( cc8* filename, u32 transform ) {

	this->Clear ();
	if ( !USFileSys::CheckFileExists ( filename )) return;

	//this->mFilename = USFileSys::Expand ( filename );
	this->mFilename = filename;
	
	this->mLoader = new MOAITextureLoader ();
	this->mLoader->mFilename = this->mFilename;
	this->mLoader->mTransform = transform;
	
	this->mIsRenewable = true;
	
	this->mTransform = transform;
	
	this->OnLoad ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );

	this->Init ( bytes, size, transform, debugname );
	
	data.Unlock ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( const void* data, u32 size, u32 transform, cc8* debugname ) {

	this->Clear ();
	
	this->mLoader = new MOAITextureLoader ();
	this->mLoader->mFilename = debugname ? debugname : "";
	this->mLoader->mTransform = transform;
	this->mLoader->mFileDataSize = size;
	this->mLoader->mFileData = malloc ( size );
	memcpy ( this->mLoader->mFileData, data, size );
	
	this->mTransform = transform;
	
	this->OnLoad ();
}

//----------------------------------------------------------------//
void MOAITexture::InitFrameBuffer ( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat ) {

	if ( MOAIGfxDevice::Get ().IsFramebufferSupported ()) {

		if ( !this->mFrameBuffer ) {
			this->mFrameBuffer = new MOAIFrameBuffer ();
		}

		this->mWidth = width;
		this->mHeight = height;

		this->mFrameBuffer->Init ( width, height, colorFormat, depthFormat, stencilFormat );
		this->mIsRenewable = true;
		
		this->OnLoad ();
	}
	else {
		MOAILog ( 0, MOAILogMessages::MOAITexture_NoFramebuffer );
	}
}

//----------------------------------------------------------------//
bool MOAITexture::IsFrameBuffer () {

	return ( this->mFrameBuffer != 0 );
}

//----------------------------------------------------------------//
bool MOAITexture::IsRenewable () {

	return this->mIsRenewable;
}

//----------------------------------------------------------------//
bool MOAITexture::IsValid () {

	if ( this->mFrameBuffer && ( !this->mFrameBuffer->IsValid ())) {
		return false;
	}
	return ( this->mGLTexID != 0 );
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () :
	mGLTexID ( 0 ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( GL_LINEAR ),
	mMagFilter ( GL_NEAREST ),
	mWrap ( GL_CLAMP_TO_EDGE ),
	mLoader ( 0 ),
	mFrameBuffer ( 0 ),
	mDataSize ( 0 ),
	mIsRenewable ( false ),
	mTransform ( DEFAULT_TRANSFORM ),
	mIsDirty ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITexture::OnBind () {

	if ( !this->mGLTexID ) return;

	glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
	
	if ( this->mIsDirty ) {
	
#if USE_OPENGLES1	
		if ( !MOAIGfxDevice::Get ().IsProgrammable ()) {
			glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		}
#endif
		
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrap );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrap );
		
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter );
		
		this->mIsDirty = false;
	}
}

//----------------------------------------------------------------//
void MOAITexture::OnClear () {

	this->OnUnload ();
	
	this->mWidth = 0;
	this->mHeight = 0;
	
	if ( this->mLoader ) {
		delete this->mLoader;
		this->mLoader = 0;
	}
	
	if ( this->mFrameBuffer ) {
		delete this->mFrameBuffer;
		this->mFrameBuffer = 0;
	}
	
	this->mFilename.clear ();
	this->mTransform = DEFAULT_TRANSFORM;
	
	this->mIsRenewable = false;
}

//----------------------------------------------------------------//
void MOAITexture::OnLoad () {

	if ( !MOAIGfxDevice::Get ().GetHasContext ()) {
		MOAILog ( 0, MOAILogMessages::MOAITexture_MissingDevice_S, this->mFilename.str ());
		return;
	}

	if ( this->mFrameBuffer ) {
		
		this->mFrameBuffer->Bind ();
		if ( this->mFrameBuffer->IsValid ()) {
		
			glGenTextures ( 1, &this->mGLTexID );
			glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
			glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mGLTexID, 0 );
			
			// refresh tex params on next bind
			this->mIsDirty = true;
		}
	}
	else if ( this->mLoader ) {

		//if ( !GLEW_OES_compressed_paletted_texture ) {
		//	transform.ConvertToTrueColor ();
		//}
		this->mLoader->Load ();

#ifdef MOAI_OS_NACL
		if ( g_core->IsMainThread () ) {
			printf ( "ERROR: Texture Cannot perform blocking file I/O on main thread\n" );
		}

		g_blockOnMainThreadTexLoad = true;
		pp::CompletionCallback cc ( NaClLoadTexture, this );
		g_core->CallOnMainThread ( 0, cc , 0 );

		while ( g_blockOnMainThreadTexLoad ) {
			usleep ( 1000 );
		}
#else
		this->CreateTexture ();
#endif

		if ( this->mGLTexID ) {

			if ( this->mFilename.size() == 0 && this->mLoader ) {
				// Loader's filename doesn't seem to be copied in all code paths...
				MOAIGfxDevice::Get ().ReportTextureAlloc ( this->mLoader->mFilename, this->mDataSize );
			}
			else {
				MOAIGfxDevice::Get ().ReportTextureAlloc ( this->mFilename, this->mDataSize );
			}
			
			// done with the loader entirely
			delete this->mLoader;
			this->mLoader = 0;
			
			// refresh tex params on next bind
			this->mIsDirty = true;
		}
	}
}

//----------------------------------------------------------------//
void MOAITexture::OnRenew () {

	if ( !this->mFrameBuffer ) {
		STLString filename = this->mFilename;
		this->Init ( filename, this->mTransform );
	}
}

//----------------------------------------------------------------//
void MOAITexture::OnUnload () {

	if ( this->mGLTexID ) {
	
		if ( MOAIGfxDevice::IsValid ()) {
			MOAIGfxDevice::Get ().ReportTextureFree ( this->mFilename, this->mDataSize );
		}
#ifdef MOAI_OS_NACL
		g_blockOnMainThreadTexUnload = true;

		if ( g_core->IsMainThread () ) {
			this->DeleteTexture ();
			g_blockOnMainThreadTexUnload = false;
		}
		else {
			pp::CompletionCallback cc ( NaClUnLoadTexture, this );
			g_core->CallOnMainThread ( 0, cc , 0 );
		}

		while ( g_blockOnMainThreadTexUnload ) {
			usleep ( 1000 );
		}
#else
		this->DeleteTexture ();
#endif
		this->mGLTexID = 0;
	}
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIGfxResource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "GL_LINEAR", ( u32 )GL_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_LINEAR", ( u32 )GL_LINEAR_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_NEAREST", ( u32 )GL_LINEAR_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_NEAREST", ( u32 )GL_NEAREST );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_LINEAR", ( u32 )GL_NEAREST_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_NEAREST", ( u32 )GL_NEAREST_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_RGBA4",				( u32 )GL_RGBA4 );
	state.SetField ( -1, "GL_RGB5_A1",				( u32 )GL_RGB5_A1 );
	state.SetField ( -1, "GL_DEPTH_COMPONENT16",	( u32 )GL_DEPTH_COMPONENT16 );
	//***state.SetField ( -1, "GL_DEPTH_COMPONENT24",	( u32 )GL_DEPTH_COMPONENT24 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX1",		( u32 )GL_STENCIL_INDEX1 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX4",		( u32 )GL_STENCIL_INDEX4 );
	state.SetField ( -1, "GL_STENCIL_INDEX8",		( u32 )GL_STENCIL_INDEX8 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX16",		( u32 )GL_STENCIL_INDEX16 );
	
	// TODO:
	#ifdef MOAI_OS_ANDROID
		state.SetField ( -1, "GL_RGB565",				( u32 )GL_RGB565 );
	#else
		state.SetField ( -1, "GL_RGBA8",				( u32 )GL_RGBA8 );
	#endif
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "bind",					_bind },
		{ "getSize",				_getSize },
		{ "initFrameBuffer",		_initFrameBuffer },
		{ "load",					_load },
		{ "release",				_release },
		{ "setFilter",				_setFilter },
		{ "setWrap",				_setWrap },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	STLString path = state.GetField ( -1, "mPath", "" );
	
	if ( path.size ()) {
		this->Init ( path, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	STLString path = USFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( int filter ) {

	this->SetFilter ( filter, filter );
}

//----------------------------------------------------------------//
void MOAITexture::SetFilter ( int min, int mag ) {

	this->mMinFilter = min;
	this->mMagFilter = mag;
	
	this->mIsDirty = true;
}

//----------------------------------------------------------------//
void MOAITexture::SetWrap ( int wrap ) {

	this->mWrap = wrap;
	this->mIsDirty = true;
}
