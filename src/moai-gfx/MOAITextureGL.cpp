// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAITextureGL.h>

//================================================================//
// local
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
void MOAITextureGL::Init ( const ZLImage& image, cc8* debugname ) {
	this->ZLTextureGL::Init ( image, debugname );
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname ) {
	this->ZLTextureGL::Init ( image, srcX, srcY, width, height, debugname );
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( cc8* filename, u32 transform, cc8* debugname ) {
	this->ZLTextureGL::Init ( filename, transform, debugname );
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( ZLStream& stream, u32 transform, cc8* debugname ) {
	this->ZLTextureGL::Init ( stream, transform, debugname );
}

//----------------------------------------------------------------//
void MOAITextureGL::Init ( const void* data, size_t size, u32 transform, cc8* debugname ) {
	this->ZLTextureGL::Init ( data, size, transform, debugname );
}

//----------------------------------------------------------------//
MOAITextureGL::MOAITextureGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResourceGL < ZLTextureGL >)
		RTTI_EXTEND ( MOAITextureBaseGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureGL::~MOAITextureGL () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITextureGL::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIGfxResourceGL < ZLTextureGL >::RegisterLuaClass ( state );
	MOAITextureBaseGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITextureGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResourceGL < ZLTextureGL >::RegisterLuaFuncs ( state );
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
