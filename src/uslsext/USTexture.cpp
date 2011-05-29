// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USData.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USTexture.h>

//================================================================//
// USTextureLoader
//================================================================//
class USTextureLoader {
public:

	STLString	mFilename;
	void*		mFileData;
	u32			mFileDataSize;
	USImage		mImage;
	u32			mTransform;
	bool		mDone;
	
	//----------------------------------------------------------------//
	USImage* Load ( u32 transform = 0 ) {
	
		if ( this->mDone ) {
			if ( this->mImage.IsOK ()) {
				return &this->mImage;
			}
			return 0;
		}
	
		this->mDone = true;
		this->mTransform |= transform;
	
		if ( !this->mImage.IsOK ()) {
	
			if ( this->mFileData ) {
				this->mImage.Load ( this->mFileData, this->mFileDataSize, this->mTransform );
				free ( this->mFileData );
				this->mFileData = 0;
			}
			else if ( mFilename.size ()) {
				this->mImage.Load ( this->mFilename, this->mTransform );
			}
			
			
		}
		
		if ( this->mImage.IsOK ()) {
			return &this->mImage;
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	USTextureLoader () :
		mFileData ( 0 ),
		mFileDataSize ( 0 ),
		mDone ( false ) {
	}
	
	//----------------------------------------------------------------//
	~USTextureLoader () {
	
		if ( this->mFileData ) {
			free ( this->mFileData );
		}
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
	
	USImage* image = this->mLoader->Load ();
	
	if ( image ) {
		this->CreateTexture ( *image );
	}

	if ( this->mGLTexID ) {

		// done with the loader
		delete this->mLoader;
		this->mLoader = 0;
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
void USTexture::CreateTexture ( USImage& image ) {

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
u32 USTexture::GetHeight () {
	return this->mHeight;
}

//----------------------------------------------------------------//
u32 USTexture::GetWidth () {
	return this->mWidth;
}

//----------------------------------------------------------------//
void USTexture::Init ( USData& data, u32 transform ) {

	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	this->Init ( bytes, size, transform );
	
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
void USTexture::Init ( const void* data, u32 size, u32 transform ) {

	this->Release ();
	this->mLoader = new USTextureLoader ();
	
	this->mLoader->mTransform = transform;
	this->mLoader->mFileDataSize = size;
	this->mLoader->mFileData = malloc ( size );
	memcpy ( this->mLoader->mFileData, data, size );
	
	this->Bind ();
}

//----------------------------------------------------------------//
void USTexture::Init ( USImage& image ) {

	this->Release ();
	this->mLoader = new USTextureLoader ();
	
	this->mLoader->mTransform = 0;
	this->mLoader->mImage.Copy ( image );
	
	this->Bind ();
}

//----------------------------------------------------------------//
bool USTexture::IsOK () {
 
	return (( this->mLoader ) || ( this->mGLTexID != 0 ));
}

//----------------------------------------------------------------//
void USTexture::Release () {

	if ( this->mGLTexID ) {
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
USTexture::USTexture () :
	mGLTexID ( 0 ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mMinFilter ( GL_LINEAR ),
	mMagFilter ( GL_NEAREST ),
	mWrap ( GL_CLAMP ),
	mLoader ( 0 ) {
}

//----------------------------------------------------------------//
USTexture::~USTexture () {

	this->Release ();
}
