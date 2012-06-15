// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONLEVEL_H
#define	MOAIPARTITIONLEVEL_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIPartitionCell.h>

//================================================================//
// MOAIPartitionLevel
//================================================================//
class MOAIPartitionLevel {
private:
	
	MOAIGridSpace							mGridSpace;
	USLeanArray < MOAIPartitionCell >		mCells;
	float									mCellSize;

	//----------------------------------------------------------------//
	void					Clear				();
	void					ExtractProps		( MOAIPartitionCell& cell, MOAIPartitionLevel* layer );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USVec3D& point, u32 planeID, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USBox& box, u32 planeID, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 planeID, u32 mask );
	MOAIPartitionCell*		GetCell				( MOAIProp& prop );
	void					Init				( float cellSize, u32 width, u32 height );
	void					PlaceProp			( MOAIProp& prop );

public:

	friend class MOAIPartition;
	friend class MOAIProp;

	//----------------------------------------------------------------//
					MOAIPartitionLevel	();
					~MOAIPartitionLevel	();
};

#endif
