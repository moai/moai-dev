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

#define DEBUG_TEXTURE_MEMORY 1

static size_t _g_TextureSize = 0;

#ifdef DEBUG_TEXTURE_MEMORY

	#define DEBUG_TEXTURE_ALLOC(name, size) do { \
		_g_TextureSize += size; \
		printf("TEXTURE: + %10lu = %6.2fMB < %s\n", size, _g_TextureSize / 1024.0 / 1024.0, (name).c_str()); \
	} while(0);

	#define DEBUG_TEXTURE_FREE(name, size) do { \
		_g_TextureSize -= size; \
		printf("TEXTURE: - %10lu = %6.2fMB < %s\n", size, _g_TextureSize / 1024.0 / 1024.0, (name).c_str()); \
	} while(0);

#else
	#define	DEBUG_TEXTURE_ALLOC(name, size)
	#define DEBUG_TEXTURE_FREE(name, size)
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
	bool			mReloadable;
	
	//----------------------------------------------------------------//
	void Load ( u32 transform = 0 ) {
	
		if (( this->mType != TYPE_UNKNOWN ) && ( !this->mReloadable )) {
			return;
		}
		
		this->mReloadable = false;
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
		else {
			this->mReloadable = true;
		}
	}
	
	//----------------------------------------------------------------//
	MOAITextureLoader () :
		mFileData ( 0 ),
		mFileDataSize ( 0 ),
		mType ( TYPE_UNKNOWN ),
		mReloadable ( false ) {
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
	@text	Initializes shader as a frame buffer.
	
	@in		MOAITexture self
	@in		number width
	@in		number height
	@out	nil
*/
int MOAITexture::_initFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "UNN" )
	
	u32 width				= state.GetValue < u32 >( 2, 0 );
	u32 height				= state.GetValue < u32 >( 3, 0 );
	GLenum colorFormat		= state.GetValue < GLenum >( 4, 0 );
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
		MOAI_CHECK_FILE ( filename );
		self->Init ( filename, transform );
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
/**	@name	setFilter
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

//----------------------------------------------------------------//
/**	@name	softRelease
	@text	Attempt to release the resources for this texture and free it up
			in such a way that it can be re-loaded automatically on demand.
			Generally this is used when responding to a memory warning from
			the system and allow older/unused textures to be released in an
			attempt to keep the application from crashing. Currently textures
			loaded from files (not buffers) can be reloaded in this fashion.
			Be warned, however, loading texures is expensive and your frame
			rate is likely going to suffer.
 
 @in		MOAITexture self
 @in		int age	(Optional) Release only if the texture hasn't been used in X frames.
 @out		bool True if the texture was actually released.
 */
int MOAITexture::_softRelease ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )
	
	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean(L, self->SoftRelease(age) );

	return 1;
}

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
void MOAITexture::Affirm () {

	if ( this->mGLTexID ) return;
	
	if ( this->mFrameBuffer ) {
		
		this->mFrameBuffer->Bind ();
		if ( this->mFrameBuffer->IsOK ()) {
		
			glGenTextures ( 1, &this->mGLTexID );
			glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
			glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
			glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mGLTexID, 0 );
		}
	}
	else if ( this->mLoader ) {

		//if ( !GLEW_OES_compressed_paletted_texture ) {
		//	transform.ConvertToTrueColor ();
		//}
		this->mLoader->Load ();
		
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
		}

		if ( this->mGLTexID ) {

			if ( this->mFilename.size() == 0 && this->mLoader ) {
				// Loader's filename doesn't seem to be copied in all code paths...
				DEBUG_TEXTURE_ALLOC ( this->mLoader->mFilename, this->mDataSize )
			}
			else {
				DEBUG_TEXTURE_ALLOC ( this->mFilename, this->mDataSize )
			}
			
			this->mLastFrameUsed = MOAISim::Get ().GetFrameCounter ();
			
			if( this->mLoader->mReloadable ) {
				// Release the loader's copy. It isn't needed any more.
				this->mLoader->Release();
			}
			else {
				// done with the loader entirely
				delete this->mLoader;
				this->mLoader = 0;
			}
		}
	}
}

