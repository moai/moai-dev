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
	
	typedef USLeanList < MOAIProp* >::Iterator PropIt;
	USLeanList < MOAIProp* > mProps;

	//----------------------------------------------------------------//
	void			Clear					();
	void			ExtractProps			( USLeanList < MOAIProp* >& props );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USRect& rect, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USFrustum& frustum, u32 mask );
	void			InsertProp				( MOAIProp& prop );
	void			RemoveProp				( MOAIProp& prop );
	void			ScheduleProps			(); // schedule all props in cell for update
					
public:

	//----------------------------------------------------------------//
					MOAIPartitionCell		();
					~MOAIPartitionCell		();
};

#endif
