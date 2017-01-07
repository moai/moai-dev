// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIGfxMgr.h>
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
// TODO: doxygen
int MOAIDeck::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	u32 index						= state.GetValue < u32 >( 2, 1 );
	//MOAIMaterialBatch* materials	= state.GetLuaObject < MOAIMaterialBatch >( 3, false );
	//ZLVec3D offset					= state.GetValue < ZLVec3D >( 4, ZLVec3D::ORIGIN );
	//ZLVec3D scale					= state.GetValue < ZLVec3D >( 7, ZLVec3D::AXIS );

	self->Draw ( index );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return bounds for an item or the maximum bounds for the
			deck.

	@override

		@in		number idx
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
	
	@override
	
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
*/
int MOAIDeck::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	
	ZLBox box;
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
		box = self->GetBounds ( idx );
	}
	else {
	
		box = self->GetBounds ();
	}
	
	state.Push ( box );
	return 6;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx ) {

	this->MOAIDeck_Draw ( idx );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds () {

	return this->GetBounds ( 0 );
	
	if ( this->mBoundsDirty ) {
	
		this->mMaxBounds = this->MOAIDeck_ComputeMaxBounds ();
		
		// flip and expand to account for flip flags
		//ZLBox bounds = this->mMaxBounds;
		//bounds.Scale ( -1.0f );
		//bounds.Bless ();
		
		//this->mMaxBounds.Grow ( bounds );
		this->mBoundsDirty = false;
	}
	return this->mMaxBounds;
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds ( u32 idx ) {

	return this->MOAIDeck_GetBounds ( idx );
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::GetCollisionShape ( u32 idx ) {

	return MOAIDeck_GetCollisionShape ( idx );
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIDeck::GetMaterialBatch () {

	return this->MOAIDeck_GetMaterialBatch ();
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mBoundsDirty ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {
}

//----------------------------------------------------------------//
bool MOAIDeck::Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	return this->Overlap ( idx, vec, granularity, result );
}

//----------------------------------------------------------------//
bool MOAIDeck::Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {

	return this->Overlap ( idx, vec, granularity, result );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "draw",					_draw },
		{ "getBounds",				_getBounds },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeck::SetBoundsDirty () {

	this->mBoundsDirty = true;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_ComputeMaxBounds () {

	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIDeck::MOAIDeck_Draw ( u32 idx ) {
	UNUSED ( idx );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_GetBounds ( u32 idx ) {
	UNUSED ( idx );

	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::MOAIDeck_GetCollisionShape ( u32 idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIDeck::MOAIDeck_GetMaterialBatch () {

	return 0;
}

//----------------------------------------------------------------//
bool MOAIDeck::MOAIDeck_Overlap ( u32 idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( result );
	UNUSED ( granularity );
	
	ZLBounds bounds = this->GetBounds ( idx );
	return (( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) && bounds.Contains ( ZLVec3D ( vec.mY, vec.mY, 0.0f ), ZLBox::PLANE_XY ));
}

//----------------------------------------------------------------//
bool MOAIDeck::MOAIDeck_Overlap ( u32 idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( result );
	UNUSED ( granularity );
	
	ZLBounds bounds = this->GetBounds ( idx );
	return (( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) && bounds.Contains ( vec ));
}