//----------------------------------------------------------------//
MOAITexture* MOAITexture::AffirmTexture ( USLuaState& state, int idx ) {

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
bool MOAITexture::Bind () {

	this->Affirm ();
	if ( !this->mGLTexID ) {
		return false;
	}

	// attempt to recover from lost context
	if ( !glIsTexture ( this->mGLTexID )) {
	
		this->mGLTexID = 0;
	
		// ugh... fix this monstrosity later!
		if ( this->mFilename.size ()) {
			this->Init ( this->mFilename );
			this->Affirm ();
		}
		if ( !this->mGLTexID ) return false;
	}

	this->mLastFrameUsed = MOAISim::Get ().GetFrameCounter ();

	glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
	glEnable ( GL_TEXTURE_2D );
	
	if ( !MOAIGfxDevice::Get ().IsProgrammable ()) {
		glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	}
	
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrap );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrap );
	
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter );
	
	return true;
}

//----------------------------------------------------------------//
bool MOAITexture::BindFrameBuffer () {

	this->Affirm ();
	return this->mFrameBuffer->Bind ();
}

//----------------------------------------------------------------//
void MOAITexture::Clear () {

	if ( this->mGLTexID ) {
		DEBUG_TEXTURE_FREE ( this->mFilename, this->mDataSize )
		glDeleteTextures ( 1, &this->mGLTexID );
		this->mGLTexID = 0;
	}
	
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
}

