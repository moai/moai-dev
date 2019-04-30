// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAITexture.h>

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
		@opt	boolean autoClear		Default value is 'false.' Only used if there is a reloader in play.
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
MOAITextureBase* MOAITexture::AffirmTexture ( MOAILuaState& state, int idx ) {

	MOAITextureBase* textureBase = 0;
	
	textureBase = state.GetLuaObject < MOAITextureBase >( idx, false );
	if ( textureBase ) return textureBase;
	
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

	u32 transform = MOAITexture::DEFAULT_TRANSFORM;
	cc8* debugName = 0;
	int debugNameIdx = 1;
	
	if ( state.IsType ( idx + 1, LUA_TNUMBER )) {
		transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );
		debugNameIdx++;
	}
	debugName = state.GetValue < cc8* >( debugNameIdx, 0 );
	
	bool done = false;

	if ( state.IsType ( idx, LUA_TSTRING )) {
		cc8* filename = lua_tostring ( state, idx );
		transform = state.GetValue < u32 >( idx + 1, MOAITexture::DEFAULT_TRANSFORM );
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
void MOAITexture::Init ( MOAIDataBuffer& data, u32 transform, cc8* debugname ) {

	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );
	this->Init ( bytes, size, transform, debugname );
	data.Unlock ();
}

//----------------------------------------------------------------//
void MOAITexture::Init ( const ZLImage& image, cc8* debugname ) {
	this->ZLTexture::Init ( image, debugname );
}

//----------------------------------------------------------------//
void MOAITexture::Init ( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {
	this->ZLTexture::Init ( image, srcX, srcY, width, height, debugname );
}

//----------------------------------------------------------------//
void MOAITexture::Init ( cc8* filename, u32 transform, cc8* debugname ) {
	this->ZLTexture::Init ( filename, transform, debugname );
}

//----------------------------------------------------------------//
void MOAITexture::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {
	this->ZLTexture::Init ( stream, transform, debugname );
}

//----------------------------------------------------------------//
void MOAITexture::Init ( const void* data, size_t size, u32 transform, cc8* debugname ) {
	this->ZLTexture::Init ( data, size, transform, debugname );
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractGfxResource )
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIGfxResource < ZLTexture >::RegisterLuaClass ( state );
	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource < ZLTexture >::RegisterLuaFuncs ( state );
	MOAITextureBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	STLString path = state.GetFieldValue ( -1, "mPath", "" );
	
	if ( path.size ()) {
		this->Init ( path, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	STLString path = ZLFileSys::GetRelativePath ( this->mFilename );
	state.SetField ( -1, "mPath", path.str ());
}
