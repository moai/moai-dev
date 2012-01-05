// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONLAYER_H
#define	MOAIPARTITIONLAYER_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIPartitionCell.h>

//================================================================//
// MOAIPartitionLayer
//================================================================//
class MOAIPartitionLayer {
private:
	
	MOAIGridSpace							mGridSpace;
	USLeanArray < MOAIPartitionCell >		mCells;
	float									mCellSize;

	//----------------------------------------------------------------//
	void					Clear				();
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USVec2D point, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USRect rect, u32 mask );
	void					GatherProps			( MOAIPartitionResultBuffer& results, MOAIProp* ignore, const USFrustum& frustum, u32 mask );
	MOAIPartitionCell*		GetCell				( MOAIProp& prop );
	void					Init				( float cellSize, u32 width, u32 height );
	void					PlaceProp			( MOAIProp& prop );

public:

	friend class MOAIPartition;
	friend class MOAIProp;

	//----------------------------------------------------------------//
					MOAIPartitionLayer	();
					~MOAIPartitionLayer	();
};

#endif
