// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAITileDeck2D.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set the model space dimensions of a single tile. When grid drawing, this
			should be a unit rect centered at the origin for tiles that fit each grid
			cell. Growing or shrinking the rect will cause tiles to overlap or leave
			gaps between them.
	
	@in		MOAITileDeck2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAITileDeck2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Controls how the texture is subdivided into tiles. Default
			behavior is to subdivide the texture into N by M tiles,
			but is tile dimensions are provided (in UV space) then the resulting
			tile set will be N * tileWidth by M * tileHeight in UV
			space. This means the tile set does not have to fill all of the
			texture. The upper left hand corner of the tile set will always be
			at UV 0, 0.
	
	@in		MOAITileDeck2D self
	@in		number width			Width of the tile deck in tiles.
	@in		number height			Height of the tile deck in tiles.
	@opt	number cellWidth		Width of individual tile in UV space. Defaults to 1 / width.
	@opt	number cellHeight		Height of individual tile in UV space. Defaults to 1 / height.
	@opt	number xOff				X offset of the tile from the cell.
	@opt	number yOff				Y offset of the tile from the cell.
	@opt	number tileWidth		Default value is cellWidth.
	@opt	number tileHeight		Default value is cellHeight.
	@out	nil
*/
int	MOAITileDeck2D::_setSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UNN" )
	
	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float cellWidth		= state.GetValue < float >( 4, 1.0f / ( float )width );
	float cellHeight	= state.GetValue < float >( 5, 1.0f / ( float )height );
	
	float xOff			= state.GetValue < float >( 6, 0.0f );
	float yOff			= state.GetValue < float >( 7, 0.0f );
	
	float tileWidth		= state.GetValue < float >( 8, cellWidth );
	float tileHeight	= state.GetValue < float >( 9, cellHeight );
	
	self->SetWidth ( width );
	self->SetHeight ( height );
	
	self->SetCellWidth ( cellWidth );
	self->SetCellHeight ( cellHeight );
	
	self->SetXOff ( xOff );
	self->SetYOff ( yOff );
	
	self->SetTileWidth ( tileWidth );
	self->SetTileHeight ( tileHeight );
	
	return 0;
}

//================================================================//
// MOAITileDeck2D
//================================================================//

//----------------------------------------------------------------//
void MOAITileDeck2D::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );
	
	idx = idx - 1;
	
	MOAICellCoord coord = this->GetCellCoord ( idx );
	USRect uvRect = this->GetTileRect ( coord );
	uvRect.FlipY ();
	
	MOAIQuadBrush quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( uvRect );
	quad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAITileDeck2D::GetRect () {
	return this->mRect;
}

//----------------------------------------------------------------//
USRect MOAITileDeck2D::GetRect ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );

	return this->mRect;
}

//----------------------------------------------------------------//
MOAITileDeck2D::MOAITileDeck2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck2D )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	this->mRect.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
}

//----------------------------------------------------------------//
MOAITileDeck2D::~MOAITileDeck2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setRect",			_setRect },
		{ "setSize",			_setSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIGridSpace::SerializeIn ( state, serializer );
	
	this->mTexture.Set ( *this, serializer.MemberIDToObject < MOAITextureBase >( state.GetField < uintptr >( -1, "mTexture", 0 )));
}

//----------------------------------------------------------------//
void MOAITileDeck2D::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIGridSpace::SerializeOut ( state, serializer );
	
	state.SetField ( -1, "mTexture", serializer.AffirmMemberID ( this->mTexture ));
}
