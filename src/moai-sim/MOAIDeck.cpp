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
// local
//================================================================//

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

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, const ZLVec2D& vec ) {
	
	ZLRect bounds = this->GetBounds ( idx ).GetRect ( ZLBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIMaterialBatch& materials ) {

	this->Draw ( idx, materials, ZLVec3D::ORIGIN, ZLVec3D::AXIS );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	scale.mX = ( idx & MOAITileFlags::XFLIP ) ? -scale.mX : scale.mX;
	scale.mY = ( idx & MOAITileFlags::YFLIP ) ? -scale.mY : scale.mY;
	
	this->DrawIndex ( idx & MOAITileFlags::CODE_MASK, materials, offset, scale );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( idx );
	UNUSED ( materials );
	UNUSED ( offset );
	UNUSED ( scale );
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
	mBoundsDirty ( true ),
	mHitGranularity ( HIT_TEST_COARSE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialBatch )
	RTTI_END
	
	this->mMaxBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {

	this->mBoundsDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIMaterialBatch::RegisterLuaClass ( state );

	state.SetField ( -1, "HIT_TEST_COARSE",			HIT_TEST_COARSE );
	state.SetField ( -1, "HIT_TEST_MEDIUM",			HIT_TEST_MEDIUM );
	state.SetField ( -1, "HIT_TEST_FINE",			HIT_TEST_FINE );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIMaterialBatch::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setBoundsDeck",			_setBoundsDeck },
		{ "setHitGranularity",		_setHitGranularity },
		{ "setHitMask",				_setHitMask },
		{ "setHitMaskScalar",		_setHitMaskScalar },
		{ "setHitMaskThreshold",	_setHitMaskThreshold },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIDeck::ResolveMaterialBatch ( MOAIMaterialBatch* override ) {

	return override ? override : this;
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