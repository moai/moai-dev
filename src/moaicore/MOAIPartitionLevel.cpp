// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionLevel.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// MOAIPartitionLevel
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionLevel::Clear () {

	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::ExtractProps ( MOAIPartitionCell& cell, MOAIPartitionLevel* layer ) {

	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].ExtractProps ( cell, layer );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask ) {

	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherProps ( results, ignore, mask );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask ) {
	
	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherProps ( results, ignore, point, orientation, mask );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, u32 planeID, u32 mask ) {

	USVec2D cellPoint ( 0.0f, 0.0f );
	
	switch ( planeID ) {
		case USBox::PLANE_XY:
			cellPoint.Init ( point.mX, point.mY );
			break;
		case USBox::PLANE_XZ:
			cellPoint.Init ( point.mX, point.mZ );
			break;
		case USBox::PLANE_YZ:
			cellPoint.Init ( point.mY, point.mZ );
			break;
	};

	float halfSize = this->mCellSize * 0.5f;
	cellPoint.mX = cellPoint.mX - halfSize;
	cellPoint.mY = cellPoint.mY + halfSize;

	MOAICellCoord coord = this->mGridSpace.GetCellCoord ( cellPoint.mX, cellPoint.mY );
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	int xTotal = ( width < 2 ) ? 1 : 2;
	int yTotal = ( height < 2 ) ? 1 : 2;
	
	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord.mX + x, coord.mY - y );
			int addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherProps ( results, ignore, point, mask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USBox& box, u32 planeID, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;

	USRect rect = box.GetRect ( planeID );
	MOAICellCoord coord0 = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMin - halfSize );
	MOAICellCoord coord1 = this->mGridSpace.GetCellCoord ( rect.mXMax + halfSize, rect.mYMax + halfSize );

	int xTotal = coord1.mX - coord0.mX + 1;
	int yTotal = coord1.mY - coord0.mY + 1;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord0.mX + x, coord0.mY + y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherProps ( results, ignore, box, mask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherProps ( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 planeID, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;

	USRect rect = frustum.mAABB.GetRect ( planeID );
	MOAICellCoord coord0 = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMin - halfSize );
	MOAICellCoord coord1 = this->mGridSpace.GetCellCoord ( rect.mXMax + halfSize, rect.mYMax + halfSize );

	int xTotal = coord1.mX - coord0.mX + 1;
	int yTotal = coord1.mY - coord0.mY + 1;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord0.mX + x, coord0.mY + y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherProps ( results, ignore, frustum, mask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::Init ( float cellSize, u32 width, u32 height ) {

	this->mCellSize = cellSize;

	this->mGridSpace.SetWidth ( width );
	this->mGridSpace.SetHeight ( height );
	this->mGridSpace.SetCellWidth ( cellSize );
	this->mGridSpace.SetCellHeight ( cellSize );
	
	this->mCells.Init ( this->mGridSpace.GetTotalCells ());
}

//----------------------------------------------------------------//
MOAIPartitionCell* MOAIPartitionLevel::GetCell ( MOAIProp& prop ) {

	USVec3D loc;
	prop.mBounds.GetCenter ( loc );
	
	MOAICellCoord coord = this->mGridSpace.GetCellCoord ( loc.mX, loc.mY );
	u32 cellAddr = this->mGridSpace.GetCellAddr ( coord );
	return &this->mCells [ cellAddr ];
}

//----------------------------------------------------------------//
MOAIPartitionLevel::MOAIPartitionLevel () :
	mCellSize ( 0.0f ) {
}

//----------------------------------------------------------------//
MOAIPartitionLevel::~MOAIPartitionLevel () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::PlaceProp ( MOAIProp& prop ) {

	//USVec3D loc;
	//prop.mBounds.GetCenter ( loc );
	//MOAICellCoord coord = this->mGridSpace.GetCellCoord ( loc.mX, loc.mY );
	//printf ( "cell %d ( %d, %d )\n", this->mGridSpace.GetCellAddr ( coord ), coord.mX, coord.mY );

	MOAIPartitionCell* cell = this->GetCell ( prop );
	cell->InsertProp ( prop );
}
