// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 interfaceMask, u32 queryMask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLVec3D& point, u32 planeID, u32 interfaceMask, u32 queryMask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLBox& box, u32 planeID, u32 interfaceMask, u32 queryMask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const ZLFrustum& frustum, u32 planeID, u32 interfaceMask, u32 queryMask );
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
