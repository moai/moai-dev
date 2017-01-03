// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGraphicsGridProp.h>
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
// MOAIGraphicsGridProp
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsGridProp::MOAIGraphicsGridProp () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckPropBase )
		RTTI_EXTEND ( MOAIGridPropBase )
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIGraphicsPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsGridProp::~MOAIGraphicsGridProp () {
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaClass ( state );
	MOAIGridPropBase::RegisterLuaClass ( state );
	MOAIGraphicsPropBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIPartitionHull::RegisterLuaFuncs ( state );
	MOAIGridPropBase::RegisterLuaFuncs ( state );
	MOAIGraphicsPropBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIPartitionHull::SerializeIn ( state, serializer );
	MOAIGridPropBase::SerializeIn ( state, serializer );
	MOAIGraphicsPropBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIPartitionHull::SerializeOut ( state, serializer );
	MOAIGridPropBase::SerializeOut ( state, serializer );
	MOAIGraphicsPropBase::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAIAbstractDrawable_Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );

	if ( !this->IsVisible ( lod )) return;
	if ( !this->mDeck ) return;
	if ( this->IsClear ()) return;
	if ( !this->mGrid ) return;

	this->LoadGfxState ();
	this->LoadVertexTransform ();
	this->LoadUVTransform ();
	
	MOAIGrid& grid = *this->mGrid;
	MOAICellCoord c0, c1;

	if ( subPrimID == MOAIPartitionHull::NO_SUBPRIM_ID ) {
		this->GetGridBoundsInView ( c0, c1 );
	}
	else {
		c0 = c1 = grid.GetCellCoord ( subPrimID );
	}
	grid.Draw ( this->mDeck, 0, this->mMaterialBatch, c0, c1 );
}

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {
	
	if ( MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAINode_Update () {
	
	MOAIGraphicsPropBase::MOAINode_Update ();
	MOAIPartitionHull::MOAINode_Update ();
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	if (( this->mFlags & FLAGS_EXPAND_FOR_SORT ) && this->mGrid && this->mDeck ) {
		
		// add a sub-prim for each visible grid cell
		const ZLAffine3D& mtx = this->GetLocalToWorldMtx ();
		
		MOAIGrid& grid = *this->mGrid;
		
		MOAICellCoord c0;
		MOAICellCoord c1;
		
		// TODO: this needs to be pushed up one level to GatherHulls
		// should not assume anything about the view or rendering
		// only need to do this if we have a frustum - will break
		// expected results for other queries
		this->GetGridBoundsInView ( c0, c1 );

		for ( int y = c0.mY; y <= c1.mY; ++y ) {
			for ( int x = c0.mX; x <= c1.mX; ++x ) {
				
				MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
				u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
				if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
				
				MOAICellCoord coord ( x, y );
				int subPrimID = grid.GetCellAddr ( coord );
				
				ZLVec3D loc;
				loc.Init ( grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER ));
				
				ZLBox bounds = this->mDeck->GetBounds ( idx );
				bounds.Offset ( loc );
				
				mtx.Transform ( loc );
				bounds.Transform ( mtx );
				
				buffer.PushResult ( *this, key, subPrimID, this->GetPriority (), loc, this->GetBounds ()); // TODO: should use tile bounds for expand mode
			}
		}
	}
	else {
		buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetBounds ());
	}
}

//----------------------------------------------------------------//
u32 MOAIGraphicsGridProp::MOAIPartitionHull_GetModelBounds ( ZLBox& bounds ) {
	
	if ( this->mGrid ) {
		
		if ( this->mGrid->GetRepeat ()) {
			return BOUNDS_GLOBAL;
		}
		ZLRect rect = this->mGrid->GetBounds ();
		bounds.Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, 0.0f, 0.0f );
		return this->mGrid->GetRepeat () ? BOUNDS_GLOBAL : BOUNDS_OK;
	}
	return BOUNDS_EMPTY;
}
