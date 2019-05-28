// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLTextureGL.h>
#include <zl-gfx/ZLTextureFormat.h>

//================================================================//
// ZLTextureGL
//================================================================//

//----------------------------------------------------------------//
void ZLTextureGL::Clear () {

	this->Destroy ();

	this->mFilename.clear ();
	this->mDebugName.clear ();
	
	this->ClearImage ();

	if ( this->mTextureData ) {
		free ( this->mTextureData );
		this->mTextureData = 0;
	}
	this->mTextureDataSize = 0;
	this->mTextureDataFormat = 0;
}

//----------------------------------------------------------------//
void ZLTextureGL::ClearImage () {

	if ( this->mImage ) {
		delete this->mImage;
	}
	this->mImage = NULL;
}

//----------------------------------------------------------------//
void ZLTextureGL::CopyImage ( const ZLImage& image ) {

	this->ClearImage ();
	
	this->mImage = new ZLImage ();
	*this->mImage = image;
}

//----------------------------------------------------------------//
void ZLTextureGL::Init ( const ZLImage& image, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
		this->CopyImage ( image );
		this->mDebugName = debugname;
		this->FinishInit ();
		this->DoCPUCreate (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void ZLTextureGL::Init ( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
	
		this->mImage = new ZLImage ();
		
		this->mImage->Init ( width, height, image.GetColorFormat (), image.GetPixelFormat ());
		this->mImage->Blit ( image, srcX, srcY, 0, 0, width, height );
		this->mDebugName = debugname;
		this->FinishInit ();
		this->DoCPUCreate (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void ZLTextureGL::Init ( cc8* filename, u32 transform, cc8* debugname ) {

	this->Clear ();
	
	if ( ZLFileSys::CheckFileExists ( filename )) {
		
		this->mFilename = ZLFileSys::GetAbsoluteFilePath ( filename );
		if ( debugname ) {
			this->mDebugName = debugname;
		}
		else {
			this->mDebugName = this->mFilename;
		}		
		this->mTransform = transform;
		this->FinishInit ();
		this->DoCPUCreate (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void ZLTextureGL::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {

	this->Clear ();
	this->LoadFromStream ( stream, transform );
	
	// if we're OK, store the debugname and load
	if ( this->mTextureData || ( this->mImage && this->mImage->IsOK ())) {
		this->mDebugName = debugname;
		this->FinishInit ();
		this->DoCPUCreate (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void ZLTextureGL::Init ( const void* data, size_t size, u32 transform, cc8* debugname ) {

	ZLByteStream stream;
	stream.SetBuffer ( data, size, size );
	this->Init ( stream, transform, debugname );
}

//----------------------------------------------------------------//
bool ZLTextureGL::LoadFromStream ( ZLStream& stream, u32 transform ) {
	UNUSED ( transform ); // TODO: why is transform unused?

	bool result = false;
	ZLImageFormat* imageFormat = ZLImageFormatMgr::Get ().FindFormat ( stream );
	ZLTextureFormat* textureFormat = imageFormat ? imageFormat->AsType < ZLTextureFormat >() : NULL;
	
	if ( imageFormat ) {

		this->mTextureDataFormat = NULL;

		if ( textureFormat ) {
		
			ZLTextureInfo textureInfo;
	
			if ( textureFormat->GetTextureInfo ( stream, textureInfo )) {
				
				void* data = malloc ( textureInfo.mSize );
				size_t size = stream.ReadBytes ( data, textureInfo.mSize );
				
				if ( size == textureInfo.mSize ) {
				
					this->mTextureData = data;
					this->mTextureDataSize = size;
					this->mTextureDataFormat = textureFormat;
					
					this->mWidth = textureInfo.mWidth;
					this->mHeight = textureInfo.mHeight;
					
					result = true;
				}
				else {
					free ( data );
				}
			}
		}
		
		if ( !this->mTextureDataFormat ) {
		
			ZLImage* image = new ZLImage ();
			imageFormat->ReadImage ( *image, stream, this->mTransform );
			
			if ( image->IsOK ()) {
				this->mImage = image;
				this->mWidth = image->GetWidth ();
				this->mHeight = image->GetHeight ();
				result = true;
			}
			else {
				delete image;
			}
		}
	}
	return result;
}

//----------------------------------------------------------------//
ZLTextureGL::ZLTextureGL () :
	mTransform ( DEFAULT_TRANSFORM ),
	mTextureData ( 0 ),
	mTextureDataSize ( 0 ),
	mTextureDataFormat ( 0 ),
	mImage ( NULL ) {

	RTTI_BEGIN
		RTTI_EXTEND ( ZLTextureBaseGL )
	RTTI_END
}

//----------------------------------------------------------------//
ZLTextureGL::~ZLTextureGL () {

	this->Clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLTextureGL::ZLAbstractGfxResource_OnCPUCreate () {

	if ( this->mFilename.size ()) {
		ZLFileStream stream;
		stream.OpenRead ( this->mFilename );
		this->LoadFromStream ( stream, this->mTransform );
		stream.Close ();
	}
	return (( this->mImage && this->mImage->IsOK ()) || this->mTextureData );
}

//----------------------------------------------------------------//
void ZLTextureGL::ZLAbstractGfxResource_OnCPUDestroy () {

	// if we know the filename it is safe to clear out
	// the image and/or buffer
	if ( this->HasLoader () || this->mFilename.size ()) {
		
		// force cleanup right away - the image is now in OpenGL, why keep it around until the next GC?
		this->ClearImage ();
		
		if ( this->mTextureData ) {
			free ( this->mTextureData );
			this->mTextureData = 0;
		}
		this->mTextureDataSize = 0;
		this->mTextureDataFormat = 0;
	}
	
	if ( this->HasLoader ()) {
		this->mFilename.clear ();
	}
}

//----------------------------------------------------------------//
bool ZLTextureGL::ZLAbstractGfxResource_OnGPUCreate () {
	
	bool success = false;
	
	if ( this->mImage && this->mImage->IsOK ()) {
		success =  this->CreateTextureFromImage ( *this->mImage );
	}
	else if ( this->mTextureDataFormat && this->mTextureData ) {
		success = this->mTextureDataFormat->CreateTexture ( *this, this->mTextureData, this->mTextureDataSize );
	}
	
	if ( !success ) {
		this->Clear ();
		return false;
	}
	
	return this->ZLAbstractGfxResource_OnGPUUpdate ();
}
