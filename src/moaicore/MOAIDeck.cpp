// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBoundsDeck.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// MOAIDeckGfxState
//================================================================//

//----------------------------------------------------------------//
MOAIDeckGfxState::MOAIDeckGfxState () :
	mShader ( 0 ),
	mTexture ( 0 ) {
}

//----------------------------------------------------------------//
void MOAIDeckGfxState::Reset () {
	this->mShader = 0;
	this->mTexture = 0;
}

//----------------------------------------------------------------//
void MOAIDeckGfxState::SetShader ( MOAIGfxState* shader ) {

	if ( shader ) {
		this->mShader = shader;
	}
}

//----------------------------------------------------------------//
void MOAIDeckGfxState::SetTexture ( MOAIGfxState* texture ) {

	if ( texture ) {
		this->mTexture = texture;
	}
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setBoundsDeck
	@text	Set or clear the bounds override deck.
	
	@in		MOAIDeck self
	@opt	MOAIBoundsDeck boundsDeck
	@out	nil
*/
int MOAIDeck::_setBoundsDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	
	self->mBoundsDeck.Set ( *self, state.GetLuaObject < MOAIBoundsDeck >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShader
	@text	Set the shader to use if neither the deck item nor the prop specifies a shader.
	
	@in		MOAIDeck self
	@in		MOAIShader shader
	@out	nil
*/
int MOAIDeck::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "UU" )
	
	self->mShader.Set ( *self, state.GetLuaObject < MOAIShader >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIDeck self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIDeck::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, 2 );
	self->mTexture.Set ( *self, texture );

	if ( texture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec ) {
	
	USRect bounds = this->GetBounds ( idx, remapper ).GetRect ( USBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIDeckRemapper* remapper ) {

	this->Draw ( idx, remapper, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIDeckRemapper* remapper, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	
	idx = remapper ? remapper->Remap ( idx ) : idx;
	
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	xScl = ( idx & MOAITileFlags::XFLIP ) ? -xScl : xScl;
	yScl = ( idx & MOAITileFlags::YFLIP ) ? -yScl : yScl;
	
	this->DrawIndex ( idx & MOAITileFlags::CODE_MASK, xOff, yOff, zOff, xScl, yScl, zScl );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( idx );
	UNUSED ( xOff );
	UNUSED ( yOff );
	UNUSED ( zOff );
	UNUSED ( xScl );
	UNUSED ( yScl );
	UNUSED ( zScl );
}

//----------------------------------------------------------------//
USBox MOAIDeck::GetBounds () {

	if ( this->mBoundsDirty ) {
		this->mMaxBounds = this->ComputeMaxBounds ();
		
		// flip and expand to account for flip flags
		USBox bounds = this->mMaxBounds;
		bounds.Scale ( -1.0f );
		bounds.Bless ();
		
		this->mMaxBounds.Grow ( bounds );
		this->mBoundsDirty = false;
	}
	return this->mMaxBounds;
}

//----------------------------------------------------------------//
USBox MOAIDeck::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {

	idx = remapper ? remapper->Remap ( idx ) : idx;
	
	USBox bounds;
	
	if ( this->mBoundsDeck ) {
		bounds = this->mBoundsDeck->GetItemBounds ( idx & MOAITileFlags::CODE_MASK );
	}
	else {
		bounds = this->GetItemBounds ( idx & MOAITileFlags::CODE_MASK );
	}

	if ( idx & MOAITileFlags::FLIP_MASK ) {

		USVec3D scale;
		scale.mX = ( idx & MOAITileFlags::XFLIP ) ? -1.0f : 1.0f;
		scale.mY = ( idx & MOAITileFlags::YFLIP ) ? -1.0f : 1.0f;
		scale.mZ = 1.0f;

		bounds.Scale ( scale );
		bounds.Bless ();
	}
	return bounds;
}

//----------------------------------------------------------------//
void MOAIDeck::GetGfxState ( MOAIDeckGfxState& gfxState ) {

	if ( this->mShader ) {
		gfxState.SetShader ( this->mShader );
	}
	else {
		gfxState.SetShader ( &MOAIShaderMgr::Get ().GetShader ( this->mDefaultShaderID ));
	}
	gfxState.SetTexture ( this->mTexture );
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mContentMask ( 0xffffffff ),
	mDefaultShaderID ( MOAIShaderMgr::DECK2D_SHADER ),
	mBoundsDirty ( true ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->mMaxBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {

	this->mShader.Set ( *this, 0 );
	this->mTexture.Set ( *this, 0 );
	this->mBoundsDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setBoundsDeck",			_setBoundsDeck },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeck::SetBoundsDirty () {

	this->mBoundsDirty = true;
}
