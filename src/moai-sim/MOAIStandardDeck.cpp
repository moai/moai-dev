// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBoundsDeck.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIStandardDeck.h>
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
	
	@in		MOAIStandardDeck self
	@opt	MOAIBoundsDeck boundsDeck
	@out	nil
*/
int MOAIStandardDeck::_setBoundsDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStandardDeck, "U" )
	
	self->mBoundsDeck.Set ( *self, state.GetLuaObject < MOAIBoundsDeck >( 2, true ));
	
	return 0;
}

//================================================================//
// MOAIStandardDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIStandardDeck::Contains ( u32 idx, const ZLVec2D& vec ) {
	
	ZLRect bounds = this->GetBounds ( idx ).GetRect ( ZLBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIStandardDeck::Draw ( u32 idx, MOAIMaterialBatch& materials ) {

	this->Draw ( idx, materials, ZLVec3D::ORIGIN, ZLVec3D::AXIS );
}

//----------------------------------------------------------------//
void MOAIStandardDeck::Draw ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	scale.mX = ( idx & MOAITileFlags::XFLIP ) ? -scale.mX : scale.mX;
	scale.mY = ( idx & MOAITileFlags::YFLIP ) ? -scale.mY : scale.mY;
	
	this->DrawIndex ( idx & MOAITileFlags::CODE_MASK, materials, offset, scale );
}

//----------------------------------------------------------------//
void MOAIStandardDeck::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( idx );
	UNUSED ( materials );
	UNUSED ( offset );
	UNUSED ( scale );
}

//----------------------------------------------------------------//
ZLBox MOAIStandardDeck::GetBounds () {

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
ZLBox MOAIStandardDeck::GetBounds ( u32 idx ) {
	
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
void MOAIStandardDeck::GetCollisionShape ( MOAICollisionShape& shape ) {

	shape.Set ();
}

//----------------------------------------------------------------//
bool MOAIStandardDeck::Inside ( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad ) {
	UNUSED ( idx );
	UNUSED ( materials );
	UNUSED ( granularity );
	UNUSED ( vec );
	UNUSED ( pad );
	
	// we've passed the trivial reject; if we don't have a more finely grained test, return 'true'
	return true;
}

//----------------------------------------------------------------//
MOAIStandardDeck::MOAIStandardDeck () :
	mBoundsDirty ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIMaterialBatch )
	RTTI_END
	
	this->mMaxBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIStandardDeck::~MOAIStandardDeck () {

	this->mBoundsDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIStandardDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	MOAIMaterialBatch::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStandardDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatch::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setBoundsDeck",			_setBoundsDeck },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIStandardDeck::ResolveMaterialBatch ( MOAIMaterialBatch* override ) {

	return override ? *override : *this;
}

//----------------------------------------------------------------//
void MOAIStandardDeck::SetBoundsDirty () {

	this->mBoundsDirty = true;
}
