//
//  MOAIMultiTextureDeck2D.cpp
//  libmoai
//
//  Created by Isaac Barrett on 11/7/13.
//
//

#include "pch.h"
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIMultiTextureDeck2D.h>
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
	@text	Set capacity of multi-texture deck.
 
	@in		MOAIMultiTextureDeck2D self
	@in		number nTextures
	@out	nil
 */
int MOAIMultiTextureDeck2D::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMultiTextureDeck2D, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mQuads.Init ( total );
	self->mTextures.Init( total );
	self->mTextures.Fill( 0 );
	
	for ( u32 i = 0; i < total; ++i ) {
		MOAIQuadBrush& quad = self->mQuads [ i ];
		quad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
		quad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/** @name	setQuad
	@text	
 
	@in		MOAIMultiTextureDeck2D self
	@in		number index
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

int MOAIMultiTextureDeck2D::_setQuad ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIMultiTextureDeck2D, "UNNNNNNNNN" )
	
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

int MOAIMultiTextureDeck2D::_setRect ( lua_State *L ) {
	return 0;
}

//----------------------------------------------------------------//

int MOAIMultiTextureDeck2D::_setTexture ( lua_State *L ) {
	return 0;
}


//----------------------------------------------------------------//

int MOAIMultiTextureDeck2D::_setUVQuad ( lua_State *L ) {
	return 0;
}

//----------------------------------------------------------------//

int MOAIMultiTextureDeck2D::_setUVRect ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//



//================================================================//
// MOAIMultiTextureDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAIMultiTextureDeck2D::ComputeMaxBounds() {
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
void MOAIMultiTextureDeck2D::DrawIndex(u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl){
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
USBox MOAIMultiTextureDeck2D::GetItemBounds ( u32 idx ) {
	
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
MOAIMultiTextureDeck2D::MOAIMultiTextureDeck2D () {
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIMultiTextureDeck2D::~MOAIMultiTextureDeck2D(){
	
	this->mTexture.Set ( *this, 0 );
	
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::RegisterLuaClass( MOAILuaState &state ){
	
	MOAIDeck::RegisterLuaClass( state );
	
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::RegisterLuaFuncs(MOAILuaState &state){
	MOAIDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setTexture",			_setTexture },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ "transform",			_transform },
		{ "transformUV",		_transformUV },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::Transform ( const USAffine3D& mtx ) {
	
	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformVerts ( mtx );
	}
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::TransformUV ( const USAffine3D& mtx ) {
	
	u32 total = this->mQuads.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		this->mQuads [ i ].TransformUVs ( mtx );
	}
}
