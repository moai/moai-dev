// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIData.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase2D.h>

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
	@param height (out)
*/
int MOAITexture::_getTextureSize ( lua_State* L ) {
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
	@param data (in) A MOAIData object containing image data.  Must be one of the filetypes above.
*/
int MOAITexture::_load ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "U" )

	MOAIData* data = state.GetLuaData < MOAIData >( 2 );

	if ( data ) {

		self->Load ( *data );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename = lua_tostring ( state, 2 );
		self->Load ( filename );
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
/**	@brief <tt>setRect ( self, xMin, yMin, xMax, yMax )</tt>\n
\n
	Convenience method. Sets the default dimentions for primitives displaying this texture.
	@param self (in)
	@param xMin (in)
	@param yMin (in)
	@param xMax (in)
	@param yMax (in)
*/
int MOAITexture::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITexture::_setUVRect ( lua_State* L ) {
	LUA_SETUP ( MOAITexture, "UNNNN" )
	
	self->mUVRect = state.GetRect < float >( 2 );

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

	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		
		texture = state.GetLuaData < MOAITexture >( idx );
		MOAIData* data = state.GetLuaData < MOAIData >( idx );
		
		if ( data ) {
			texture = new MOAITexture ();
			texture->Load ( *data );
		}
	}
	else if ( state.IsType ( idx, LUA_TSTRING )) {
		
		cc8* filename = lua_tostring ( state, idx );
		texture = new MOAITexture ();
		texture->Load ( filename );
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
void MOAITexture::Draw ( MOAIDrawingMtx2D& transform, u32 idx ) {
	UNUSED ( idx );

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();
	drawbuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());
	
	USGLQuad quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( this->mUVRect );
	quad.Draw ();
}

//----------------------------------------------------------------//
void MOAITexture::Draw ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();
	drawbuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());

	USGLQuad quad;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			u32 tile = grid.GetTile ( x, y );
			if ( tile & USTile::HIDDEN ) continue;
			
			USRect tileRect = grid.GetTileRect ( x, y );
			USRect uvRect = this->mUVRect;
			
			if ( tile & USTile::XFLIP ) {
				uvRect.FlipX ();
			}

			if ( tile & USTile::YFLIP ) {
				uvRect.FlipY ();
			}
			
			quad.SetVerts ( tileRect );
			quad.SetUVs ( uvRect );
			quad.Draw ();
		}
	}
}

//----------------------------------------------------------------//
USRect MOAITexture::GetBounds ( u32 idx ) {
	UNUSED ( idx );
	
	return this->mRect;
}

//----------------------------------------------------------------//
void MOAITexture::Load ( MOAIData& data ) {

	this->Init ( data, USImageTransform::TRUECOLOR | USImageTransform::PREMULTIPLY_ALPHA );
	this->SetFilter ( GL_LINEAR, GL_NEAREST );
	this->SetWrap ( GL_REPEAT );
}

//----------------------------------------------------------------//
void MOAITexture::Load ( cc8* filename ) {

	this->mTexturePath = USFileSys::Expand ( filename );

	if ( !USFileSys::CheckFileExists ( this->mTexturePath )) return;
	this->Init ( this->mTexturePath, USImageTransform::TRUECOLOR | USImageTransform::PREMULTIPLY_ALPHA );
	this->SetFilter ( GL_LINEAR, GL_NEAREST );
	this->SetWrap ( GL_REPEAT );
}

//----------------------------------------------------------------//
MOAITexture::MOAITexture () {

	RTTI_SINGLE ( MOAIContentLibrary2D )
	this->SetContentMask ( CAN_DRAW );
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mUVRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAITexture::~MOAITexture () {
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIContentLibrary2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "FILTER_POINT", ( u32 )GL_NEAREST );
	state.SetField ( -1, "FILTER_BILERP", ( u32 )GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAITexture::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIContentLibrary2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "bind",				_bind },
		{ "getTextureSize",		_getTextureSize },
		{ "load",				_load },
		{ "release",			_release },
		{ "setFilter",			_setFilter },
		{ "setRect",			_setRect },
		{ "setUVRect",			_setUVRect },
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
		this->Load ( filename.mBuffer );
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
	PrettyPrint ( repr, "U", GetU ());
	PrettyPrint ( repr, "V", GetV ());

	return repr;
}
