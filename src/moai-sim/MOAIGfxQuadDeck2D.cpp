// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGfxQuadDeck2D.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getQuad
	@text	Get model space quad given a deck index. 
			Vertex order is clockwiese from upper left.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@out	number x0
	@out	number y0
	@out	number x1
	@out	number y1
	@out	number x2
	@out	number y2
	@out	number x3
	@out	number y3
*/
int MOAIGfxQuadDeck2D::_getQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 idx = state.GetValue < int >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {

		ZLQuad& quad = self->mQuads [ idx ].mModelQuad;
		
		state.Push ( quad.mV [ 0 ].mX );
		state.Push ( quad.mV [ 0 ].mY );
		state.Push ( quad.mV [ 1 ].mX );
		state.Push ( quad.mV [ 1 ].mY );
		state.Push ( quad.mV [ 2 ].mX );
		state.Push ( quad.mV [ 2 ].mY );
		state.Push ( quad.mV [ 3 ].mX );
		state.Push ( quad.mV [ 3 ].mY );
		
		return 8;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Set model space quad given a valid deck index and a rect.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIGfxQuadDeck2D::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( idx < self->mQuads.Size ()) {
		
		ZLRect rect = self->mQuads [ idx ].GetVtxBounds();
		
		lua_pushnumber ( state, rect.mXMin );
		lua_pushnumber ( state, rect.mYMin );
		lua_pushnumber ( state, rect.mXMax );
		lua_pushnumber ( state, rect.mYMax );
		return 4;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVQuad
	@text	Get UV model space quad given a valid deck index.
			Vertex order is clockwise from upper left.

	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@out	number u0
	@out	number v0
	@out	number u1
	@out	number v1
	@out	number u2
	@out	number v2
	@out	number u3
	@out	number v3
*/
int MOAIGfxQuadDeck2D::_getUVQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 idx = state.GetValue < int >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
		
		ZLQuad& quad = self->mQuads [ idx ].mUVQuad;
		
		state.Push ( quad.mV [ 0 ].mX );
		state.Push ( quad.mV [ 0 ].mY );
		state.Push ( quad.mV [ 1 ].mX );
		state.Push ( quad.mV [ 1 ].mY );
		state.Push ( quad.mV [ 2 ].mX );
		state.Push ( quad.mV [ 2 ].mY );
		state.Push ( quad.mV [ 3 ].mX );
		state.Push ( quad.mV [ 3 ].mY );
		
		return 8;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getUVRect
	@text	Get UV model space quad given a valid deck index.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx	Index of the quad.
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIGfxQuadDeck2D::_getUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 idx = state.GetValue < int >( 2, 1 ) - 1;
	if ( MOAILogMessages::CheckIndexPlusOne ( idx, self->mQuads.Size (), L )) {
		
		ZLRect rect = self->mQuads [ idx ].GetUVBounds ();
		
		state.Push ( rect.mXMin );
		state.Push ( rect.mYMin );
		state.Push ( rect.mXMax );
		state.Push ( rect.mYMax );
		
		return 4;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Set capacity of quad deck.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number nQuads
	@out	nil
*/
int MOAIGfxQuadDeck2D::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mQuads.Init ( total );
	self->mMaterialIDs.Init ( total );
	
	for ( u32 i = 0; i < total; ++i ) {
		MOAIQuadBrush& quad = self->mQuads [ i ];
		quad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
		quad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
		self->mMaterialIDs [ i ] = MOAIMaterialBatch::UNKNOWN;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaterialID
	@text	Specify an optional material ID for a quad.
	
	@in		MOAIGfxQuadDeck2D self
	@in		number idx
	@in		number id
	@out	nil
*/
int MOAIGfxQuadDeck2D::_setMaterialID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuadDeck2D, "UN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 id		= state.IsType ( 3, LUA_TNUMBER ) ? state.GetValue < u32 >( 3, 1 ) - 1 : MOAIMaterialBatch::UNKNOWN;
	
	self->mMaterialIDs [ idx ] = id;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setQuad
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
	
		ZLQuad quad;
		
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
/**	@lua	setRect
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
/**	@lua	setUVQuad
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
	
		ZLQuad quad;
		
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
/**	@lua	setUVRect
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
/**	@lua	transform
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
/**	@lua	transformUV
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
MOAIQuadBrush& MOAIGfxQuadDeck2D::AffirmQuad ( u32 idx ) {

	this->mQuads.Grow ( idx, 1 );
	return this->mQuads [ idx ];
}

//----------------------------------------------------------------//
ZLBox MOAIGfxQuadDeck2D::ComputeMaxBounds () {

	ZLRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	u32 size = this->mQuads.Size ();
	for ( u32 i = 0; i < size; ++i ) {
		rect.Grow ( this->mQuads [ i ].GetVtxBounds ());
	}
	
	ZLBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {

	u32 size = this->mQuads.Size ();
	if ( size ) {

		idx = idx - 1;
		u32 itemIdx = idx % size;

		materials.LoadGfxState ( this, this->mMaterialIDs [ itemIdx ], idx, MOAIShaderMgr::DECK2D_SHADER );

		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );
		
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		this->mQuads [ itemIdx ].Draw ( offset.mX, offset.mY, offset.mZ, scale.mX, scale.mY  );
	}
}

//----------------------------------------------------------------//
ZLBox MOAIGfxQuadDeck2D::GetItemBounds ( u32 idx ) {
	
	ZLBox bounds;
	
	u32 size = this->mQuads.Size ();
	if ( size ) {
	
		idx = ( idx - 1 ) % size;
	
		ZLRect rect = this->mQuads [ idx ].GetVtxBounds ();
		bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
		return bounds;
	}
	
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
bool MOAIGfxQuadDeck2D::Inside ( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad ) {
	UNUSED ( pad );

	u32 size = this->mQuads.Size ();
	if ( size ) {
		idx = ( idx - 1 ) % size;
		const MOAIQuadBrush& quadBrush = this->mQuads [ idx ];
		return materials.TestHit ( this, idx, granularity, quadBrush.mModelQuad, quadBrush.mUVQuad, vec.mX, vec.mY );
	}
	return false;
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::MOAIGfxQuadDeck2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStandardDeck )
	RTTI_END
	
	//this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::~MOAIGfxQuadDeck2D () {
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStandardDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStandardDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getQuad",			_getQuad },
		{ "getRect",			_getRect },
		{ "getUVQuad",			_getUVQuad },
		{ "getUVRect",			_getUVRect },
		{ "reserve",			_reserve },
		{ "setMaterialID",		_setMaterialID },
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
void MOAIGfxQuadDeck2D::Transform ( const ZLAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformVerts ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::TransformUV ( const ZLAffine3D& mtx ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformUVs ( mtx );
	}
}