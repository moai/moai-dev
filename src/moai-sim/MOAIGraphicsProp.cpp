// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsProp
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsProp::MOAIGraphicsProp () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckHolderWithIndex )
		RTTI_EXTEND ( MOAIGraphicsPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsProp::~MOAIGraphicsProp () {
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeckHolderWithIndex::RegisterLuaClass ( state );
	MOAIGraphicsPropBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	
	MOAIDeckHolderWithIndex::RegisterLuaFuncs ( state );
	MOAIGraphicsPropBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIDeckHolderWithIndex::SerializeIn ( state, serializer );
	MOAIGraphicsPropBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIDeckHolderWithIndex::SerializeOut ( state, serializer );
	MOAIGraphicsPropBase::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAIAbstractDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	if ( !this->IsVisible ()) return;
	if ( !this->mDeck ) return;
	if ( this->IsClear ()) return;

	this->PushGfxState ();
	this->LoadVertexTransform ();
	this->LoadUVTransform ();
	
	this->mDeck->Draw ( this->mIndex );
	
	this->PopGfxState ();
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {
	
	if ( MOAIDeckHolderWithIndex::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAINode_Update () {
	
	MOAIGraphicsPropBase::MOAINode_Update ();
}

//----------------------------------------------------------------//
ZLBounds MOAIGraphicsProp::MOAIPartitionHull_GetModelBounds () {
	
	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );

	bool passTrivial = this->InsideModelBounds ( vec, pad );
	
	// TODO: handle grids
	// TODO: handle padding
	if ( passTrivial && this->mDeck && ( this->mHitGranularity > HIT_TEST_COARSE )) {
	
		//return this->mDeck->Inside ( this->mIndex, this->mMaterialBatch, this->mHitGranularity, vec, pad );
		return this->mDeck->Overlap ( this->mIndex, vec.Vec2D(), this->mHitGranularity, 0 );
	}
	return passTrivial;
}
