// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGridPathGraph.h>
#include <moaicore/MOAIPathTerrainDeck.h>

//================================================================//
// MOAIGridPathGraphParams
//================================================================//
class MOAIGridPathGraphParams {
public:

	float mHCost;
	float mVCost;
	float mDCost;
	float mZCost;

	float mGWeight;
	float mHWeight;
	
	u32 mHeuristic;
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setGrid
	@text	Set graph data to use for pathfinding. 

	@in		MOAIGridPathGraph self
	@opt	MOAIGrid grid					Default value is nil.
	@out	nil
*/
int MOAIGridPathGraph::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridPathGraph, "U" )

	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	self->SetGrid ( grid );
	
	return 0;
}

//================================================================//
// MOAIGridPathGraph
//================================================================//

//----------------------------------------------------------------//
float MOAIGridPathGraph::ComputeHeuristic ( MOAIGridPathGraphParams& params, const MOAICellCoord& c0, const MOAICellCoord& c1 ) {

	float hMove = ( float )abs ( c1.mX - c0.mX );
	float vMove = ( float )abs ( c1.mY - c0.mY );

	switch ( params.mHeuristic ) {
		
		case MANHATTAN_DISTANCE:
		
			return ( hMove * params.mHCost ) + ( vMove * params.mVCost );
		
		case DIAGONAL_DISTANCE:
			
			if ( hMove < vMove ) {
				return ( hMove * params.mDCost ) + (( vMove - hMove ) * params.mVCost );
			}
			return ( vMove * params.mDCost ) + (( hMove - vMove ) * params.mHCost );
		
		case EUCLIDEAN_DISTANCE: {
			
			USVec2D v0 = this->mGrid->GetCellPoint ( c0, MOAIGridSpace::TILE_CENTER );
			USVec2D v1 = this->mGrid->GetCellPoint ( c1, MOAIGridSpace::TILE_CENTER );
			
			return v0.Dist ( v1 );
		}
	};
	return 0.0f;
}

//----------------------------------------------------------------//
MOAIGridPathGraph::MOAIGridPathGraph () {
	
	RTTI_SINGLE ( MOAIGridPathGraph )
}

