// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONLEVEL_H
#define	MOAIPARTITIONLEVEL_H

#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIPartitionCell.h>

//================================================================//
// MOAIPartitionLevel
//================================================================//
class MOAIPartitionLevel {
private:
	
	MOAIGridSpace							mGridSpace;
	ZLLeanArray < MOAIPartitionCell >		mCells;
	float									mCellSize;

	//----------------------------------------------------------------//
	void					Clear				();
	void					ExtractProps		( MOAIPartitionCell& cell, MOAIPartitionLevel* level );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, u32 interfaceMask, u32 queryMask );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLVec3D& point, u32 planeID, u32 interfaceMask, u32 queryMask );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLRect& rect, u32 interfaceMask, u32 queryMask );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLBox& box, u32 planeID, u32 interfaceMask, u32 queryMask );
	void					GatherHulls			( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLFrustum& frustum, u32 planeID, u32 interfaceMask, u32 queryMask );
	MOAIPartitionCell*		GetCell				( MOAIPartitionHull& hull );
	void					Init				( float cellSize, u32 width, u32 height );
	void					PlaceHull			( MOAIPartitionHull& hull );

public:

	friend class MOAIPartition;
	friend class MOAIPartitionHull;

	//----------------------------------------------------------------//
					MOAIPartitionLevel	();
					~MOAIPartitionLevel	();
};

#endif
