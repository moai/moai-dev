// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPvrHeader.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIMultiTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	load
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
void MOAITexture::Clear () {

	this->Destroy ();

	this->mFilename.clear ();
	this->mDebugName.clear ();
	this->mImage.Set ( *this, 0 );
	
	if ( this->mData ) {
		free ( this->mData );
		this->mData = NULL;
	}
	this->mDataSize = 0;
}

//----------------------------------------------------------------//
bool MOAITexture::Init ( MOAILuaState& state, int idx ) {

	u32 transform = MOAITexture::DEFAULT_TRANSFORM;
	cc8* debugName = "";
	int debugNameIdx = 1;
	
	if ( state.IsType ( idx + 1, LUA_TNUMBER )) {
		transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );
		debugNameIdx++;
	}
	debugName = state.GetValue < cc8* >( debugNameIdx, "" );
		
	bool done = false;

	if ( state.IsType ( idx, LUA_TSTRING )) {
		cc8* filename = lua_tostring ( state, idx );
		u32 transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );
		this->Init ( filename, transform );
		done = true;
	}
	
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
	
		if ( !done ) {
			MOAIImage* image = state.GetLuaObject < MOAIImage >( idx, false );
			if ( image ) {
				this->Init ( *image, debugName ? debugName : "(texture from MOAIImage)" );
				done = true;
			}
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
			if ( stream ) {
				this->Init ( *stream, transform, debugName ? debugName : "(texture from MOAIStream)" );
				done = true;
			}
		}
	}
	return done;
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
		this->mImage.Set ( *this, &image );
		this->mDebugName = debugname;
		this->DoCPUAffirm ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( MOAIImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
		this->mImage.Set ( *this, new MOAIImage ());
		this->mImage->Init ( width, height, image.GetColorFormat (), image.GetPixelFormat ());
		this->mImage->Blit ( image, srcX, srcY, 0, 0, width, height );
		this->mDebugName = debugname;
		this->DoCPUAffirm ();
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( cc8* filename, u32 transform, cc8* debugname ) {

	this->Clear ();
	
	if ( MOAILogMessages::CheckFileExists ( filename )) {
		
		this->mFilename = ZLFileSys::GetAbsoluteFilePath ( filename );
		if ( debugname ) {
			this->mDebugName = debugname;
		}
		else {
			this->mDebugName = this->mFilename;
		}		
		this->mTransform = transform;
		this->DoCPUAffirm ();
	}
	else {
	
		STLString expand = ZLFileSys::GetAbsoluteFilePath ( filename );
		MOAILog ( NULL, MOAILogMessages::MOAI_FileNotFound_S, expand.str ());
	}
}

//----------------------------------------------------------------//
void MOAITexture::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {

	this->Clear ();
	
	MOAIPvrHeader header;
	header.Load ( stream );
	
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
	else {
	
		this->mImage.Set ( *this, new MOAIImage ());
		this->mImage->Load ( stream, transform );
		if ( !this->mImage->IsOK ()) {
			this->mImage.Set ( *this, 0 );
		}
	}
	
	// if we're OK, store the debugname and load
	if ( this->mData || ( this->mImage && this->mImage->IsOK ())) {
		this->mDebugName = debugname;
		this->DoCPUAffirm ();
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

	ZLByteStream stream;
	stream.SetBuffer ( data, size, size );
	this->Init ( stream, transform, debugname );
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

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAITexture::OnCPUCreate () {

	if ( this->mFilename.size ()) {
	
		this->mImage.Set ( *this, new MOAIImage ());
		this->mImage->Load ( this->mFilename, this->mTransform );
		
		if ( !this->mImage->IsOK ()) {
			
			this->mImage.Set ( *this, 0 );
			
			// if no image, check to see if the file is a PVR
			ZLFileStream stream;
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
			return false;
		}
	}
	
	if ( this->mImage && this->mImage->IsOK ()) {
		
		this->mWidth = this->mImage->GetWidth ();
		this->mHeight = this->mImage->GetHeight ();
		return true;
	}
	else if ( this->mData ) {
	
		MOAIPvrHeader* header = MOAIPvrHeader::GetHeader ( this->mData, this->mDataSize );
		if ( header ) {
			this->mWidth = header->mWidth;
			this->mHeight = header->mHeight;
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAITexture::OnCPUDestroy () {

	// if we know the filename it is safe to clear out
	// the image and/or buffer
	if ( this->HasReloader () || this->mFilename.size ()) {
		
		this->mImage.Set ( *this, 0 );
		
		if ( this->mData ) {
			free ( this->mData );
			this->mData = NULL;
		}
		this->mDataSize = 0;
	}
	
	if ( this->HasReloader ()) {
		this->mFilename.clear ();
	}
}

//----------------------------------------------------------------//
bool MOAITexture::OnGPUCreate () {
	
	bool success = false;
	
	if ( this->mImage && this->mImage->IsOK ()) {
		success =  this->CreateTextureFromImage ( *this->mImage );
	}
	else if ( this->mData ) {
		success = this->CreateTextureFromPVR ( this->mData, this->mDataSize );
	}
	
	if ( success ) return true;
	
	this->Clear ();
	return success;
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
	
	STLString path = ZLFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}

