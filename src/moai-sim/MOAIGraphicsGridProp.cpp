// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFancyGrid.h>
#include <moai-sim/MOAIGraphicsGridProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsGridProp
//================================================================//

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::DrawGrid ( const MOAICellCoord &c0, const MOAICellCoord &c1 ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	ZLVec3D offset	= ZLVec3D::ORIGIN;
	ZLVec3D scale	= ZLVec3D::AXIS;
	
	assert ( this->mGrid );
	MOAIGrid& grid = *this->mGrid;
	
	float tileWidth = grid.GetTileWidth ();
	float tileHeight = grid.GetTileHeight ();
	
	// is this grid fancy?
	MOAIFancyGrid* fancyGrid = this->mGrid->AsType < MOAIFancyGrid >();
	
	const ZLAffine3D& modelToWorldMtx = this->GetWorldDrawingMtx ();
	ZLColorVec penColor = gfxMgr.GetPenColor ();
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			ZLIndex addr = grid.GetCellAddr ( x, y );
			u32 idx = grid.GetTile ( addr );
			
			if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
			
			MOAICellCoord coord ( x, y );
			ZLVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );

			float xScale = ( idx & MOAITileFlags::XFLIP ) ? -tileWidth : tileWidth;
			float yScale = ( idx & MOAITileFlags::YFLIP ) ? -tileHeight : tileHeight;

			ZLAffine3D mtx = modelToWorldMtx;

			if ( idx & MOAITileFlags::ROT_90 ) {
				mtx.PrependRot90SclTr2D ( xScale, yScale, loc.mX, loc.mY );
			}
			else {
				mtx.PrependSclTr2D ( xScale, yScale, loc.mX, loc.mY );
			}

			gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX, mtx );
			
			if ( fancyGrid ) {
				gfxMgr.SetPenColor ( penColor * fancyGrid->GetTileColor ( addr ));
			}
			
			this->mDeck->Draw (( idx & MOAITileFlags::CODE_MASK ) - 1 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::GetGridFrameInView ( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ) {

	const ZLFrustum& frustum = MOAIGfxMgr::Get ().GetViewVolume ();
	
	ZLRect viewRect;
	//if ( frustum.GetXYSectRect ( this->GetWorldToLocalMtx (), viewRect )) {
	if ( frustum.GetXYSectRect ( worldToLocalMtx, viewRect )) {
	
		// TODO: need to take into account perspective and truncate rect based on horizon
		// TODO: consider bringing back poly to tile scanline converter...

		ZLRect deckFrame = this->mDeck->GetBounds ().mAABB.GetRect ( ZLBox::PLANE_XY );

		this->mGrid->GetFrameInRect ( viewRect, c0, c1, deckFrame );
	}
}

//----------------------------------------------------------------//
MOAIGraphicsGridProp::MOAIGraphicsGridProp () {
	
	RTTI_BEGIN ( MOAIGraphicsGridProp )
		RTTI_EXTEND ( MOAIHasDeckAndGrid )
		RTTI_EXTEND ( MOAIAbstractGraphicsProp )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsGridProp::~MOAIGraphicsGridProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAIAbstractRenderNode_LoadGfxState ( u32 renderPhase ) {
	if ( renderPhase == MOAIAbstractRenderNode::RENDER_PHASE_DRAW_DEBUG ) return true;

	if ( this->mDeck && MOAIAbstractGraphicsProp::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) {
		this->LoadUVTransform ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAIAbstractRenderNode_Render ( u32 renderPhase ) {
	
	if ( renderPhase == MOAIAbstractRenderNode::RENDER_PHASE_DRAW ) {
		this->DrawDebug ();
		return;
	}
	
	MOAICellCoord c0, c1;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	u32 subPrimID = gfxMgr.GetIndex ();

	if ( subPrimID == MOAIPartitionHull::NO_SUBPRIM_ID ) {
		this->GetGridFrameInView ( this->GetWorldToLocalMtx (), c0, c1 );
	}
	else {
		c0 = c1 = this->mGrid->GetCellCoord ( subPrimID );
	}
	this->DrawGrid ( c0, c1 );
}

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIAbstractGraphicsProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAINode_Update () {
	
	MOAIAbstractGraphicsProp::MOAINode_Update ();
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
		this->GetGridFrameInView ( this->GetWorldToLocalMtx (), c0, c1 );

		for ( int y = c0.mY; y <= c1.mY; ++y ) {
			for ( int x = c0.mX; x <= c1.mX; ++x ) {

				MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
				u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
				if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;

				MOAICellCoord coord ( x, y );
				int subPrimID = grid.GetCellAddr ( coord );

				ZLVec3D loc;
				loc.Init ( grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER ));

				ZLBox aabb = this->mDeck->GetBounds ( idx ).mAABB;
				aabb.Offset ( loc );

				mtx.Transform ( loc );
				aabb.Transform ( mtx );

				buffer.PushResult ( *this, key, subPrimID, this->GetPriority (), loc, aabb, this->GetPiv ());
			}
		}
	}
	else {
		buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetWorldBounds ().mAABB, this->GetPiv ());
	}
}
