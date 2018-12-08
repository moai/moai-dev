// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIGridPropBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIGridPropBase self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIGridPropBase::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPropBase, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIGridPropBase self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIGridPropBase::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPropBase, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIGridPropBase self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIGridPropBase::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPropBase, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIGridPropBase
//================================================================//

//----------------------------------------------------------------//
//void MOAIGridPropBase::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( sampler );

	//if ( !this->mDeck ) return;
	//
	//sampler.SetSourcePrim ( this );
	//
	//if ( this->mGrid ) {
	//	
	//	ZLRect localRect = sampler.GetLocalRect ();
	//	
	//	MOAICellCoord c0;
	//	MOAICellCoord c1;
	//	
	//	ZLRect deckBounds = this->mDeck->GetBounds ().GetRect( ZLBox::PLANE_XY );

	//	this->mGrid->GetBoundsInRect ( localRect, c0, c1, deckBounds );
	//	//this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	//}
	//else {
	//	//this->mDeck->GatherSurfaces ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), sampler );
	//}
//}

//----------------------------------------------------------------//
void MOAIGridPropBase::GetGridBoundsInView ( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ) {

	const ZLFrustum& frustum = MOAIGfxMgr::Get ().mGfxState.GetViewVolume ();
	
	ZLRect viewRect;
	//if ( frustum.GetXYSectRect ( this->GetWorldToLocalMtx (), viewRect )) {
	if ( frustum.GetXYSectRect ( worldToLocalMtx, viewRect )) {
	
		// TODO: need to take into account perspective and truncate rect based on horizon
		// TODO: consider bringing back poly to tile scanline converter...

		ZLRect deckBounds = this->mDeck->GetBounds ().GetRect ( ZLBox::PLANE_XY );

		this->mGrid->GetBoundsInRect ( viewRect, c0, c1, deckBounds );
	}
}

//----------------------------------------------------------------//
MOAIGridPropBase::MOAIGridPropBase () :
	mGridScale ( 1.0f, 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGridPropBase::~MOAIGridPropBase () {
	
	this->mGrid.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGridPropBase::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeckPropBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGridPropBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIDeckPropBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getGrid",				_getGrid },
		{ "setGrid",				_setGrid },
		{ "setGridScale",			_setGridScale },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGridPropBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIDeckPropBase::SerializeIn ( state, serializer );
	
	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIGridPropBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIDeckPropBase::SerializeOut ( state, serializer );
	
	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

//================================================================//
// MOAIGridPropBase virtual
//================================================================//

//----------------------------------------------------------------//
//void MOAIGridPropBase::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {
//
//	if (( this->mFlags & FLAGS_EXPAND_FOR_SORT ) && this->mGrid && this->mDeck ) {
//		
//		// add a sub-prim for each visible grid cell
//		const ZLAffine3D& mtx = this->GetLocalToWorldMtx ();
//		
//		MOAIGrid& grid = *this->mGrid;
//		
//		MOAICellCoord c0;
//		MOAICellCoord c1;
//		
//		// TODO: this needs to be pushed up one level to GatherHulls
//		// should not assume anything about the view or rendering
//		// only need to do this if we have a frustum - will break
//		// expected results for other queries
//		this->GetGridBoundsInView ( c0, c1 );
//
//		for ( int y = c0.mY; y <= c1.mY; ++y ) {
//			for ( int x = c0.mX; x <= c1.mX; ++x ) {
//				
//				MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
//				u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
//				if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
//				
//				MOAICellCoord coord ( x, y );
//				int subPrimID = grid.GetCellAddr ( coord );
//				
//				ZLVec3D loc;
//				loc.Init ( grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER ));
//				
//				ZLBox bounds = this->mDeck->GetBounds ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ));
//				bounds.Offset ( loc );
//				
//				mtx.Transform ( loc );
//				bounds.Transform ( mtx );
//				
//				buffer.PushResult ( *this, key, subPrimID, this->GetPriority (), loc, this->GetBounds ()); // TODO: should use tile bounds for expand mode
//			}
//		}
//	}
//	else {
//		buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetBounds ());
//	}
//}

//----------------------------------------------------------------//
//u32 MOAIGridPropBase::MOAIPartitionHull_GetModelBounds ( ZLBox& bounds ) {
//	
//	if ( this->mGrid ) {
//		
//		if ( this->mGrid->GetRepeat ()) {
//			return BOUNDS_GLOBAL;
//		}
//		ZLRect rect = this->mGrid->GetBounds ();
//		bounds.Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, 0.0f, 0.0f );
//		return this->mGrid->GetRepeat () ? BOUNDS_GLOBAL : BOUNDS_OK;
//	}
//	else if ( this->mDeck ) {
//	
//		bounds = this->mDeck->GetBounds ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ));
//		return BOUNDS_OK;
//	}
//	
//	return BOUNDS_EMPTY;
//}
