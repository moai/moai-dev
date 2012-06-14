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
/**	@name	setQuad
	@text	Set model space quad. Vertex order is clockwise from
			upper left (xMin, yMax)
	
	@in		MOAITileDeck2D self
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAITileDeck2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UNNNNNNNN" )

	USQuad quad;
	
	quad.mV [ 0 ].mX = state.GetValue < float >( 2, 0.0f );
	quad.mV [ 0 ].mY = state.GetValue < float >( 3, 0.0f );
	quad.mV [ 1 ].mX = state.GetValue < float >( 4, 0.0f );
	quad.mV [ 1 ].mY = state.GetValue < float >( 5, 0.0f );
	quad.mV [ 2 ].mX = state.GetValue < float >( 6, 0.0f );
	quad.mV [ 2 ].mY = state.GetValue < float >( 7, 0.0f );
	quad.mV [ 3 ].mX = state.GetValue < float >( 8, 0.0f );
	quad.mV [ 3 ].mY = state.GetValue < float >( 9, 0.0f );

	self->mQuad.SetVerts ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
	self->SetBoundsDirty ();

	return 0;
}

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
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mQuad.SetVerts ( x0, y0, x1, y1 );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVQuad
	@text	Set the UV space dimensions of the quad. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAITileDeck2D self
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAITileDeck2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UNNNNNNNN" )

	USQuad quad;
	
	quad.mV [ 0 ].mX = state.GetValue < float >( 2, 0.0f );
	quad.mV [ 0 ].mY = state.GetValue < float >( 3, 0.0f );
	quad.mV [ 1 ].mX = state.GetValue < float >( 4, 0.0f );
	quad.mV [ 1 ].mY = state.GetValue < float >( 5, 0.0f );
	quad.mV [ 2 ].mX = state.GetValue < float >( 6, 0.0f );
	quad.mV [ 2 ].mY = state.GetValue < float >( 7, 0.0f );
	quad.mV [ 3 ].mX = state.GetValue < float >( 8, 0.0f );
	quad.mV [ 3 ].mY = state.GetValue < float >( 9, 0.0f );
	
	self->mQuad.SetUVs ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVRect
	@text	Set the UV space dimensions of the quad.
	
	@in		MOAITileDeck2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAITileDeck2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UNNNN" )
	
	float u0	= state.GetValue < float >( 2, 0.0f );
	float v0	= state.GetValue < float >( 3, 0.0f );
	float u1	= state.GetValue < float >( 4, 0.0f );
	float v1	= state.GetValue < float >( 5, 0.0f );

	self->mQuad.SetUVs ( u0, v0, u1, v1 );

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
	@opt	number xOff				X offset of the tile from the cell. Defaults to 0.
	@opt	number yOff				Y offset of the tile from the cell. Defaults to 0.
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

//----------------------------------------------------------------//
/**	@name	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAITileDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAITileDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UU" )
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->Transform ( transform->GetLocalToWorldMtx ());
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	transformUV
	@text	Apply the given MOAITransform to all the uv coordinates in the deck.
	
	@in		MOAITileDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAITileDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UU" )
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAITileDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAITileDeck2D::ComputeMaxBounds () {
	return this->GetItemBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zScl );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	
	idx = idx - 1;
	
	MOAICellCoord coord = this->GetCellCoord ( idx );
	USRect uvRect = this->GetTileRect ( coord );
	
	float uScale = ( uvRect.mXMax - uvRect.mXMin );
	float vScale = -( uvRect.mYMax - uvRect.mYMin );
	
	float uOff = uvRect.mXMin + ( 0.5f * uScale );
	float vOff = uvRect.mYMin - ( 0.5f * vScale );
	
	this->mQuad.Draw ( xOff, yOff, zOff, xScl, yScl, uOff, vOff, uScale, vScale );
}

//----------------------------------------------------------------//
USBox MOAITileDeck2D::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );
	USBox bounds;
	USRect rect = this->mQuad.GetVtxBounds ();
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
MOAITileDeck2D::MOAITileDeck2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIGridSpace )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	this->mQuad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mQuad.SetUVs ( -0.5f, -0.5f, 0.5f, 0.5f );
}

//----------------------------------------------------------------//
MOAITileDeck2D::~MOAITileDeck2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	MOAIGridSpace::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIGridSpace::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ "setSize",			_setSize },
		{ "transform",			_transform },
		{ "transformUV",		_transformUV },
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

//----------------------------------------------------------------//
void MOAITileDeck2D::Transform ( const USAffine3D& mtx ) {

	this->mQuad.TransformVerts ( mtx );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::TransformUV ( const USAffine3D& mtx ) {

	this->mQuad.TransformUVs ( mtx );
}