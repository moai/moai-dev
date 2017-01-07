// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxQuad2D.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setQuad
	@text	Set model space quad. Vertex order is clockwise from
			upper left (xMin, yMax)
	
	@in		MOAIGfxQuad2D self
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
int MOAIGfxQuad2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNNNNNN" )

	self->mQuad.mModelQuad = state.GetQuad ( 2 );
	self->SetBoundsDirty ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set the model space dimensions of the quad.
	
	@in		MOAIGfxQuad2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuad2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
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
	
	@in		MOAIGfxQuad2D self
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
int MOAIGfxQuad2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNNNNNN" )

	self->mQuad.mUVQuad = state.GetQuad ( 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUVRect
	@text	Set the UV space dimensions of the quad.
	
	@in		MOAIGfxQuad2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuad2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
	float u0	= state.GetValue < float >( 2, 0.0f );
	float v0	= state.GetValue < float >( 3, 0.0f );
	float u1	= state.GetValue < float >( 4, 0.0f );
	float v1	= state.GetValue < float >( 5, 0.0f );

	self->mQuad.SetUVs ( u0, v0, u1, v1 );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAIGfxQuad2D self
	@in		MOAITransformBase transform
	@out	nil
*/
int MOAIGfxQuad2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UU" )
	
	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
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
	
	@in		MOAIGfxQuad2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAIGfxQuad2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UU" )
	
	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAIGfxQuad2D
//================================================================//

//----------------------------------------------------------------//
MOAIGfxQuad2D::MOAIGfxQuad2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsDeckBase )
	RTTI_END
	
	// set up rects to draw a unit tile centered at the origin
	this->mQuad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mQuad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::~MOAIGfxQuad2D () {
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsDeckBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGraphicsDeckBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ "transform",			_transform },
		{ "transformUV",		_transformUV },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIGraphicsDeckBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIGraphicsDeckBase::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::Transform ( const ZLAffine3D& mtx ) {

	this->mQuad.TransformVerts ( mtx );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::TransformUV ( const ZLAffine3D& mtx ) {

	this->mQuad.TransformUVs ( mtx );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIGfxQuad2D::MOAIDeck_ComputeMaxBounds () {

	return this->GetBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::MOAIDeck_Draw ( u32 idx ) {

	if ( !this->GetMaterialBatch ()->LoadGfxState ( 0, idx, MOAIShaderMgr::DECK2D_SHADER )) return;
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIQuadBrush::BindVertexFormat ();
	
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX ));
	gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_MTX ));
	
	this->mQuad.Draw ();
}

//----------------------------------------------------------------//
ZLBounds MOAIGfxQuad2D::MOAIDeck_GetBounds ( u32 idx ) {
	UNUSED ( idx );
	ZLBounds bounds;
	ZLRect rect = this->mQuad.GetVtxBounds ();
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );
	bounds.mStatus = rect.Area () == 0.0f ? ZLBounds::ZL_BOUNDS_EMPTY : ZLBounds::ZL_BOUNDS_OK;
	return bounds;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIGfxQuad2D::MOAIDeck_GetCollisionShape ( u32 idx ) {
}

//----------------------------------------------------------------//
bool MOAIGfxQuad2D::MOAIDeck_Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	return this->GetMaterialBatch ()->TestHit ( this, idx, granularity, this->mQuad.mModelQuad, this->mQuad.mUVQuad, vec.mX, vec.mY );
}

//----------------------------------------------------------------//
bool MOAIGfxQuad2D::MOAIDeck_Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	// TODO: handle granularity

	return (( vec.mZ == 0.0f ) && this->MOAIDeck_Overlap ( idx, vec, granularity, result ));
}