//----------------------------------------------------------------//
void MOAITexture::CreateTextureFromImage ( MOAIImage& image ) {

	if ( !image.IsOK ()) return;

	printf ( "MOAITexture::CreateTextureFromImage - %s\n", this->mFilename.str ());

	MOAIGfxDevice::Get ().ClearErrors ();

	// get the dimensions before trying to get the OpenGL texture ID
	this->mWidth = image.GetWidth ();
	this->mHeight = image.GetHeight ();

	glGenTextures ( 1, &this->mGLTexID );
	if ( !this->mGLTexID ) return;

	glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );

	USPixel::Format pixelFormat = image.GetPixelFormat ();
	USColor::Format colorFormat = image.GetColorFormat ();

	// generate mipmaps if set up to use them
	if (	( this->mMinFilter == GL_LINEAR_MIPMAP_LINEAR ) ||
			( this->mMinFilter == GL_LINEAR_MIPMAP_NEAREST ) ||
			( this->mMinFilter == GL_NEAREST_MIPMAP_LINEAR ) ||
			( this->mMinFilter == GL_NEAREST_MIPMAP_NEAREST )) {
		
		glTexParameteri ( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	}

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
		
		if ( glGetError () != 0 ) {
			// we have an error
			this->Clear ();
			return;
		}
		
		this->mDataSize = image.GetBitmapSize ();
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
				if ( glGetError () != 0 ) {
					// we have an error
					this->mGLTexID = 0;
					return;
				}
			}
			else {
				glTexImage2D( GL_TEXTURE_2D, 0, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData);
				if ( glGetError () != 0 ) {
					// we have an error
					this->Clear ();
					return;
				}
			}
		}
		else {
			for ( int level = 0; width > 0 && height > 0; ++level ) {
				GLsizei currentSize = (GLsizei) USFloat::Max ( (float)(32), (float)(width * height * header->mBitCount / 8) );
			
				if ( compressed ) {
					glCompressedTexImage2D ( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, currentSize, imageData );
					if ( glGetError () != 0 ) {
						// we have an error
						this->Clear ();
						return;
					}
				}
				else {
					glTexImage2D( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData);
					if ( glGetError () != 0 ) {
						// we have an error
						this->Clear ();
						return;
					}
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
u32 MOAITexture::GetHeight () {
	return this->mHeight;
}

//----------------------------------------------------------------//
size_t MOAITexture::GetMemoryUsage () {
	return _g_TextureSize;
}

//----------------------------------------------------------------//
u32 MOAITexture::GetWidth () {
	return this->mWidth;
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, cc8* debugname ) {

	this->Clear ();
	this->mLoader = new MOAITextureLoader ();
	
	this->mLoader->mTransform = 0;
	this->mLoader->mImage.Copy ( image );
	
	if ( debugname ) {
		this->mLoader->mFilename = debugname;
	}
	
	this->Bind ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( cc8* filename, u32 transform ) {

	this->Clear ();
	if ( !USFileSys::CheckFileExists ( filename )) return;

	this->mFilename = USFileSys::Expand ( filename );
	
	this->mLoader = new MOAITextureLoader ();
	this->mLoader->mTransform = transform;
	this->mLoader->mFilename = this->mFilename;
	
	this->Bind ();
}


//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	this->Init ( bytes, size, transform, debugname );
	
	data.Unlock ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( const void* data, u32 size, u32 transform, cc8* debugname ) {

	this->Clear ();
	this->mLoader = new MOAITextureLoader ();
	
	this->mLoader->mTransform = transform;
	this->mLoader->mFileDataSize = size;
	this->mLoader->mFileData = malloc ( size );
	memcpy ( this->mLoader->mFileData, data, size );
	
	if ( debugname ) {
		this->mLoader->mFilename = debugname;
	}
	
	this->Bind ();
}

//----------------------------------------------------------------//
void MOAITexture::InitFrameBuffer ( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat ) {

	if ( !this->mFrameBuffer ) {
		this->mFrameBuffer = new MOAIFrameBuffer ();
	}

	this->mWidth = width;
	this->mHeight = height;

	this->mFrameBuffer->Init ( width, height, colorFormat, depthFormat, stencilFormat );
}

//----------------------------------------------------------------//
bool MOAITexture::IsFrameBuffer () {

	return ( this->mFrameBuffer != 0 );
}

//----------------------------------------------------------------//
bool MOAITexture::IsOK () {
 
	return (( this->mLoader ) || ( this->mGLTexID != 0 ));
}

//----------------------------------------------------------------//
bool MOAITexture::IsReloadable () {
	
	return this->mLoader && this->mLoader->mReloadable;
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () :
	mGLTexID ( 0 ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( GL_LINEAR ),
	mMagFilter ( GL_NEAREST ),
	mWrap ( GL_REPEAT ),
	mLoader ( 0 ),
	mFrameBuffer ( 0 ),
	mDataSize ( 0 ),
	mLastFrameUsed ( 0 ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( USLuaState& state ) {
	
	state.SetField ( -1, "GL_LINEAR", ( u32 )GL_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_LINEAR", ( u32 )GL_LINEAR_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_NEAREST", ( u32 )GL_LINEAR_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_NEAREST", ( u32 )GL_NEAREST );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_LINEAR", ( u32 )GL_NEAREST_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_NEAREST", ( u32 )GL_NEAREST_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_RGBA4",				( u32 )GL_RGBA4 );
	//***state.SetField ( -1, "GL_RGBA8",				( u32 )GL_RGBA8 );
	//state.SetField ( -1, "GL_RGB565",				( u32 )GL_RGB565 );
	state.SetField ( -1, "GL_RGB5_A1",				( u32 )GL_RGB5_A1 );
	state.SetField ( -1, "GL_DEPTH_COMPONENT16",	( u32 )GL_DEPTH_COMPONENT16 );
	//***state.SetField ( -1, "GL_DEPTH_COMPONENT24",	( u32 )GL_DEPTH_COMPONENT24 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX1",		( u32 )GL_STENCIL_INDEX1 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX4",		( u32 )GL_STENCIL_INDEX4 );
	state.SetField ( -1, "GL_STENCIL_INDEX8",		( u32 )GL_STENCIL_INDEX8 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX16",		( u32 )GL_STENCIL_INDEX16 );
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "bind",					_bind },
		{ "getSize",				_getSize },
		{ "initFrameBuffer",		_initFrameBuffer },
		{ "load",					_load },
		{ "release",				_release },
		{ "setFilter",				_setFilter },
		{ "setWrap",				_setWrap },
		{ "softRelease",			_softRelease },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	STLString path = state.GetField ( -1, "mPath", "" );
	
	if ( path.size ()) {
		USFilename filename;
		filename.Bless ( path.str ());
		this->Init ( filename.mBuffer, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
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
}

//----------------------------------------------------------------//
void MOAITexture::SetWrap ( int wrap ) {

	this->mWrap = wrap;
}

//----------------------------------------------------------------//
bool MOAITexture::SoftRelease ( int age ) {
	
	if( !this->IsReloadable ())
		return false;
	
	if ( !this->mGLTexID ) {
		return false;
	}

	u32 f = MOAISim::Get ().GetFrameCounter ();
	u32 myage = f > this->mLastFrameUsed ? f - this->mLastFrameUsed : 0xffffffff - this->mLastFrameUsed + f;
	if ( myage < ( u32 )( age & 0x7fffffff )) {
		return false;
	}

	DEBUG_TEXTURE_FREE ( this->mFilename, this->mDataSize )

	glDeleteTextures ( 1, &this->mGLTexID );
	this->mGLTexID = 0;
	
	// Horrible to call this (especially since we're likely freeing textures
	// in a loop), but generally this is only called in response to a low 
	// memory warning and we want to free as soon as possible.
	glFlush ();
	
	return true;
}

//----------------------------------------------------------------//
STLString MOAITexture::ToString () {

	STLString repr;

	PrettyPrint ( repr, "mWidth", GetWidth ());
	PrettyPrint ( repr, "mHeight", GetHeight ());

	return repr;
}
