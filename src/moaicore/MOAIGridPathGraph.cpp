// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGridPathGraph.h>
#include <moaicore/MOAIPathTerrainDeck.h>

//================================================================//
// MOAIGridPathGraph
//================================================================//

//----------------------------------------------------------------//
MOAIGridPathGraph::MOAIGridPathGraph () :
	mGrid ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGridPathGraph::~MOAIGridPathGraph () {
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::PushNeighbor ( MOAIPathFinder& pathFinder, u32 tile0, int xTile, int yTile, float moveCost ) {

	MOAICellCoord coord = this->mGrid->GetCellCoord ( xTile, yTile );

	if ( this->mGrid->IsValidCoord ( coord )) {
		
		u32 tile1 = this->mGrid->GetTile ( xTile, yTile );
		
		if ( pathFinder.CheckMask ( tile1 )) {
			
			int neighborID = this->mGrid->GetCellAddr ( coord );
			
			if ( !pathFinder.IsVisited ( neighborID )) {
				
				float terrainCost = pathFinder.ComputeTerrainCost ( moveCost, tile0, tile1 );
				
				int targetID = pathFinder.GetTargetNodeID ();
				MOAICellCoord targetCoord = this->mGrid->GetCellCoord ( targetID );
				float heuristic = ( float )(( targetCoord.mX - coord.mX ) + ( targetCoord.mY - coord.mY ));
				
				pathFinder.PushState ( neighborID, moveCost + terrainCost + heuristic );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGridPathGraph::PushNeighbors ( MOAIPathFinder& pathFinder, int nodeID ) {

	if ( !this->mGrid ) return;

	float hMoveCost = this->mGrid->GetCellWidth ();
	float vMoveCost = this->mGrid->GetCellHeight ();
	float dMoveCost = sqrtf (( hMoveCost * hMoveCost ) + ( vMoveCost * vMoveCost ));

	MOAICellCoord coord = this->mGrid->GetCellCoord ( nodeID );

	int xTile = coord.mX;
	int yTile = coord.mY;
	
	u32 tile0 = this->mGrid->GetTile ( xTile, yTile );
	
	this->PushNeighbor ( pathFinder, tile0, xTile - 1, yTile, hMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile + 1, yTile, hMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile, yTile + 1, vMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile, yTile - 1, vMoveCost );
	
	this->PushNeighbor ( pathFinder, tile0, xTile - 1, yTile - 1, dMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile - 1, yTile + 1, dMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile + 1, yTile - 1, dMoveCost );
	this->PushNeighbor ( pathFinder, tile0, xTile + 1, yTile + 1, dMoveCost );
}

