// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONCELL_H
#define	MOAIPARTITIONCELL_H

class MOAIPartition;
class MOAIPartitionLayer;
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
	void			GatherProps				( MOAIPartition& partition, MOAIProp* ignore, u32 mask );
	void			GatherProps				( MOAIPartition& partition, MOAIProp* ignore, USVec2D& point, u32 mask );
	void			GatherProps				( MOAIPartition& partition, MOAIProp* ignore, USRect& rect, u32 mask );
	void			InsertProp				( MOAIProp& prop );
	void			RemoveProp				( MOAIProp& prop );
					
public:

	//----------------------------------------------------------------//
					MOAIPartitionCell		();
					~MOAIPartitionCell		();
};

#endif
