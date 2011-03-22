// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAITileLibrary.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() setFlip ( self, xFlip, yFlip )</tt>\n
\n
	Set global flip flags for the tile library.
	@param self (in)
	@param xFlip (in)
	@param yFlip (in)
*/
int MOAITileLibrary::_setFlip ( lua_State* L ) {
	LUA_SETUP ( MOAITileLibrary, "UBB" )
	
	bool xFlip = state.GetValue < bool >( 2, false );
	bool yFlip = state.GetValue < bool >( 3, false );

	// clear flip flags
	self->mFlags = self->mFlags & ~USTile::FLIP_MASK;

	self->mFlags |= xFlip ? USTile::XFLIP : 0;
	self->mFlags |= yFlip ? USTile::YFLIP : 0;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRect ( self, xMin, yMin, xMax, yMax )</tt>\n
\n
	Convenience method. Sets the default dimentions for non-grid primitives displaying this texture.
	@param self (in)
	@param xMin (in)
	@param yMin (in)
	@param xMax (in)
	@param yMax (in)
*/
int MOAITileLibrary::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAITileLibrary, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setSize ( self, xTiles, yTiles )</tt>\n
	\n
	Subdivides an image into tiles for individual display.
	@param self (in)
	@param xTiles (in)
	@param yTiles (in)
*/
int	MOAITileLibrary::_setSize ( lua_State* L ) {
	LUA_SETUP ( MOAITileLibrary, "UNN" )
	
	u32 width = state.GetValue < u32 >( 2, 0 );
	u32 height = state.GetValue < u32 >( 3, 0 );
	float tileWidth = state.GetValue < float >( 4, 1.0f / ( float )width );
	float tileHeight = state.GetValue < float >( 5, 1.0f / ( float )height );
	
	self->SetWidth ( width );
	self->SetHeight ( height );
	self->SetTileWidth ( tileWidth );
	self->SetTileHeight ( tileHeight );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setTexture ( self, texture )</tt>\n
	\n
	Assigns the texture to be used.
	@param self (in)
	@param texture (in)
*/
int	MOAITileLibrary::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAITileLibrary, "U" )
	
	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAITileLibrary
//================================================================//

//----------------------------------------------------------------//
bool MOAITileLibrary::Bind () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	if ( !drawBuffer.BindTexture ( this->mTexture )) return false;
	USGLQuad::BindVertexFormat ( drawBuffer );

	return true;
}

//----------------------------------------------------------------//
void MOAITileLibrary::Draw ( MOAIDrawingMtx2D& transform, u32 idx ) {

	if ( idx & USTile::HIDDEN ) return;
	idx = USTile::ToggleYFlip ( idx );
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());
	
	this->DrawTile ( idx, this->mRect );
}

//----------------------------------------------------------------//
void MOAITileLibrary::Draw ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.SetVtxTransform ( transform.GetLocalToWorldMtx ());

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			u32 tile = grid.GetTile ( x, y );
			if ( tile & USTile::HIDDEN ) continue;
			
			USRect tileRect = grid.GetTileRect ( x, y );
			this->DrawTile ( tile, tileRect );
		}
	}
}

//----------------------------------------------------------------//
void MOAITileLibrary::DrawTile ( u32 tile, USRect rect ) {
	
	USGLQuad glQuad;
	glQuad.SetVerts ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
	
	USRect uvRect = this->GetTileRect ( tile & USTile::CODE_MASK );
	
	u32 flags = tile ^ this->mFlags;
	
	if ( flags & USTile::XFLIP ) {
		uvRect.FlipX ();
	}

	if ( flags & USTile::YFLIP ) {
		uvRect.FlipY ();
	}
	
	glQuad.SetUVs ( uvRect );
	glQuad.Draw ();
}

//----------------------------------------------------------------//
USRect MOAITileLibrary::GetBounds ( u32 idx ) {
	UNUSED ( idx );

	return this->mRect;
}

//----------------------------------------------------------------//
MOAITileLibrary::MOAITileLibrary () :
	mFlags ( 0 ) {
	
	RTTI_SINGLE ( MOAIContentLibrary2D )
	this->SetContentMask ( CAN_DRAW );
}

//----------------------------------------------------------------//
MOAITileLibrary::~MOAITileLibrary () {
}

//----------------------------------------------------------------//
void MOAITileLibrary::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIContentLibrary2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITileLibrary::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIContentLibrary2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "setFlip",			_setFlip },
		{ "setRect",			_setRect },
		{ "setSize",			_setSize },
		{ "setTexture",			_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITileLibrary::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {

	USGridSpace::SerializeIn ( state );
	
	this->mTexture = serializer.GetRefField < MOAITexture >( state, -1, "mTexture" );
}

//----------------------------------------------------------------//
void MOAITileLibrary::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {

	USGridSpace::SerializeOut ( state );
	
	serializer.SetRefField ( state, -1, "mTexture", this->mTexture );
}

//----------------------------------------------------------------//
STLString MOAITileLibrary::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mTexture )

	return repr;
}
