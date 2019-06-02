// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeck::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	ZLIndex index					= state.GetValue < ZLIndex >( 2, ZLIndexOp::ZERO );
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

	@overload

		@in		number idx
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
	
	@overload
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
	
		ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
		box = self->GetBounds ( idx ).mAABB;
	}
	else {
		box = self->GetBounds ().mAABB;
	}
	
	state.Push ( box );
	return 6;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIDeck::Draw ( ZLIndex idx ) {

	this->MOAIDeck_Draw ( idx );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds () {

	return this->GetBounds ( ZLIndexOp::ZERO );
	/*
	if ( this->mBoundsDirty ) {
	
		this->mMaxBounds = this->MOAIDeck_ComputeMaxAABB ();
		
		// flip and expand to account for flip flags
		//ZLBox aabb = this->mMaxBounds;
		//aabb.Scale ( -1.0f );
		//aabb.Bless ();
		
		//this->mMaxBounds.Grow ( aabb );
		this->mBoundsDirty = false;
	}
	return this->mMaxBounds;*/
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::GetBounds ( ZLIndex idx ) {

	return this->MOAIDeck_GetBounds ( idx );
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::GetCollisionShape ( ZLIndex idx ) {

	return MOAIDeck_GetCollisionShape ( idx );
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mMaxBounds ( ZLBounds::EMPTY ),
	mBoundsDirty ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {
}

//----------------------------------------------------------------//
bool MOAIDeck::Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {

	return this->MOAIDeck_Overlap ( idx, vec, granularity, result );
}


//----------------------------------------------------------------//
void MOAIDeck::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeck::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

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
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_ComputeMaxAABB () {

	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIDeck::MOAIDeck_Draw ( ZLIndex idx ) {
	UNUSED ( idx );
}

//----------------------------------------------------------------//
ZLBounds MOAIDeck::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAIDeck::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {
	UNUSED ( idx );

	return 0;
}

//----------------------------------------------------------------//
bool MOAIDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( result );
	UNUSED ( granularity );
	
	ZLBounds bounds = this->GetBounds ( idx );
	return (( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) && bounds.mAABB.Contains ( ZLVec3D ( vec.mY, vec.mY, 0.0f ), ZLBox::PLANE_XY ));
}

//----------------------------------------------------------------//
bool MOAIDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( result );
	UNUSED ( granularity );
	
	ZLBounds bounds = this->GetBounds ( idx );
	return (( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) && bounds.mAABB.Contains ( vec ));
}
