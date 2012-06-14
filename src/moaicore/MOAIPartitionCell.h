// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONCELL_H
#define	MOAIPARTITIONCELL_H

class MOAIPartition;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIProp;

//================================================================//
// MOAIPartitionCell
//================================================================//
class MOAIPartitionCell {
private:
	
	friend class MOAIPartition;
	friend class MOAIPartitionLevel;
	friend class MOAIProp;
	
	typedef USLeanList < MOAIProp* >::Iterator PropIt;
	USLeanList < MOAIProp* > mProps;

	//----------------------------------------------------------------//
	void			Clear					();
	void			ExtractProps			( MOAIPartitionCell& cell, MOAIPartitionLevel* layer );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, u32 mask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USBox& box, u32 mask );
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
