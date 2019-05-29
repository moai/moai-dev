// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAITextureGL.h>
#include <moai-gfx-gl/ZLTextureFormat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	load
	@text	Loads a texture from a data buffer or a file. Optionally pass
			in an image transform (not applicable to PVR textures).
 
	@overload
		@in		MOAITextureGL self
		@in		string filename
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
 
	@overload
		@in		MOAITextureGL self
		@in		MOAIImage image
		@opt	string debugname		Name used when reporting texture debug information
		@opt	boolean autoClear		Default value is 'false.' Only used if there is a reloader in play.
		@out	nil
 
	@overload
		@in		MOAITextureGL self
		@in		MOAIDataBuffer buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
 
	@overload
		@in		MOAITextureGL self
		@in		MOAIStream buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
*/
int MOAITextureGL::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureGL, "U" )

	self->Init ( state, 2 );
	return 0;
}

//================================================================//
// MOAITextureGL
//================================================================//

//----------------------------------------------------------------//
void MOAITextureGL::Clear () {

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
void MOAITextureGL::ClearImage () {

	if ( this->mImage ) {
		delete this->mImage;
	}
	this->mImage = NULL;
}

//----------------------------------------------------------------//
void MOAITextureGL::CopyImage ( const ZLImage& image ) {

	this->ClearImage ();
	
	this->mImage = new ZLImage ();
	*this->mImage = image;
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( const ZLImage& image, cc8* debugname ) {

	this->Clear ();
	
	if ( image.IsOK ()) {
		this->CopyImage ( image );
		this->mDebugName = debugname;
		this->FinishInit ();
		this->DoCPUCreate (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {

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
void MOAITextureGL::Init ( cc8* filename, u32 transform, cc8* debugname ) {

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
void MOAITextureGL::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {

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
void MOAITextureGL::Init ( const void* data, size_t size, u32 transform, cc8* debugname ) {

	ZLByteStream stream;
	stream.SetBuffer ( data, size, size );
	this->Init ( stream, transform, debugname );
}

//----------------------------------------------------------------//
bool MOAITextureGL::Init ( MOAILuaState& state, int idx ) {

	u32 transform = MOAITextureGL::DEFAULT_TRANSFORM;
	cc8* debugName = 0;
	int debugNameIdx = 1;
	
	if ( state.IsType ( idx + 1, LUA_TNUMBER )) {
		transform = state.GetValue < u32 >( idx + 1, MOAITextureGL::DEFAULT_TRANSFORM );
		debugNameIdx++;
	}
	debugName = state.GetValue < cc8* >( debugNameIdx, 0 );
	
	bool done = false;

	if ( state.IsType ( idx, LUA_TSTRING )) {
		cc8* filename = lua_tostring ( state, idx );
		transform = state.GetValue < u32 >( idx + 1, MOAITextureGL::DEFAULT_TRANSFORM );
		this->Init ( filename, transform, debugName ? debugName : filename );
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
void MOAITextureGL::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );
	this->Init ( bytes, size, transform, debugname );
	data.Unlock ();
}

//----------------------------------------------------------------//
bool MOAITextureGL::LoadFromStream ( ZLStream& stream, u32 transform ) {
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
MOAITextureGL::MOAITextureGL () :
	mTransform ( DEFAULT_TRANSFORM ),
	mTextureData ( 0 ),
	mTextureDataSize ( 0 ),
	mTextureDataFormat ( 0 ),
	mImage ( NULL ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAITextureBaseGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureGL::~MOAITextureGL () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITextureGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITextureBaseGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITextureGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITextureBaseGL::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextureGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	STLString path = state.GetFieldValue ( -1, "mPath", "" );
	
	if ( path.size ()) {
		this->Init ( path, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITextureGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	STLString path = ZLFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITextureGL::ZLAbstractGfxResource_OnCPUCreate () {

	if ( this->mFilename.size ()) {
		ZLFileStream stream;
		stream.OpenRead ( this->mFilename );
		this->LoadFromStream ( stream, this->mTransform );
		stream.Close ();
	}
	return (( this->mImage && this->mImage->IsOK ()) || this->mTextureData );
}

//----------------------------------------------------------------//
void MOAITextureGL::ZLAbstractGfxResource_OnCPUDestroy () {

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
bool MOAITextureGL::ZLAbstractGfxResource_OnGPUCreate () {
	
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
