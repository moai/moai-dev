// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAISurfaceDeck2D.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

SUPPRESS_EMPTY_FILE_WARNING

#if 0

//----------------------------------------------------------------//
//void MOAIProp::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveSurfaceLists
	@text	Reserve surface lists for deck.
	
	@in		MOAISurfaceDeck2D self
	@in		number nLists
	@out	nil
*/
int MOAISurfaceDeck2D::_reserveSurfaceLists ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mBrushes.Init ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveSurfaces
	@text	Reserve surfaces for a given list in deck.
	
	@in		MOAISurfaceDeck2D self
	@in		number idx
	@in		number nSurfaces
	@out	nil
*/
int MOAISurfaceDeck2D::_reserveSurfaces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UNN" )

	u32 brushID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total		= state.GetValue < u32 >( 3, 0 );
	
	if ( brushID < self->mBrushes.Size ()) {
		self->mBrushes [ brushID ].mEdges.Init ( total );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSurface
	@text	Set a surface in a surface list.
	
	@in		MOAISurfaceDeck2D self
	@in		number idx
	@in		number surfaceIdx
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAISurfaceDeck2D::_setSurface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UNNNNNN" )

	u32 brushID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 surfaceID	= state.GetValue < u32 >( 3, 1 ) - 1;
	
	if ( brushID < self->mBrushes.Size ()) {
		MOAISurfaceBrush2D& brush = self->mBrushes [ brushID ];
		if ( surfaceID < brush.mEdges.Size ()) {
		
			ZLEdge2D& edge = brush.mEdges [ surfaceID ];
			
			edge.mV0.mX		= state.GetValue < float >( 4, 0.0f );
			edge.mV0.mY		= state.GetValue < float >( 5, 0.0f );
			
			edge.mV1.mX		= state.GetValue < float >( 6, 0.0f );
			edge.mV1.mY		= state.GetValue < float >( 7, 0.0f );
			
			if ( surfaceID == 0 ) {
				brush.mBounds.Init ( edge.mV0 );
				brush.mBounds.Grow ( edge.mV1 );
			}
			else {
				brush.mBounds.Grow ( edge.mV0 );
				brush.mBounds.Grow ( edge.mV1 );
			}
		}
		self->SetBoundsDirty ();
	}
	return 0;
}

