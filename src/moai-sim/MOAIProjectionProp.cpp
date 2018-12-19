// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIProjectionProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProjectionProp::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProjectionProp, "UUUU" );
	
	MOAIGraphicsPropBase* sourceProp = state.GetLuaObject < MOAIGraphicsPropBase >( 2, true );
	if ( !sourceProp ) return 0;
	
	MOAIViewLayer* sourceLayer = state.GetLuaObject < MOAIViewLayer >( 3, true );
	if ( !sourceLayer ) return 0;
	
	MOAIViewLayer* destLayer = state.GetLuaObject < MOAIViewLayer >( 4, true );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceProp, sourceProp );
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	return 0;
}

//================================================================//
// MOAIProjectionProp
//================================================================//

//----------------------------------------------------------------//
MOAIProjectionProp::MOAIProjectionProp () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartitionHull )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIProjectionProp::~MOAIProjectionProp () {
}

//----------------------------------------------------------------//
void MOAIProjectionProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIPartitionHull::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIPartitionHull::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIAbstractDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIAbstractDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );
}


//----------------------------------------------------------------//
bool MOAIProjectionProp::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAINode_Update () {

	MOAIPartitionHull::MOAINode_Update ();
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetWorldBounds ());
}

//----------------------------------------------------------------//
u32 MOAIProjectionProp::MOAIPartitionHull_AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIAbstractDrawable >();
}

//----------------------------------------------------------------//
ZLBounds MOAIProjectionProp::MOAIPartitionHull_GetModelBounds () {

//	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
bool MOAIProjectionProp::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {

//	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
//	worldToLocal.Transform ( vec );
//
//	bool passTrivial = this->InsideModelBounds ( vec, pad );
//
//	// TODO: handle grids
//	// TODO: handle padding
//	if ( passTrivial && this->mDeck && ( this->mHitGranularity > HIT_TEST_COARSE )) {
//
//		//return this->mDeck->Inside ( this->mIndex, this->mMaterialBatch, this->mHitGranularity, vec, pad );
//		return this->mDeck->Overlap ( this->mIndex, vec.Vec2D(), this->mHitGranularity, 0 );
//	}
//	return passTrivial;
	return false;
}
