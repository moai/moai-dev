// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsProp
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsProp::MOAIGraphicsProp () {
	
	RTTI_BEGIN ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIHasDeckAndIndex )
		RTTI_EXTEND ( MOAIGraphicsPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsProp::~MOAIGraphicsProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAIDrawable_Draw ( int subPrimID ) {
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
ZLBounds MOAIGraphicsProp::MOAIAbstractProp_GetModelBounds () {
	
	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIHasDeckAndIndex::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndIndex, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndIndex, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndIndex, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndIndex, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAINode_Update () {
	
	MOAIGraphicsPropBase::MOAINode_Update ();
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
