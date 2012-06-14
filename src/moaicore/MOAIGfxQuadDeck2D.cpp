// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGfxQuadDeck2D.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Set capacity of quad deck.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number nQuads
	@out	nil
*/
int MOAIGfxQuadDeck2D::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mQuads.Init ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		MOAIQuadBrush& quad = self->mQuads [ i ];
		quad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
		quad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setQuad
	@text	Set model space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
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
int MOAIGfxQuadDeck2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UNNNNNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		USQuad quad;
		
		quad.mV [ 0 ].mX = state.GetValue < float >( 3, 0.0f );
		quad.mV [ 0 ].mY = state.GetValue < float >( 4, 0.0f );
		quad.mV [ 1 ].mX = state.GetValue < float >( 5, 0.0f );
		quad.mV [ 1 ].mY = state.GetValue < float >( 6, 0.0f );
		quad.mV [ 2 ].mX = state.GetValue < float >( 7, 0.0f );
		quad.mV [ 2 ].mY = state.GetValue < float >( 8, 0.0f );
		quad.mV [ 3 ].mX = state.GetValue < float >( 9, 0.0f );
		quad.mV [ 3 ].mY = state.GetValue < float >( 10, 0.0f );

		self->mQuads [ idx ].SetVerts ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
		self->SetBoundsDirty ();
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuadDeck2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		float x0	= state.GetValue < float >( 3, 0.0f );
		float y0	= state.GetValue < float >( 4, 0.0f );
		float x1	= state.GetValue < float >( 5, 0.0f );
		float y1	= state.GetValue < float >( 6, 0.0f );
		
		if ( idx < self->mQuads.Size ()) {
			self->mQuads [ idx ].SetVerts ( x0, y0, x1, y1 );
			self->SetBoundsDirty ();
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVQuad
	@text	Set UV space quad given a valid deck index. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
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
int MOAIGfxQuadDeck2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UNNNNNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		USQuad quad;
		
		quad.mV [ 0 ].mX = state.GetValue < float >( 3, 0.0f );
		quad.mV [ 0 ].mY = state.GetValue < float >( 4, 0.0f );
		quad.mV [ 1 ].mX = state.GetValue < float >( 5, 0.0f );
		quad.mV [ 1 ].mY = state.GetValue < float >( 6, 0.0f );
		quad.mV [ 2 ].mX = state.GetValue < float >( 7, 0.0f );
		quad.mV [ 2 ].mY = state.GetValue < float >( 8, 0.0f );
		quad.mV [ 3 ].mX = state.GetValue < float >( 9, 0.0f );
		quad.mV [ 3 ].mY = state.GetValue < float >( 10, 0.0f );

		self->mQuads [ idx ].SetUVs ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVRect
	@text	Set UV space quad given a valid deck index and a rect.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuadDeck2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UNNNNN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
	
		float u0	= state.GetValue < float >( 3, 0.0f );
		float v0	= state.GetValue < float >( 4, 0.0f );
		float u1	= state.GetValue < float >( 5, 0.0f );
		float v1	= state.GetValue < float >( 6, 0.0f );

		if ( idx < self->mQuads.Size ()) {
			self->mQuads [ idx ].SetUVs ( u0, v0, u1, v1 );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	transform
	@text	Apply the given MOAITransform to all the vertices in the deck.
	
	@in		MOAIGfxQuadDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAIGfxQuadDeck2D::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UU" )
	
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
	
	@in		MOAIGfxQuadDeck2D self
	@in		MOAITransform transform
	@out	nil
*/
int MOAIGfxQuadDeck2D::_transformUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UU" )
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUV ( transform->GetLocalToWorldMtx ());
	}
	return 0;
}

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAIGfxQuadDeck2D::ComputeMaxBounds () {

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	u32 size = this->mQuads.Size ();
	for ( u32 i = 0; i < size; ++i ) {
		rect.Grow ( this->mQuads [ i ].GetVtxBounds ());
	}
	
	USBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zScl );

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );

	u32 size = this->mQuads.Size ();
	if ( size ) {
		idx = ( idx - 1 ) % size;
		this->mQuads [ idx ].Draw ( xOff, yOff, zOff, xScl, yScl );
	}
}

//----------------------------------------------------------------//
USBox MOAIGfxQuadDeck2D::GetItemBounds ( u32 idx ) {
	
	USBox bounds;
	
	u32 size = this->mQuads.Size ();
	if ( size ) {
	
		idx = ( idx - 1 ) % size;
	
		USRect rect = this->mQuads [ idx ].GetVtxBounds ();
		bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
		return bounds;
	}
	
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::MOAIGfxQuadDeck2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::~MOAIGfxQuadDeck2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
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
void MOAIGfxQuadDeck2D::Transform ( const USAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformVerts ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::TransformUV ( const USAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformUVs ( mtx );
	}
}