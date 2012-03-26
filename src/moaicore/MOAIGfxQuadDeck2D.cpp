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

//================================================================//
// MOAIGfxQuadDeck2D
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );

	u32 size = this->mQuads.Size ();
	if ( size ) {
		idx = ( idx - 1 ) % size;		
		this->mQuads [ idx ].Draw ( xOff, yOff, xScale, yScale );
	}
}

//----------------------------------------------------------------//
USRect MOAIGfxQuadDeck2D::GetRect ( u32 idx, MOAIDeckRemapper* remapper ) {
	
	u32 size = this->mQuads.Size ();
	if ( size ) {

		idx = remapper ? remapper->Remap ( idx ) : idx;
		idx = ( idx - 1 ) % size;
	
		MOAIQuadBrush& quad = this->mQuads [ idx ];
		return quad.GetVtxBounds ();
	}
	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return rect;
}

USRect MOAIGfxQuadDeck2D::GetRect ( ) {

	u32 size = this->mQuads.Size ();
	USRect totalRect;
	totalRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	for ( u32 i = 0; i < size; ++i ) {
		MOAIQuadBrush& quad = this->mQuads [ i ];
		USRect rect = quad.GetVtxBounds ();

		totalRect.Grow ( rect );
	}
	return totalRect;
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::MOAIGfxQuadDeck2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck2D )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIGfxQuadDeck2D::~MOAIGfxQuadDeck2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::ScaleScreenCoords ( float xScale, float yScale ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].ScaleVerts ( xScale, yScale );
	}
}

//----------------------------------------------------------------//
void MOAIGfxQuadDeck2D::ScaleUVCoords ( float xScale, float yScale ) {

	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].ScaleUVs ( xScale, yScale );
	}
}

