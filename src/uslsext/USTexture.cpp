// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USData.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USPvrHeader.h>
#include <uslsext/USTexture.h>
#include <uslsext/USDrawBuffer.h>

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

size_t USTexture::GetMemoryUsage()
{
	return _g_TextureSize;
}


//================================================================//
// USTextureLoader
//================================================================//
class USTextureLoader {
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
	USImage			mImage;
	u32				mTransform;
	u32				mType;
	bool			mReloadable;
	
	void Release() {
		if ( this->mFileData )
		{
			free ( this->mFileData );
			this->mFileData = 0;
		}
		
		this->mImage.Release();
	}
	
	
	//----------------------------------------------------------------//
	void Load ( u32 transform = 0 ) {
	
		if ( this->mType != TYPE_UNKNOWN && !mReloadable ) {
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
				this->mReloadable = this->mImage.IsOK();
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
				
				if ( USPvrHeader::GetHeader( this->mFileData, this->mFileDataSize )) {				
					this->mType = TYPE_PVR;
					this->mReloadable = true;
				}
			}
		#endif
		
		if ( this->mType == TYPE_UNKNOWN ) {
			this->mType = TYPE_FAIL;
			this->Release();
		}
	}
	
	//----------------------------------------------------------------//
	USTextureLoader () :
		mFileData ( 0 ),
		mFileDataSize ( 0 ),
		mType ( TYPE_UNKNOWN ),
		mReloadable ( false ) {
	}
	
	//----------------------------------------------------------------//
	~USTextureLoader () {
		Release();
	}
};

//================================================================//
// USTexture
//================================================================//

//----------------------------------------------------------------//
void USTexture::AffirmTexture () {

	if ( this->mGLTexID ) return;
	
	if ( !this->mLoader ) {
		return;
	}
	
	//if ( !GLEW_OES_compressed_paletted_texture ) {
	//	transform.ConvertToTrueColor ();
	//}
	
	this->mLoader->Load ();
	
	switch ( this->mLoader->mType ) {
		
		case USTextureLoader::TYPE_MOAI_IMAGE: {
			this->CreateTextureFromImage ( this->mLoader->mImage );
			break;
		}
		
		case USTextureLoader::TYPE_PVR: {
			this->CreateTextureFromPVR ( this->mLoader->mFileData, this->mLoader->mFileDataSize );
			break;
		}
		default:
			delete this->mLoader;
			this->mLoader = 0;
	}

	if ( this->mGLTexID ) {
#ifdef DEBUG_TEXTURE_MEMORY
		if ( this->mFilename.size() == 0 && this->mLoader )
		{
			// Loader's filename doesn't seem to be copied in all code paths...
			DEBUG_TEXTURE_ALLOC( this->mLoader->mFilename, this->mDataSize)
		}
		else
		{
			DEBUG_TEXTURE_ALLOC( this->mFilename, this->mDataSize)
		}
#endif
		
		this->mLastFrameUsed = USDrawBuffer::Get().GetFrameCounter();
		
		if( this->mLoader->mReloadable )
		{
			// Release the loader's copy. It isn't needed any more.
			this->mLoader->Release();
		}
		else
		{
			// done with the loader entirely
			delete this->mLoader;
			this->mLoader = 0;
		}
	}
}

//----------------------------------------------------------------//
bool USTexture::Bind () {

	this->AffirmTexture ();
	if ( !this->mGLTexID ) {
		return false;
	}

	// attempt to recover from lost context
	if ( !glIsTexture ( this->mGLTexID )) {
	
		this->mGLTexID = 0;
	
		// ugh... fix this monstrosity later!
		if ( this->mFilename.size ()) {
			this->Init ( this->mFilename );
			this->AffirmTexture ();
		}
		if ( !this->mGLTexID ) return false;
	}
	
	this->mLastFrameUsed = USDrawBuffer::Get().GetFrameCounter();

	glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );

	glEnable ( GL_TEXTURE_2D );
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mWrap );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mWrap );
	
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mMinFilter );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->mMagFilter );
	
	return true;
}

