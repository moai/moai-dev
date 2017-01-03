// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsProp
//================================================================//

//----------------------------------------------------------------//
bool MOAIGraphicsProp::ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {
	
	if ( MOAIIndexedPropBase::ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIGraphicsPropBase::ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
MOAIGraphicsProp::MOAIGraphicsProp () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckPropBase )
		RTTI_EXTEND ( MOAIIndexedPropBase )
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIGraphicsPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsProp::~MOAIGraphicsProp () {
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::OnDepNodeUpdate () {
	
	MOAIGraphicsPropBase::OnDepNodeUpdate ();
	MOAIPartitionHull::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
	MOAIIndexedPropBase::RegisterLuaClass ( state );
	MOAIGraphicsPropBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	MOAIIndexedPropBase::RegisterLuaFuncs ( state );
	MOAIGraphicsPropBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIPartitionHull::SerializeIn ( state, serializer );
	MOAIIndexedPropBase::SerializeIn ( state, serializer );
	MOAIGraphicsPropBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIPartitionHull::SerializeOut ( state, serializer );
	MOAIIndexedPropBase::SerializeOut ( state, serializer );
	MOAIGraphicsPropBase::SerializeOut ( state, serializer );
}

//================================================================//
// MOAIGraphicsProp virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAIAbstractDrawable_Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );

	if ( !this->IsVisible ( lod )) return;
	if ( !this->mDeck ) return;
	if ( this->IsClear ()) return;

	this->LoadGfxState ();
	this->LoadVertexTransform ();
	this->LoadUVTransform ();
	
	this->mDeck->Draw ( this->mIndex, this->mMaterialBatch );
}

//----------------------------------------------------------------//
u32 MOAIGraphicsProp::MOAIPartitionHull_GetModelBounds ( ZLBox& bounds ) {
	
	if ( this->mDeck ) {
	
		bounds = this->mDeck->GetBounds ( this->mIndex );
		return BOUNDS_OK;
	}
	return BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );

	bool passTrivial = this->InsideModelBounds ( vec, pad );
	
	// TODO: handle grids
	if ( passTrivial && this->mDeck && ( this->mHitGranularity > HIT_TEST_COARSE )) {
	
		return this->mDeck->Inside ( this->mIndex, this->mMaterialBatch, this->mHitGranularity, vec, pad );
	}
	return passTrivial;
}
