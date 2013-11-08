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
/**	@name	setTexture
	@text	Sets or clears a texture for the given index.
 
	@in		MOAITextureBase self
	@in		number index
	@opt	MOAITextureBase texture		Default value is nil.
	@out	nil
 */
int MOAIMultiTextureDeck2D::_setTexture ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAIMultiTextureDeck2D, "UN");
	
	u32 idx						= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAITextureBase* texture	= state.GetLuaObject < MOAITextureBase >( 3, true );
	
	self->SetTexture ( idx, texture );
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	transformUVAtIndex
	@text	Apply the given MOAITransform to the UV coordinates in the specified index.
 
	@in		MOAIMultiTextureDeck2d	self
	@in		number index
	@in		MOAITransform transform
	@out	nil
 */
int MOAIMultiTextureDeck2D::_transformUVAtIndex ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAIMultiTextureDeck2D, "UIU");
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformUVAtIndex(index, transform->GetLocalToWorldMtx() );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	transformVertsAtIndex
	@text	Apply the given MOAITransform to the vertices in the specified index.
 
	@in		MOAIMultiTextureDeck2d	self
	@in		number index
	@in		MOAITransform transform
	@out	nil
 */
int MOAIMultiTextureDeck2D::_transformVertsAtIndex ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAIMultiTextureDeck2D, "UIU");
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAITransform* transform = state.GetLuaObject < MOAITransform >( 2, true );
	if ( transform ) {
		transform->ForceUpdate ();
		self->TransformVertsAtIndex(index, transform->GetLocalToWorldMtx() );
		self->SetBoundsDirty();
	}
	return 0;
}
//================================================================//
// MOAIMultiTextureDeck2D
//================================================================//

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::DrawIndex(u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl){
	UNUSED ( idx );
	UNUSED ( zScl );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	
	// draw all quads with appropriate texture
	u32 size = this->mQuads.Size ();
	if ( size ) {
		idx = ( idx - 1 ) % size;
		this->mQuads [ idx ].Draw ( xOff, yOff, zOff, xScl, yScl );
	}
}

//----------------------------------------------------------------//
MOAIMultiTextureDeck2D::MOAIMultiTextureDeck2D () {
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxQuadDeck2D )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIMultiTextureDeck2D::~MOAIMultiTextureDeck2D(){
	
	this->mTexture.Set ( *this, 0 );
	
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::RegisterLuaClass( MOAILuaState &state ){
	
	MOAIGfxQuadDeck2D::RegisterLuaClass( state );
	
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::RegisterLuaFuncs(MOAILuaState &state){
	MOAIDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",				_reserve },
		{ "setTexture",				_setTexture },
		{ "transformVertsAtIndex",	_transformVertsAtIndex },
		{ "transformUVAtIndex",		_transformUVAtIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::SetTexture(u32 idx, MOAITextureBase *texture){
	if ( idx >= this->mTextures.Size ()) return;
	if ( this->mTextures [ idx ] == texture ) return;
	
	this->LuaRetain ( texture );
	this->LuaRelease ( this->mTextures [ idx ]);
	this->mTextures [ idx ] = texture;
	
	this->mTexture.Set(*this, texture);
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::TransformUVAtIndex(u32 idx, const USAffine3D &mtx){
	if ( idx >= this->mQuads.Size ()) return;
	
	this->mQuads[ idx ].TransformUVs ( mtx );
}

//----------------------------------------------------------------//
void MOAIMultiTextureDeck2D::TransformVertsAtIndex(u32 idx, const USAffine3D &mtx){
	if ( idx >= this->mQuads.Size ()) return;
	
	this->mQuads [ idx ].TransformVerts( mtx );
}

