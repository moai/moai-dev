// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAITexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>bind ( self )</tt>\n
\n
	Forces the texture to load.
	@param self (in)
*/
int MOAITexture::_bind ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "U" )
	
	self->Bind ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( width, height ) bind ( self )</tt>\n
\n
	Returns the width and height of the texture's source image.
	@param self (in)
	@param width (out)
	@param height (out)f
*/
int MOAITexture::_getSize ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "U" )
	
	lua_pushnumber ( state, self->mDevWidth );
	lua_pushnumber ( state, self->mDevHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>load ( self, filename|data )</tt>\n
\n
	Loads an image to associate with this texture.
	@param self (in)
	@param filename (in) The name of the image file to load.  Must be a .bmp, .jpg or .png.
	@param data (in) A MOAIDataBuffer object containing image data.  Must be one of the filetypes above.
*/
int MOAITexture::_load ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "U" )

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );
	u32 transform = state.GetValue < u32 >( 3, DEFAULT_TRANSFORM );

	if ( data ) {

		self->Load ( *data, transform );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename = lua_tostring ( state, 2 );
		self->Load ( filename, transform );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>release ( self )</tt>\n
\n
	Forces texture out of memory.
	@param self (in)
*/
int MOAITexture::_release ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "U" )
	
	self->USTexture::Release ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setFilter ( self, filter )</tt>\n
\n
	Sets the filter mode.
	@param self (in)
	@param filter (in) MOAITexture.GL_LINEAR or MOAITexture.GL_NEAREST.
*/
int MOAITexture::_setFilter ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "UN" )

	int min = state.GetValue < int >( 2, GL_LINEAR );
	int mag = state.GetValue < int >( 3, min );

	self->SetFilter ( min, mag );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITexture::_setWrap ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "UB" )
	
	bool wrap = state.GetValue < bool >( 2, false );
	
	self->mWrap = wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

	return 0;
}


//================================================================//
// MOAITexture
//================================================================//

//----------------------------------------------------------------//
MOAITexture* MOAITexture::AffirmTexture ( USLuaState& state, int idx ) {

	MOAITexture* texture = 0;

	u32 transform = state.GetValue < u32 >( idx + 1, DEFAULT_TRANSFORM );

	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		
		texture = state.GetLuaObject < MOAITexture >( idx );
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( idx );
		
		if ( data ) {
			texture = new MOAITexture ();
			texture->Load ( *data, transform );
		}
	}
	else if ( state.IsType ( idx, LUA_TSTRING )) {
		
		cc8* filename = lua_tostring ( state, idx );
		texture = new MOAITexture ();
		texture->Load ( filename, transform );
	}

	return texture;
}

//----------------------------------------------------------------//
bool MOAITexture::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAITexture::Load ( MOAIDataBuffer& data, u32 transform ) {

	this->Init ( data, transform );
	this->SetFilter ( GL_LINEAR, GL_NEAREST );
	this->SetWrap ( GL_REPEAT );
}

//----------------------------------------------------------------//
void MOAITexture::Load ( cc8* filename, u32 transform ) {

	this->mTexturePath = USFileSys::Expand ( filename );

	if ( !USFileSys::CheckFileExists ( this->mTexturePath )) return;
	this->Init ( this->mTexturePath, transform );
	this->SetFilter ( GL_LINEAR, GL_NEAREST );
	this->SetWrap ( GL_REPEAT );
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( USLuaState& state ) {
	
	state.SetField ( -1, "FILTER_POINT", ( u32 )GL_NEAREST );
	state.SetField ( -1, "FILTER_BILERP", ( u32 )GL_LINEAR );
	
	//state.SetField ( -1, "POW_TWO", ( u32 )USImageTransform::POW_TWO ); // TODO: still not sold we should offet this 'feature'
	state.SetField ( -1, "QUANTIZE", ( u32 )USImageTransform::QUANTIZE );
	state.SetField ( -1, "TRUECOLOR", ( u32 )USImageTransform::TRUECOLOR );
	state.SetField ( -1, "PREMULTIPLY_ALPHA", ( u32 )USImageTransform::PREMULTIPLY_ALPHA );
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "bind",				_bind },
		{ "getSize",			_getSize },
		{ "load",				_load },
		{ "release",			_release },
		{ "setFilter",			_setFilter },
		{ "setWrap",			_setWrap },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITexture::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	STLString path = state.GetField ( -1, "mPath", "" );
	
	if ( path.size ()) {
		USFilename filename;
		filename.Bless ( path.str ());
		this->Load ( filename.mBuffer, DEFAULT_TRANSFORM ); // TODO: serialization
	}
}

//----------------------------------------------------------------//
void MOAITexture::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );

	STLString path = USFileSys::GetRelativePath ( this->mTexturePath );
	state.SetField ( -1, "mPath", path.str ());
}

//----------------------------------------------------------------//
STLString MOAITexture::ToString () {

	STLString repr;

	PrettyPrint ( repr, "mWidth", GetWidth ());
	PrettyPrint ( repr, "mHeight", GetHeight ());

	return repr;
}
