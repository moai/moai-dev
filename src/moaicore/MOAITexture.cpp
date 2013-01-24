// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPvrHeader.h>
#include <moaicore/MOAIStream.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIMultiTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads a texture from a data buffer or a file. Optionally pass
			in an image transform (not applicable to PVR textures).
	
	@overload
		@in		MOAITexture self
		@in		string filename
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
	
	@overload
		@in		MOAITexture self
		@in		MOAIImage image
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
	
	@overload
		@in		MOAITexture self
		@in		MOAIDataBuffer buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
	
	@overload
		@in		MOAITexture self
		@in		MOAIStream buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
*/
int MOAITexture::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture, "U" )

	self->Init ( state, 2 );
	return 0;
}

//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState* MOAITexture::AffirmTexture ( MOAILuaState& state, int idx ) {

	MOAIGfxState* gfxState = 0;
	
	gfxState = state.GetLuaObject < MOAITextureBase >( idx, false );
	if ( gfxState ) return gfxState;
	
	gfxState = state.GetLuaObject < MOAIMultiTexture >( idx, false );
	if ( gfxState ) return gfxState;
	
	MOAITexture* texture = new MOAITexture ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}

//----------------------------------------------------------------//
bool MOAITexture::Init ( MOAILuaState& state, int idx ) {

	u32 transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );
	cc8* debugName = state.GetValue < cc8* >( idx + 2, 0 );

	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		
		bool done = false;
		
		MOAIImage* image = state.GetLuaObject < MOAIImage >( idx, false );
		if ( image ) {
			this->Init ( *image, debugName ? debugName : "(texture from MOAIImage)" );
			done = true;
		}
		
		if ( !done ) {
			MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( idx, false );
			if ( data ) {
				this->Init ( *data, transform, debugName ? debugName : "(texture from MOAIDataBuffer)" );
				done = true;
			}
		}
		
		if ( !done ) {
			MOAIStream* stream = state.GetLuaObject < MOAIStream >( idx, false );
			if ( stream && stream->GetUSStream ()) {
				this->Init ( *stream->GetUSStream (), transform, debugName ? debugName : "(texture from MOAIStream)" );
				done = true;
			}
		}
		
		return done;
	}
	else if ( state.IsType ( idx, LUA_TSTRING )) {
		
		cc8* filename = lua_tostring ( state, idx );
		this->Init ( filename, transform );
		return true;
	}
	return false;
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
void MOAITexture::Init ( cc8* filename, u32 transform, cc8* debugname ) {

	this->Clear ();
	if ( MOAILogMessages::CheckFileExists ( filename )) {
		
		this->mFilename = USFileSys::GetAbsoluteFilePath ( filename );
		if ( debugname ) {
			this->mDebugName = debugname;
		}
		else {
			this->mDebugName = this->mFilename;
		}		
		this->mTransform = transform;
		this->Load ();
	} else {
			
		STLString expand = USFileSys::GetAbsoluteFilePath ( filename );
		MOAILog ( NULL, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());
			
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( USStream& stream, u32 transform, cc8* debugname ) {

	this->Clear ();
	
	this->mImage.Load ( stream, transform );
	
	// if no image, check to see if the file is a PVR
	if ( !this->mImage.IsOK ()) {
		
		MOAIPvrHeader header;
		header.Load ( stream );
		
		// get file data, check if PVR		
		if ( header.IsValid ()) {
			
			u32 size = header.GetTotalSize ();
			
			this->mData = malloc ( size );
			this->mDataSize = size;		
			
			size = stream.ReadBytes ( this->mData, size );
			
			if ( size != this->mDataSize ) {
				free ( this->mData );
				this->mData = 0;
				this->mDataSize = 0;
			}
		}
	}
	
	// if we're OK, store the debugname and load
	if ( this->mImage.IsOK () || this->mData ) {
		this->mDebugName = debugname;
		this->Load ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );

	USByteStream stream;
	stream.SetBuffer ( bytes, size, size );

	this->Init ( stream, transform, debugname );
	
	data.Unlock ();
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
		this->mData = NULL;
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
			this->mData = NULL;
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

