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
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsGridProp
//================================================================//

//----------------------------------------------------------------//
//ZLAffine3D MOAIGraphicsGridProp::AppendRot90SclTr ( const ZLAffine3D& mtx, const ZLAffine3D& append ) {
//
//	// don't need a general purpose matrix mult to handle just scale and offset.
//	// can omit a lot of the multiplications.
//
//	// 0 s 0 z
//	// s 0 0 y
//	// 0 0 1 0
//
//	ZLAffine3D result;
//
//	result.m [ ZLAffine3D::C0_R0 ]	=	( append.m [ ZLAffine3D::C1_R0 ] * mtx.m [ ZLAffine3D::C0_R1 ]);
//	result.m [ ZLAffine3D::C0_R1 ]	=	( append.m [ ZLAffine3D::C1_R1 ] * mtx.m [ ZLAffine3D::C0_R1 ]);
//	result.m [ ZLAffine3D::C0_R2 ]	=	( append.m [ ZLAffine3D::C1_R2 ] * mtx.m [ ZLAffine3D::C0_R1 ]);
//
//	result.m [ ZLAffine3D::C1_R0 ]	=	( append.m [ ZLAffine3D::C0_R0 ] * mtx.m [ ZLAffine3D::C1_R0 ]);
//	result.m [ ZLAffine3D::C1_R1 ]	=	( append.m [ ZLAffine3D::C0_R1 ] * mtx.m [ ZLAffine3D::C1_R0 ]);
//	result.m [ ZLAffine3D::C1_R2 ]	=	( append.m [ ZLAffine3D::C0_R2 ] * mtx.m [ ZLAffine3D::C1_R0 ]);
//	
//	result.m [ ZLAffine3D::C2_R0 ]	=	append.m [ ZLAffine3D::C2_R0 ];
//	result.m [ ZLAffine3D::C2_R1 ]	=	append.m [ ZLAffine3D::C2_R1 ];
//	result.m [ ZLAffine3D::C2_R2 ]	=	append.m [ ZLAffine3D::C2_R2 ];
//	
//	result.m [ ZLAffine3D::C3_R0 ]	=	( append.m [ ZLAffine3D::C0_R0 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R0 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R0 ]);
//	
//	result.m [ ZLAffine3D::C3_R1 ]	=	( append.m [ ZLAffine3D::C0_R1 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R1 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R1 ]);
//	
//	result.m [ ZLAffine3D::C3_R2 ]	=	( append.m [ ZLAffine3D::C0_R2 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R2 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R2 ]);
//	
//	return result;
//}

//----------------------------------------------------------------//
//ZLAffine3D MOAIGraphicsGridProp::AppendSclTr ( const ZLAffine3D& mtx, const ZLAffine3D& append ) {
//
//	// don't need a general purpose matrix mult to handle just scale and offset.
//	// can omit a lot of the multiplications.
//
//	// s 0 0 x
//	// 0 s 0 y
//	// 0 0 1 0
//
//	ZLAffine3D result;
//
//	result.m [ ZLAffine3D::C0_R0 ]	=	( append.m [ ZLAffine3D::C0_R0 ] * mtx.m [ ZLAffine3D::C0_R0 ]);
//	result.m [ ZLAffine3D::C0_R1 ]	=	( append.m [ ZLAffine3D::C0_R1 ] * mtx.m [ ZLAffine3D::C0_R0 ]);
//	result.m [ ZLAffine3D::C0_R2 ]	=	( append.m [ ZLAffine3D::C0_R2 ] * mtx.m [ ZLAffine3D::C0_R0 ]);
//
//	result.m [ ZLAffine3D::C1_R0 ]	=	( append.m [ ZLAffine3D::C1_R0 ] * mtx.m [ ZLAffine3D::C1_R1 ]);
//	result.m [ ZLAffine3D::C1_R1 ]	=	( append.m [ ZLAffine3D::C1_R1 ] * mtx.m [ ZLAffine3D::C1_R1 ]);
//	result.m [ ZLAffine3D::C1_R2 ]	=	( append.m [ ZLAffine3D::C1_R2 ] * mtx.m [ ZLAffine3D::C1_R1 ]);
//	
//	result.m [ ZLAffine3D::C2_R0 ]	=	append.m [ ZLAffine3D::C2_R0 ];
//	result.m [ ZLAffine3D::C2_R1 ]	=	append.m [ ZLAffine3D::C2_R1 ];
//	result.m [ ZLAffine3D::C2_R2 ]	=	append.m [ ZLAffine3D::C2_R2 ];
//	
//	result.m [ ZLAffine3D::C3_R0 ]	=	( append.m [ ZLAffine3D::C0_R0 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R0 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R0 ]);
//	
//	result.m [ ZLAffine3D::C3_R1 ]	=	( append.m [ ZLAffine3D::C0_R1 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R1 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R1 ]);
//	
//	result.m [ ZLAffine3D::C3_R2 ]	=	( append.m [ ZLAffine3D::C0_R2 ] * mtx.m [ ZLAffine3D::C3_R0 ])	+
//										( append.m [ ZLAffine3D::C1_R2 ] * mtx.m [ ZLAffine3D::C3_R1 ])	+
//										( append.m [ ZLAffine3D::C3_R2 ]);
//	
//	return result;
//}

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
			
			this->mDeck->Draw ( ZLIndexCast (( idx & MOAITileFlags::CODE_MASK ) - 1 ));
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
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIHasDeckAndGrid )
		RTTI_EXTEND ( MOAIGraphicsPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsGridProp::~MOAIGraphicsGridProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAIGraphicsGridProp::MOAIAbstractProp_GetModelBounds () {
	
	if ( this->mGrid ) {
	
		if ( this->mGrid->GetRepeat ()) {
			return ZLBounds::GLOBAL;
		}
		return ZLBounds ( this->mGrid->GetFrame ());
	}
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	if ( !this->IsVisible ()) return;
	if ( !this->mDeck ) return;
	if ( this->IsClear ()) return;

	this->PushGfxState ();
	this->LoadUVTransform ();
	
	MOAICellCoord c0, c1;

	if ( subPrimID == MOAIPartitionHull::NO_SUBPRIM_ID ) {
		this->GetGridFrameInView ( this->GetWorldToLocalMtx (), c0, c1 );
	}
	else {
		c0 = c1 = this->mGrid->GetCellCoord ( ZLIndexCast ( subPrimID ));
	}
	
	this->DrawGrid ( c0, c1 );
	
	this->PopGfxState ();
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndGrid, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndGrid, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndGrid, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeckAndGrid, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsPropBase, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIGraphicsPropBase::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAINode_Update () {
	
	MOAIGraphicsPropBase::MOAINode_Update ();
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
				
				ZLBox aabb = this->mDeck->GetBounds ( ZLIndexCast ( idx )).mAABB;
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