//----------------------------------------------------------------//
MOAIGridPathGraph::~MOAIGridPathGraph () {

	this->SetGrid ( 0 );
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::PushNeighbor ( MOAIPathFinder& pathFinder, MOAIGridPathGraphParams& params, u32 tile0, int xTile, int yTile, float moveCost ) {

	MOAICellCoord coord = this->mGrid->GetCellCoord ( xTile, yTile );

	if ( this->mGrid->IsValidCoord ( coord )) {
		
		u32 tile1 = this->mGrid->GetTile ( xTile, yTile );
		
		if ( pathFinder.CheckMask ( tile1 )) {
			
			int neighborID = this->mGrid->GetCellAddr ( coord );
			
			if ( !pathFinder.IsVisited ( neighborID )) {
				
				float g = ( moveCost + pathFinder.ComputeTerrainCost ( moveCost, tile0, tile1 )) * params.mGWeight;
				
				int targetID = pathFinder.GetTargetNodeID ();
				MOAICellCoord targetCoord = this->mGrid->GetCellCoord ( targetID );
				
				float h = this->ComputeHeuristic ( params, coord, targetCoord ) * params.mHWeight;
				
				pathFinder.PushState ( neighborID, g + h );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::PushNeighbors ( MOAIPathFinder& pathFinder, int nodeID ) {
	
	if ( !this->mGrid ) return;
	
	MOAIGridPathGraphParams params;
	
	params.mGWeight = pathFinder.GetGWeight ();
	params.mHWeight = pathFinder.GetHWeight ();
	
	params.mHeuristic = pathFinder.GetHeuristic ();
	
	u32 flags = pathFinder.GetFlags ();
	
	MOAICellCoord coord = this->mGrid->GetCellCoord ( nodeID );
	
	int xTile = coord.mX;
	int yTile = coord.mY;
	
	u32 tile0 = this->mGrid->GetTile ( xTile, yTile );
	
	switch ( this->mGrid->GetShape ()) {
	
		case MOAIGridSpace::RECT_SHAPE: {
			
			params.mHCost = this->mGrid->GetCellWidth ();
			params.mVCost = this->mGrid->GetCellHeight ();
			params.mDCost = sqrtf (( params.mHCost * params.mHCost ) + ( params.mVCost * params.mVCost ));
			params.mZCost = 0.0f;
			
			this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile, params.mHCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile, params.mHCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile + 1, params.mVCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile - 1, params.mVCost );
			
			if (( flags & NO_DIAGONALS ) == 0 ) {
				
				this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile - 1, params.mDCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile + 1, params.mDCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile - 1, params.mDCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile + 1, params.mDCost );
			}
			break;
		}
		case MOAIGridSpace::DIAMOND_SHAPE: {
			
			params.mHCost = this->mGrid->GetCellWidth ();
			params.mVCost = this->mGrid->GetCellHeight () * 2.0f;
			params.mDCost = sqrtf (( params.mHCost * params.mHCost ) + ( params.mVCost * params.mVCost )) * 0.5f;
			params.mZCost = 0.0f;
		
			int stepRight = 0;
			int stepLeft = -1;
			
			// need to offset x into the previous tile if y is odd
			if ( yTile & 0x01 ) {
				stepRight = 1;
				stepLeft = 0;
			}
			
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepLeft,	yTile - 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepRight,	yTile - 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepLeft,	yTile + 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepRight,	yTile + 1, params.mDCost );
			
			if (( flags & NO_DIAGONALS ) == 0 ) {
			
				this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile, params.mHCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile, params.mHCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile - 2, params.mVCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile + 2, params.mVCost );
			}
			break;
		}
		case MOAIGridSpace::OBLIQUE_SHAPE: {
			
			params.mHCost = this->mGrid->GetCellWidth ();
			params.mVCost = this->mGrid->GetCellHeight ();
			params.mDCost = sqrtf (( params.mHCost * params.mHCost ) + ( params.mVCost * params.mVCost ));
			params.mZCost = sqrtf (( params.mHCost * params.mHCost * 0.4f ) + ( params.mVCost * params.mVCost ));
			
			this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile, params.mHCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile, params.mHCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile - 1, yTile - 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + 1, yTile + 1, params.mDCost );
			
			if (( flags & NO_DIAGONALS ) == 0 ) {
				
				this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile - 1, params.mVCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile + 1, params.mVCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile - 2, yTile - 1, params.mZCost );
				this->PushNeighbor ( pathFinder, params, tile0, xTile + 2, yTile + 1, params.mZCost );
			}
			break;
		}
		case MOAIGridSpace::HEX_SHAPE: {
			
			params.mHCost = this->mGrid->GetCellWidth ();
			params.mVCost = this->mGrid->GetCellHeight () * 2.0f;
			params.mDCost = sqrtf (( params.mHCost * params.mHCost ) + ( params.mVCost * params.mVCost )) * 0.5f;
			params.mZCost = 0.0f;
			
			int stepRight = 0;
			int stepLeft = -1;
			
			// need to offset x into the previous tile if y is odd
			if ( yTile & 0x01 ) {
				stepRight = 1;
				stepLeft = 0;
			}
			
			this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile - 2, params.mVCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile, yTile + 2, params.mVCost );
			
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepLeft,	yTile - 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepRight,	yTile - 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepLeft,	yTile + 1, params.mDCost );
			this->PushNeighbor ( pathFinder, params, tile0, xTile + stepRight,	yTile + 1, params.mDCost );
			
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "MANHATTAN_DISTANCE", ( u32 )MANHATTAN_DISTANCE );
	state.SetField ( -1, "DIAGONAL_DISTANCE", ( u32 )DIAGONAL_DISTANCE );
	state.SetField ( -1, "EUCLIDEAN_DISTANCE", ( u32 )EUCLIDEAN_DISTANCE );
	
	state.SetField ( -1, "NO_DIAGONALS", ( u32 )NO_DIAGONALS );
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIPathGraph::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setGrid",			_setGrid },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::SetGrid ( MOAIGrid* grid ) {

	this->mGrid.Set ( *this, grid );
}