//----------------------------------------------------------------//
void USTexture::CreateTextureFromImage ( USImage& image ) {

	if ( !image.IsOK ()) return;

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
		
#ifdef DEBUG_TEXTURE_MEMORY
		this->mDataSize = image.GetBitmapSize();
#endif
		
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
void USTexture::CreateTextureFromPVR ( void* data, size_t size ) {
	UNUSED ( data );
	UNUSED ( size );

	#ifdef MOAI_TEST_PVR

		USPvrHeader* header = USPvrHeader::GetHeader ( data, size );
		if ( !header ) return;
		
		bool compressed = false;
		bool hasAlpha = header->mAlphaBitMask ? true : false;
		
		switch ( header->mPFFlags & USPvrHeader::PF_MASK ) {
			
			case USPvrHeader::OGL_RGBA_4444:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_SHORT_4_4_4_4;
				break;
		
			case USPvrHeader::OGL_RGBA_5551:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			
			case USPvrHeader::OGL_RGBA_8888:
				compressed = false;
				this->mGLInternalFormat = GL_RGBA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USPvrHeader::OGL_RGB_565:
				compressed = false;
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_SHORT_5_6_5;
				break;
			
			// NO IMAGE FOR THIS
//			case USPvrHeader::OGL_RGB_555:
//				break;
			
			case USPvrHeader::OGL_RGB_888:
				compressed = false;
				this->mGLInternalFormat = GL_RGB;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USPvrHeader::OGL_I_8:
				compressed = false;
				this->mGLInternalFormat = GL_LUMINANCE;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USPvrHeader::OGL_AI_88:
				compressed = false;
				this->mGLInternalFormat = GL_LUMINANCE_ALPHA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USPvrHeader::OGL_PVRTC2:
				compressed = true;
				this->mGLInternalFormat = hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
				break;
			
			case USPvrHeader::OGL_PVRTC4:
				compressed = true;
				this->mGLInternalFormat = hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
				break;
			
			case USPvrHeader::OGL_BGRA_8888:
				compressed = false;
				this->mGLInternalFormat = GL_BGRA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
			
			case USPvrHeader::OGL_A_8:
				compressed = false;
				this->mGLInternalFormat = GL_ALPHA;
				this->mGLPixelType = GL_UNSIGNED_BYTE;
				break;
		}
		
		
		glGenTextures ( 1, &this->mGLTexID );
		if ( !this->mGLTexID ) return;

		glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
		
	
#ifdef DEBUG_TEXTURE_MEMORY
		this->mDataSize = 0;
#endif
		int width = header->mWidth;
		int height = header->mHeight;
		char* imageData = (char*)(header->GetFileData ( data, size));
		if ( header->mMipMapCount == 0 ) {
			
			GLsizei currentSize = (GLsizei) USFloat::Max ( (float)(32), (float)(width * height * header->mBitCount / 8) );
#ifdef DEBUG_TEXTURE_MEMORY
			this->mDataSize += currentSize;
#endif
			if ( compressed ) {
				glCompressedTexImage2D ( GL_TEXTURE_2D, 0, this->mGLInternalFormat, width, height, 0, currentSize, imageData );
			}
			else {
				glTexImage2D( GL_TEXTURE_2D, 0, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData);
			}
		}
		else {
			
			for ( int level = 0; width > 0 && height > 0; ++level ) {
				GLsizei currentSize = (GLsizei) USFloat::Max ( (float)(32), (float)(width * height * header->mBitCount / 8) );
			
				if ( compressed ) {
					glCompressedTexImage2D ( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, currentSize, imageData );
				}
				else {
					glTexImage2D( GL_TEXTURE_2D, level, this->mGLInternalFormat, width, height, 0, this->mGLInternalFormat, this->mGLPixelType, imageData);
				}
			
				imageData += currentSize;
#ifdef DEBUG_TEXTURE_MEMORY
				this->mDataSize += currentSize;
#endif
				width >>= 1;
				height >>= 1;
			}
		}	
	#endif
}

//----------------------------------------------------------------//
u32 USTexture::GetHeight () {
	return this->mHeight;
}

//----------------------------------------------------------------//
u32 USTexture::GetWidth () {
	return this->mWidth;
}

//----------------------------------------------------------------//
void USTexture::Init ( USData& data, u32 transform, cc8* debugname ) {

	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	this->Init ( bytes, size, transform, debugname );
	
	data.Unlock ();
}

//----------------------------------------------------------------//
void USTexture::Init ( cc8* filename, u32 transform ) {

	this->Release ();
	if ( !USFileSys::CheckFileExists ( filename )) return;
	this->mFilename = USFileSys::Expand ( filename );
	
	this->mLoader = new USTextureLoader ();
	this->mLoader->mTransform = transform;
	this->mLoader->mFilename = this->mFilename;
	
	this->Bind ();
}

//----------------------------------------------------------------//
void USTexture::Init ( const void* data, u32 size, u32 transform, cc8* debugname ) {

	this->Release ();
	this->mLoader = new USTextureLoader ();
	
	this->mLoader->mTransform = transform;
	this->mLoader->mFileDataSize = size;
	this->mLoader->mFileData = malloc ( size );
	memcpy ( this->mLoader->mFileData, data, size );
	if( debugname )
		this->mLoader->mFilename = debugname;
	
	this->Bind ();
}

//----------------------------------------------------------------//
void USTexture::Init ( USImage& image, cc8* debugname ) {

	this->Release ();
	this->mLoader = new USTextureLoader ();
	
	this->mLoader->mTransform = 0;
	this->mLoader->mImage.Copy ( image );
	if( debugname )
		this->mLoader->mFilename = debugname;
	
	this->Bind ();
}

//----------------------------------------------------------------//
bool USTexture::IsOK () {
 
	return (( this->mLoader ) || ( this->mGLTexID != 0 ));
}

//----------------------------------------------------------------//
bool USTexture::IsReloadable () {
	
	return this->mLoader && this->mLoader->mReloadable;
}

//----------------------------------------------------------------//
void USTexture::Release () {

	if ( this->mGLTexID ) {
#ifdef DEBUG_TEXTURE_MEMORY
		DEBUG_TEXTURE_FREE(this->mFilename, this->mDataSize)
#endif
		glDeleteTextures ( 1, &this->mGLTexID );
		this->mGLTexID = 0;
	}
	
	this->mWidth = 0;
	this->mHeight = 0;
	
	if ( this->mLoader ) {
		delete this->mLoader;
		this->mLoader = 0;
	}
}

//----------------------------------------------------------------//
void USTexture::SetFilter ( int filter ) {

	this->SetFilter ( filter, filter );
}

//----------------------------------------------------------------//
void USTexture::SetFilter ( int min, int mag ) {

	this->mMinFilter = min;
	this->mMagFilter = mag;
}

//----------------------------------------------------------------//
void USTexture::SetWrap ( int wrap ) {

	this->mWrap = wrap;
}

//----------------------------------------------------------------//
bool USTexture::SoftRelease ( int age ) {
	
	if( !this->IsReloadable() )
		return false;
	
	if( !this->mGLTexID )
		return false;
	
//	if( true )
//		return false;

	u32 f = USDrawBuffer::Get().GetFrameCounter();
	u32 myage = f > mLastFrameUsed ? f - mLastFrameUsed : 0xffffffff - mLastFrameUsed + f;
	if( myage < (u32)(age & 0x7fffffff) )
		return false;

#ifdef DEBUG_TEXTURE_MEMORY
	DEBUG_TEXTURE_FREE(this->mFilename, this->mDataSize)
#endif
	glDeleteTextures ( 1, &this->mGLTexID );
	this->mGLTexID = 0;
	// Horrible to call this (especially since we're likely freeing textures
	// in a loop), but generally this is only called in response to a low 
	// memory warning and we want to free as soon as possible.
	glFlush();
	
	return true;
}

//----------------------------------------------------------------//
USTexture::USTexture () :
	mGLTexID ( 0 ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( GL_LINEAR ),
	mMagFilter ( GL_NEAREST ),
	mWrap ( GL_CLAMP_TO_EDGE ),
	mLoader ( 0 ),
	mLastFrameUsed (0) {
}

//----------------------------------------------------------------//
USTexture::~USTexture () {

	this->Release ();
}
