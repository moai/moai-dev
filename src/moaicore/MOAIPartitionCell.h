// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONCELL_H
#define	MOAIPARTITIONCELL_H

class MOAIPartition;
class MOAIPartitionLayer;
class MOAIPartitionResultBuffer;
class MOAIProp;

//================================================================//
// MOAIPartitionCell
//================================================================//
class MOAIPartitionCell {
private:
	
	friend class MOAIPartition;
	friend class MOAIPartitionLayer;
	friend class MOAIProp;
	
	typedef USLeanList < MOAIProp* >::Iterator PrimIt;
	USLeanList < MOAIProp* > mPrims;

	//----------------------------------------------------------------//
	void			GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USVec2D& point, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, MOAIProp* ignore, USRect& rect, u32 mask );
	void			InsertProp				( MOAIProp& prop );
	void			RemoveProp				( MOAIProp& prop );
					
public:

	//----------------------------------------------------------------//
					MOAIPartitionCell		();
					~MOAIPartitionCell		();
};

#endif
