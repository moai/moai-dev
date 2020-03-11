// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAITexture2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	load
	@text	Loads a texture from a data buffer or a file. Optionally pass
			in an image transform (not applicable to PVR textures).
 
	@overload
		@in		MOAITexture2DGL self
		@in		string filename
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
 
	@overload
		@in		MOAITexture2DGL self
		@in		MOAIImage image
		@opt	string debugname		Name used when reporting texture debug information
		@opt	boolean autoClear		Default value is 'false.' Only used if there is a reloader in play.
		@out	nil
 
	@overload
		@in		MOAITexture2DGL self
		@in		MOAIDataBuffer buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
 
	@overload
		@in		MOAITexture2DGL self
		@in		MOAIStream buffer
		@opt	number transform		Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
		@opt	string debugname		Name used when reporting texture debug information
		@out	nil
*/
int MOAITexture2D::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture2D, "U" )

	self->Init ( state, 2 );
	return 0;
}

//================================================================//
// MOAITexture2D
//================================================================//

//----------------------------------------------------------------//
void MOAITexture2D::ClearImage () {

	if ( this->mImage ) {
		delete this->mImage;
	}
	this->mImage = NULL;
}

//----------------------------------------------------------------//
void MOAITexture2D::CopyImage ( const ZLImage& image ) {

	this->ClearImage ();
	
	this->mImage = new ZLImage ();
	*this->mImage = image;
}

//----------------------------------------------------------------//
void MOAITexture2D::Init ( const ZLImage& image, cc8* debugname ) {

	this->Finalize ();
	
	if ( image.IsOK ()) {
		this->CopyImage ( image );
		
		this->mDebugName = debugname;
		this->FinishLoading (); // If you do not calculate here, it is impossible to get the texture size.
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::Init ( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {

	this->Finalize ();
	
	if ( image.IsOK ()) {
	
		this->mImage = new ZLImage ();
		
		this->mImage->Init ( width, height, image.GetColorFormat (), image.GetPixelFormat ());
		this->mImage->Blit ( image, srcX, srcY, 0, 0, width, height );
		
		this->mDebugName = debugname;
		this->FinishLoading (); // Force CPU create here to get the texture size.
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::Init ( cc8* filename, u32 transform, cc8* debugname ) {

	this->Finalize ();
	
	if ( ZLFileSys::CheckFileExists ( filename )) {
		
		this->mFilename = ZLFileSys::GetAbsoluteFilePath ( filename );
		if ( debugname ) {
			this->mDebugName = debugname;
		}
		else {
			this->mDebugName = this->mFilename;
		}
		this->mTransform = transform;
		this->FinishLoading (); // Force CPU create here to get the texture size.
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {

	this->Finalize ();
	this->LoadFromStream ( stream, transform );
	
	// if we're OK, store the debugname and load
	if ( this->mTextureData || ( this->mImage && this->mImage->IsOK ())) {
	
		this->mDebugName = debugname;
		this->FinishLoading (); // Force CPU create here to get the texture size.
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::Init ( const void* data, size_t size, u32 transform, cc8* debugname ) {

	ZLByteStream stream;
	stream.SetBuffer ( data, size, size );
	this->Init ( stream, transform, debugname );
}

//----------------------------------------------------------------//
bool MOAITexture2D::Init ( MOAILuaState& state, int idx ) {

	u32 transform = MOAITexture2D::DEFAULT_TRANSFORM;
	cc8* debugName = 0;
	int debugNameIdx = 1;
	
	if ( state.IsType ( idx + 1, LUA_TNUMBER )) {
		transform = state.GetValue < u32 >( idx + 1, MOAITexture2D::DEFAULT_TRANSFORM );
		debugNameIdx++;
	}
	debugName = state.GetValue < cc8* >( debugNameIdx, 0 );
	
	bool done = false;

	if ( state.IsType ( idx, LUA_TSTRING )) {
		cc8* filename = lua_tostring ( state, idx );
		transform = state.GetValue < u32 >( idx + 1, MOAITexture2D::DEFAULT_TRANSFORM );
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
void MOAITexture2D::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );
	this->Init ( bytes, size, transform, debugname );
	data.Unlock ();
}

//----------------------------------------------------------------//
bool MOAITexture2D::LoadFromStream ( ZLStream& stream, u32 transform ) {
	UNUSED ( transform ); // TODO: why is transform unused?

	bool result = false;
	ZLImageFormat* imageFormat = ZLImageFormatMgr::Get ().FindFormat ( stream );
//	ZLTextureFormat* textureFormat = imageFormat ? imageFormat->AsType < ZLTextureFormat >() : NULL;
	
	if ( imageFormat ) {

		this->mTextureDataFormat = NULL;

		// TODO: Gfx
//		if ( textureFormat ) {
//
//			ZLTextureInfo textureInfo;
//
//			if ( textureFormat->GetTextureInfo ( stream, textureInfo )) {
//
//				void* data = malloc ( textureInfo.mSize );
//				size_t size = stream.ReadBytes ( data, textureInfo.mSize );
//
//				if ( size == textureInfo.mSize ) {
//
//					this->mTextureData = data;
//					this->mTextureDataSize = size;
//					this->mTextureDataFormat = textureFormat;
//
//					this->mWidth = textureInfo.mWidth;
//					this->mHeight = textureInfo.mHeight;
//
//					result = true;
//				}
//				else {
//					free ( data );
//				}
//			}
//		}
		
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
MOAITexture2D::MOAITexture2D () :
	mTransform ( DEFAULT_TRANSFORM ),
	mTextureData ( 0 ),
	mTextureDataSize ( 0 ),
	mTextureDataFormat ( 0 ),
	mImage ( NULL ) {

	RTTI_BEGIN ( MOAITexture2D )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAITexture2D >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAITexture2D >)
		RTTI_EXTEND ( MOAITexture )
		this->RegisterFinalizationVisitor < MOAITexture2D >();
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2D::~MOAITexture2D () {

	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITexture2D::_Finalize () {

	if ( this->mImage ) {
		delete this->mImage;
	}
	
	if ( this->mTextureData ) {
		free ( this->mTextureData );
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITexture2D::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture2D::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
	
	STLString path = state.GetFieldValue ( -1, "mPath", "" );
	
	if ( path.size ()) {
		this->Init ( path, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture2D::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
	
	STLString path = ZLFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}

//----------------------------------------------------------------//
void MOAITexture2D::MOAIGfxResource_ClearReloadable () {

	// if we know the filename it is safe to clear out
	// the image and/or buffer
	if ( this->HasReloader () || this->mFilename.size ()) {
		
		// force cleanup right away - the image is now in OpenGL, why keep it around until the next GC?
		this->ClearImage ();
		
		if ( this->mTextureData ) {
			free ( this->mTextureData );
			this->mTextureData = NULL;
		}
		this->mTextureDataSize = 0;
		this->mTextureDataFormat = NULL;
	}
	
	if ( this->HasReloader ()) {
		this->mFilename.clear ();
	}
}

//----------------------------------------------------------------//
bool MOAITexture2D::MOAIGfxResource_FinishLoading () {

	if ( this->mFilename.size ()) {
		ZLFileStream stream;
		stream.OpenRead ( this->mFilename );
		this->LoadFromStream ( stream, this->mTransform );
		stream.Close ();
	}
	return (( this->mImage && this->mImage->IsOK ()) || this->mTextureData );
}
