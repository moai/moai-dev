// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPvrHeader.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIMultiTexture.h>

//================================================================//
// local
//================================================================//

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

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
	u32 transform = state.GetValue < u32 >( 3, DEFAULT_TRANSFORM );

	if ( data ) {
		self->Init ( *data, transform, "" );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename = lua_tostring ( state, 2 );
		self->Init ( filename, transform );
	}
	return 0;
}

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState* MOAITexture::AffirmTexture ( MOAILuaState& state, int idx ) {

	MOAIGfxState* gfxState = state.GetLuaObject < MOAITextureBase >( idx, false );
	if ( !gfxState ) {
		
		gfxState = state.GetLuaObject < MOAIMultiTexture >( idx, false );
		
		if ( !gfxState ) {
			
			u32 transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );

			if ( state.IsType ( idx, LUA_TUSERDATA )) {
				
				MOAIImage* image = state.GetLuaObject < MOAIImage >( idx, false );
				
				if ( image ) {
					MOAITexture* texture = new MOAITexture ();
					texture->Init ( *image, "" );
					gfxState = texture;
				}
				else {
				
					MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( idx, false );
					
					if ( data ) {
						MOAITexture* texture = new MOAITexture ();
						texture->Init ( *data, transform, "" );
						gfxState = texture;
					}
				}
			}
			else if ( state.IsType ( idx, LUA_TSTRING )) {
				
				cc8* filename = lua_tostring ( state, idx );
				MOAITexture* texture = new MOAITexture ();
				texture->Init ( filename, transform );
				gfxState = texture;
			}
		}
	}
	return gfxState;
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
		this->mImage.Copy ( image );
		this->mDebugName = debugname;
		this->Load ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {

	this->Clear ();
	if ( image.IsOK ()) {

		this->mImage.Init ( width, height, image.GetColorFormat (), image.GetPixelFormat ());
		this->mImage.CopyBits ( image, srcX, srcY, 0, 0, width, height );
		
		this->mDebugName = debugname;
		this->Load ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( cc8* filename, u32 transform ) {

	this->Clear ();
	if ( USFileSys::CheckFileExists ( filename )) {

		this->mFilename = USFileSys::GetAbsoluteFilePath ( filename );
		this->mDebugName = this->mFilename;
		this->mTransform = transform;
		this->Load ();
	}
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
	
	this->mImage.Load ( data, size, transform );

	// if no image, check to see if the file is a PVR
	if ( !this->mImage.IsOK ()) {
		
		// get file data, check if PVR		
		if ( MOAIPvrHeader::GetHeader ( data, size )) {
			this->mData = malloc ( size );
			memcpy ( this->mData, data, size );
			this->mDataSize = size;			
		}
	}
	
	// if we're OK, store the debugname and load
	if ( this->mImage.IsOK () || this->mData ) {
		this->mDebugName = debugname;
		this->Load ();
	}
}

//----------------------------------------------------------------//
bool MOAITexture::IsRenewable () {

	if ( this->mFilename.size ()) return true;
	if ( this->mImage.IsOK ()) return true;
	if ( this->mData ) return true;
	
	return false;
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () :
	mTransform ( DEFAULT_TRANSFORM ),
	mData ( 0 ),
	mDataSize ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}

//----------------------------------------------------------------//
void MOAITexture::OnClear () {

	MOAITextureBase::OnClear ();

	this->mFilename.clear ();
	this->mDebugName.clear ();
	this->mImage.Clear ();
	
	if ( this->mData ) {
		free ( this->mData );
	}
	this->mDataSize = 0;
}

//----------------------------------------------------------------//
void MOAITexture::OnCreate () {
	
	if ( this->mImage.IsOK ()) {
		this->CreateTextureFromImage ( this->mImage );
	}
	else if ( this->mData ) {
		this->CreateTextureFromPVR ( this->mData, this->mDataSize );
	}

	if ( this->mFilename.size ()) {
		
		this->mImage.Clear ();
		
		if ( this->mData ) {
			free ( this->mData );
		}
		this->mDataSize = 0;
	}
}

//----------------------------------------------------------------//
void MOAITexture::OnLoad () {

	if ( this->mFilename.size ()) {
	
		this->mImage.Load ( this->mFilename, this->mTransform );
		
		if ( !this->mImage.IsOK ()) {
			
			// if no image, check to see if the file is a PVR
			USFileStream stream;
			stream.OpenRead ( this->mFilename );
			
			size_t size = stream.GetLength ();
			void* data = malloc ( size );
			stream.ReadBytes ( data, size );

			stream.Close ();
			
			if ( MOAIPvrHeader::GetHeader ( data, size )) {
				this->mData = data;
				this->mDataSize = size;		
			}
			else {
				free ( data );
			}
		}
	}
	
	if ( this->mImage.IsOK ()) {
		
		this->mWidth = this->mImage.GetWidth ();
		this->mHeight = this->mImage.GetHeight ();
	}
	else if ( this->mData ) {
	
		MOAIPvrHeader* header = MOAIPvrHeader::GetHeader ( this->mData, this->mDataSize );
		if ( header ) {
			this->mWidth = header->mWidth;
			this->mHeight = header->mHeight;
		}
	}
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITextureBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAITextureBase::SerializeIn ( state, serializer );
	
	STLString path = state.GetField ( -1, "mPath", "" );
	
	if ( path.size ()) {
		this->Init ( path, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBase::SerializeOut ( state, serializer );
	
	STLString path = USFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}

