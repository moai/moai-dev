// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractChildTransform.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAITileDeck2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setQuad
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

	ZLQuad quad;
	
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
/**	@lua	setRect
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
/**	@lua	setUVQuad
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

	ZLQuad quad;
	
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
/**	@lua	setUVRect
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
/**	@lua	setSize
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
/**	@lua	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAITileDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAITileDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UU" )
	
	MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->Transform ( transform->GetLocalToWorldMtx ());
		self->SetBoundsDirty ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transformUV
	@text	Apply the given MOAITransform to all the uv coordinates in the deck.
	
	@in		MOAITileDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAITileDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITileDeck2D, "UU" )
	
	MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
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
MOAITileDeck2D::MOAITileDeck2D () {
	
	RTTI_BEGIN ( MOAITileDeck2D )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAITileDeck2D >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAITileDeck2D >)
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIHasGfxComposerBatch )
		RTTI_EXTEND ( MOAIGridSpace )
	RTTI_END
	
	this->mQuad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mQuad.SetUVs ( -0.5f, -0.5f, 0.5f, 0.5f );
}

//----------------------------------------------------------------//
MOAITileDeck2D::~MOAITileDeck2D () {
}

//----------------------------------------------------------------//
void MOAITileDeck2D::Transform ( const ZLAffine3D& mtx ) {

	this->mQuad.TransformVerts ( mtx );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::TransformUV ( const ZLAffine3D& mtx ) {

	this->mQuad.TransformUVs ( mtx );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITileDeck2D::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITileDeck2D::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
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
void MOAITileDeck2D::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;

	//this->mTexture.Set ( *this, serializer.MemberIDToObject < MOAITexture >( state.GetFieldValue < MOAISerializerBase::ObjID >( -1, "mTexture", 0 )));
}

//----------------------------------------------------------------//
void MOAITileDeck2D::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;

	//state.SetField ( -1, "mTexture", serializer.AffirmMemberID ( this->mTexture ));
}

//----------------------------------------------------------------//
ZLBounds MOAITileDeck2D::MOAIDeck_ComputeMaxAABB () {

	return this->MOAIDeck::GetBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAITileDeck2D::MOAIDeck_Draw ( ZLIndex idx ) {
	UNUSED ( idx );
	
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	MOAIQuadBrush::BindVertexFormat ();
//	
//	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
//	gfxMgr.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );
//	
//	MOAICellCoord coord = this->GetCellCoord ( idx );
//	ZLRect uvRect = this->GetTileRect ( coord );
//	
//	float uScale = ( uvRect.mXMax - uvRect.mXMin );
//	float vScale = -( uvRect.mYMax - uvRect.mYMin );
//	
//	float uOff = uvRect.mXMin + ( 0.5f * uScale );
//	float vOff = uvRect.mYMin - ( 0.5f * vScale );
//	
//	MOAIMaterialMgr& materialMgr = MOAIMaterialMgr::Get ();
//	materialMgr.Push ( this->GetMaterial ( idx ));
//	materialMgr.SetShader ( MOAIShaderPresetEnum::DECK2D_SHADER );
//	materialMgr.LoadGfxState ();
//	
//	this->mQuad.Draw ( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, uOff, vOff, uScale, vScale );
//	
//	materialMgr.Pop ();
}

//----------------------------------------------------------------//
ZLBounds MOAITileDeck2D::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );
	
	return ZLBounds ( this->mQuad.GetVtxBounds ());
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAITileDeck2D::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
bool MOAITileDeck2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	return false;
}

//----------------------------------------------------------------//
bool MOAITileDeck2D::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	//return this->TestHit ( materials, idx, granularity, this->mQuad.mModelQuad, this->mQuad.mUVQuad, vec.mX, vec.mY );
	return false;
}