//================================================================//
// MOAISurfaceDeck2D
//================================================================//

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::DrawDebug ( const ZLAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
//	
//	idx = remapper ? remapper->Remap ( idx ) : idx;
//	
//	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
//	debugLines.SetWorldMtx ( transform );
//	debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
//	
//	this->DrawDebug ( idx, 0.0f, 0.0f, false, false );
//}

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::DrawDebug ( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip ) {
//
//	idx = idx - 1;
//	idx = idx % this->mBrushes.Size ();
//	
//	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
//	
//	MOAISurfaceBrush2D& brush = this->mBrushes [ idx ];
//	
//	u32 total = brush.mEdges.Size ();
//	for ( u32 i = 0; i < total; ++i ) {
//		ZLEdge2D& edge = brush.mEdges [ i ];
//		
//		ZLVec2D v0 = edge.mV0;
//		ZLVec2D v1 = edge.mV1;
//		
//		if ( xFlip ) {
//			v0.mX *= -1.0f;
//			v1.mX *= -1.0f;
//		}
//		
//		if ( !yFlip ) {
//			v0.mY *= -1.0f;
//			v1.mY *= -1.0f;
//		}
//		
//		debugLines.DrawLine ( v0.mX + xOff, v0.mY + yOff, v1.mX + xOff, v1.mY + yOff );
//	}
//}

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::DrawDebug ( const ZLAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
//	UNUSED ( gridScale ); // TODO
//	
//	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
//	debugLines.SetWorldMtx ( transform );
//	debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
//	
//	for ( int y = c0.mY; y <= c1.mY; ++y ) {
//		for ( int x = c0.mX; x <= c1.mX; ++x ) {
//			
//			u32 tile = grid.GetTile ( x, y );
//			tile = remapper ? remapper->Remap ( tile ) : tile;
//			
//			if ( tile & MOAITileFlags::HIDDEN ) continue;
//			
//			MOAICellCoord coord ( x, y );
//			ZLVec2D loc = grid.GetCellPoint ( coord, MOAIGridSpace::TILE_CENTER );
//			
//			bool xFlip = (( tile & MOAITileFlags::XFLIP ) != 0 );
//			bool yFlip = (( tile & MOAITileFlags::YFLIP ) != 0 );
//			
//			this->DrawDebug (( tile & MOAITileFlags::CODE_MASK ) - 1, loc.mX, loc.mY, xFlip, yFlip );
//		}
//	}
//}

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::GatherSurfaces ( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler ) {
//	
//	idx = remapper ? remapper->Remap ( idx ) : idx;
//	
//	idx = idx - 1;
//	idx = idx % this->mBrushes.Size ();
//	
//	MOAISurfaceBrush2D& brush = this->mBrushes [ idx ];
//
//	u32 total = brush.mEdges.Size ();
//	for ( u32 i = 0; i < total; ++i ) {
//		ZLEdge2D& edge = brush.mEdges [ i ];
//		
//		ZLVec2D v0 = edge.mV0;
//		ZLVec2D v1 = edge.mV1;
//		
//		sampler.AddSurfaceFromLocal ( v0, v1 );
//	}
//}

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::GatherSurfaces ( MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( gridScale ); // TODO
//
//	for ( int y = c0.mY; y <= c1.mY; ++y ) {
//		for ( int x = c0.mX; x <= c1.mX; ++x ) {
//			
//			u32 tile = grid.GetTile ( x, y );
//			tile = remapper ? remapper->Remap ( tile ) : tile;
//			
//			if ( tile & MOAITileFlags::HIDDEN ) continue;
//			
//			MOAICellCoord coord ( x, y );
//			ZLVec2D loc = grid.GetCellPoint ( coord, MOAIGridSpace::TILE_CENTER );
//			
//			bool xFlip = (( tile & MOAITileFlags::XFLIP ) != 0 );
//			bool yFlip = (( tile & MOAITileFlags::YFLIP ) != 0 );
//			
//			this->GatherSurfaces ( tile & MOAITileFlags::CODE_MASK, loc.mX, loc.mY, xFlip, yFlip, sampler );
//		}
//	}
//}

//----------------------------------------------------------------//
//void MOAISurfaceDeck2D::GatherSurfaces ( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler ) {
//
//	idx = idx % this->mBrushes.Size ();
//		
//	MOAISurfaceBrush2D& brush = this->mBrushes [ idx ];
//
//	u32 total = brush.mEdges.Size ();
//	for ( u32 i = 0; i < total; ++i ) {
//		ZLEdge2D& edge = brush.mEdges [ i ];
//		
//		ZLVec2D v0 = edge.mV0;
//		ZLVec2D v1 = edge.mV1;
//		
//		if ( xFlip ) {
//			v0.mX *= -1.0f;
//			v1.mX *= -1.0f;
//		}
//		
//		if ( !yFlip ) {
//			v0.mY *= -1.0f;
//			v1.mY *= -1.0f;
//		}
//		
//		v0.mX += xOff;
//		v0.mY += yOff;
//		
//		v1.mX += xOff;
//		v1.mY += yOff;
//		
//		if ( xFlip || yFlip ) {
//		
//			if ( xFlip && yFlip ) {
//				sampler.AddSurfaceFromLocal ( v0, v1 );
//			}
//			else {
//				sampler.AddSurfaceFromLocal ( v1, v0 );
//			}
//		
//		}
//		else {
//			sampler.AddSurfaceFromLocal ( v0, v1 );
//		}
//	}
//}

//----------------------------------------------------------------//
ZLBox MOAISurfaceDeck2D::GetItemAABB ( u32 idx ) {

	if ( idx < this->mBrushes.Size ()) {
		const ZLRect& rect = this->mBrushes [ idx ].mBounds;
		return ZLBox ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0, 0.0 );
	}
	return ZLBox::EMPTY;
}

//----------------------------------------------------------------//
MOAISurfaceDeck2D::MOAISurfaceDeck2D () {
	
	RTTI_BEGIN ( MOAISurfaceDeck2D )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAISurfaceDeck2D >)
		RTTI_EXTEND ( MOAIStandardDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISurfaceDeck2D::~MOAISurfaceDeck2D () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "reserveSurfaceLists",	_reserveSurfaceLists },
		{ "reserveSurfaces",		_reserveSurfaces },
		{ "setSurface",				_setSurface },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAISurfaceDeck2D::MOAIDeck_GetBounds () {

	size_t size = this->mBrushes.Size ();

	ZLRect frame;
	for ( size_t i = 0; i < size; ++i ) {
		frame.Grow ( this->mBrushes [ i ].mBounds, i > 0 );
	}

	ZLBox aabb ( frame.mXMin, frame.mYMax, frame.mXMax, frame.mYMin, 0.0f, 0.0f );
	return aabb;
}

//----------------------------------------------------------------//
ZLBounds MOAISurfaceDeck2D::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {
	UNUSED ( idx );
}

#endif
