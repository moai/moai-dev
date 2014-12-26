// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBoundsDeck.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>

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
void MOAIDeckGfxState::SetShader ( MOAIShader* shader ) {

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
// TODO: doxygen
int MOAIDeck::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	self->mTexture->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setBoundsDeck
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
/**	@lua	setHitGranularity
	@text	Specify the granularity to use when performing a hit test. This is a hint to the implementation
			as to how much processing to allocate to a given test. The default value is MOAIDeck.HIT_TEST_COARSE,
			which will cause only the deck or prop bounds to be used. A setting of MOAIDeck.HIT_TEST_MEDIUM or
			MOAIDeck.HIT_TEST_FINE is implementation dependent on the deck, but 'medium' usually means to test
			against the geometry of the deck and 'fine' means to test against both the geometry and the pixels
			of the hit mask (if any).
	
	@opt	int granularity		One of MOAIDeck.HIT_TEST_COARSE, MOAIDeck.HIT_TEST_MEDIUM, MOAIDeck.HIT_TEST_FINE. Default is MOAIDeck.HIT_TEST_COARSE.
	@out	nil
*/
int MOAIDeck::_setHitGranularity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	self->mHitGranularity = state.GetValue < u32 >( 2, HIT_TEST_COARSE );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitMask
	@text	Set or load an image to act as the hit mask for this deck.
	
	@in		MOAIDeck self
	@in		variant mask		A MOAIImage or a path to an image file
	@out	MOAIImage mask
*/
int MOAIDeck::_setHitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	MOAIImage* image = MOAIImage::AffirmImage ( state, 2 );
	self->mHitMask.Set ( *self, image );

	if ( image ) {
		self->mHitMask->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeck::_setHitMaskScalar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	float rS	= state.GetValue < float>( 2, 1.0f );
	float gS	= state.GetValue < float>( 3, 1.0f );
	float bS	= state.GetValue < float>( 4, 1.0f );
	float aS	= state.GetValue < float>( 5, 1.0f );

	self->mHitColorScalar = ZLColor::PackRGBA ( rS, gS, bS, aS );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeck::_setHitMaskThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	float rT	= state.GetValue < float>( 2, 0.0f );
	float gT	= state.GetValue < float>( 3, 0.0f );
	float bT	= state.GetValue < float>( 4, 0.0f );
	float aT	= state.GetValue < float>( 5, 0.0f );

	self->mHitColorThreshold = ZLColor::PackRGBA ( rT, gT, bT, aT );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
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
/**	@lua	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIDeck self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAIImage.QUANTIZE, MOAIImage.TRUECOLOR, MOAIImage.PREMULTIPLY_ALPHA
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
bool MOAIDeck::Contains ( u32 idx, const ZLVec2D& vec ) {
	
	ZLRect bounds = this->GetBounds ( idx ).GetRect ( ZLBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx ) {

	this->Draw ( idx, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	
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
ZLBox MOAIDeck::GetBounds () {

	if ( this->mBoundsDirty ) {
		this->mMaxBounds = this->ComputeMaxBounds ();
		
		// flip and expand to account for flip flags
		ZLBox bounds = this->mMaxBounds;
		bounds.Scale ( -1.0f );
		bounds.Bless ();
		
		this->mMaxBounds.Grow ( bounds );
		this->mBoundsDirty = false;
	}
	return this->mMaxBounds;
}

//----------------------------------------------------------------//
ZLBox MOAIDeck::GetBounds ( u32 idx ) {
	
	ZLBox bounds;
	
	if ( this->mBoundsDeck ) {
		bounds = this->mBoundsDeck->GetItemBounds ( idx & MOAITileFlags::CODE_MASK );
	}
	else {
		bounds = this->GetItemBounds ( idx & MOAITileFlags::CODE_MASK );
	}

	if ( idx & MOAITileFlags::FLIP_MASK ) {

		ZLVec3D scale;
		scale.mX = ( idx & MOAITileFlags::XFLIP ) ? -1.0f : 1.0f;
		scale.mY = ( idx & MOAITileFlags::YFLIP ) ? -1.0f : 1.0f;
		scale.mZ = 1.0f;

		bounds.Scale ( scale );
		bounds.Bless ();
	}
	return bounds;
}

//----------------------------------------------------------------//
void MOAIDeck::GetCollisionShape ( MOAICollisionShape& shape ) {

	shape.Set ();
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
bool MOAIDeck::Inside ( u32 idx, ZLVec3D vec, float pad ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( pad );
	
	// we've passed the trivial reject; if we don't have a more finely grained test, return 'true'
	return true;
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mHitColorScalar ( 0xffffffff ),
	mHitColorThreshold ( 0x00000000 ),
	//mContentMask ( 0xffffffff ),
	mDefaultShaderID ( MOAIShaderMgr::DECK2D_SHADER ),
	mBoundsDirty ( true ),
	mHitGranularity ( HIT_TEST_COARSE ) {
	
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

	state.SetField ( -1, "HIT_TEST_COARSE",			HIT_TEST_COARSE );
	state.SetField ( -1, "HIT_TEST_MEDIUM",			HIT_TEST_MEDIUM );
	state.SetField ( -1, "HIT_TEST_FINE",			HIT_TEST_FINE );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getTexture",				_getTexture },
		{ "setBoundsDeck",			_setBoundsDeck },
		{ "setHitGranularity",		_setHitGranularity },
		{ "setHitMask",				_setHitMask },
		{ "setHitMaskScalar",		_setHitMaskScalar },
		{ "setHitMaskThreshold",	_setHitMaskThreshold },
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

//----------------------------------------------------------------//
bool MOAIDeck::TestHit ( float x, float y ) {

	static const ZLColorVec defaultHitColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	if ( this->mHitMask ) {
	
		float width = ( float )this->mHitMask->GetWidth ();
		float height = ( float )this->mHitMask->GetHeight ();
	
		ZLColorVec scalar ( this->mHitColorScalar );
		ZLColorVec threshold ( this->mHitColorThreshold );
	
		ZLColorVec maskColor ( this->mHitMask->GetColor (( u32 )( width * x ), ( u32 )( height * y ))); // TODO: wrap, clamp
		
		maskColor.Modulate ( scalar );
		
		return (
			( threshold.mR <= maskColor.mR ) &&
			( threshold.mG <= maskColor.mG ) &&
			( threshold.mB <= maskColor.mB ) &&
			( threshold.mA <= maskColor.mA )
		);
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIDeck::TestHit ( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) {

	u32 granularity = this->GetHitGranularity ();

	if ( granularity == HIT_TEST_COARSE ) return true;

	ZLVec2D uv;

	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 0, x, y, uv)) {
		return granularity == HIT_TEST_FINE ? this->TestHit ( uv.mX, uv.mY ) : true;
	}
	
	if ( ZLQuad::RemapCoord ( modelQuad, uvQuad, 1, x, y, uv)) {
		return granularity == HIT_TEST_FINE ? this->TestHit ( uv.mX, uv.mY ) : true;
	}

	return false;
}
