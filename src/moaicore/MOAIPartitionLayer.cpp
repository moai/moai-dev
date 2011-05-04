// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// MOAIPartitionLayer
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionLayer::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, u32 mask ) {

	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherProps ( partition, ignore, mask );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLayer::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, USVec2D point, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;
	point.mX = point.mX - halfSize;
	point.mY = point.mY - halfSize;

	USCellCoord coord = this->mGridSpace.GetCellCoord ( point );
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetWidth ();
	
	int xTotal = ( width < 2 ) ? 1 : 2;
	int yTotal = ( height < 2 ) ? 1 : 2;
	
	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
		
			USCellCoord offset = coord.MakeOffset ( x, y );
			int addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherProps ( partition, ignore, point, mask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLayer::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, USRect rect, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;

	rect.Bless ();
	USCellCoord coord = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMax + halfSize );

	int xTotal = ( int )( rect.Width () / this->mGridSpace.GetCellWidth ()) + 2;
	int yTotal = ( int )( rect.Height () / this->mGridSpace.GetCellHeight ()) + 2;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
		
			USCellCoord offset = coord.MakeOffset ( x, -y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherProps ( partition, ignore, rect, mask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLayer::Init ( float cellSize, u32 width, u32 height ) {

	this->mCellSize = cellSize;

	this->mGridSpace.SetWidth ( width );
	this->mGridSpace.SetHeight ( height );
	this->mGridSpace.SetCellWidth ( cellSize );
	this->mGridSpace.SetCellHeight ( cellSize );
	
	this->mCells.Init ( this->mGridSpace.GetTotalCells ());
}

//----------------------------------------------------------------//
MOAIPartitionCell* MOAIPartitionLayer::GetCell ( MOAIProp& prop ) {

	USVec2D loc;
	prop.mBounds.GetCenter ( loc );
	
	USCellCoord coord = this->mGridSpace.GetCellCoord ( loc );
	u32 cellAddr = this->mGridSpace.GetCellAddr ( coord );
	return &this->mCells [ cellAddr ];
}

//----------------------------------------------------------------//
void MOAIPartitionLayer::PlaceProp ( MOAIProp& prop ) {

	USVec2D loc;
	prop.mBounds.GetCenter ( loc );
	USCellCoord coord = this->mGridSpace.GetCellCoord ( loc );

	//printf ( "cell %d ( %d, %d )\n", this->mGridSpace.GetTileAddr ( coord ), coord.mX, coord.mY );

	MOAIPartitionCell* cell = this->GetCell ( prop );
	cell->InsertProp ( prop );
}

//----------------------------------------------------------------//
MOAIPartitionLayer::MOAIPartitionLayer () :
	mCellSize ( 0.0f ) {
}

//----------------------------------------------------------------//
MOAIPartitionLayer::~MOAIPartitionLayer () {
}
