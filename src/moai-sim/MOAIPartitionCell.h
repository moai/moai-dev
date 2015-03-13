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
	
	typedef ZLLeanList < MOAIProp* >::Iterator PropIt;
	ZLLeanList < MOAIProp* > mProps;

	//----------------------------------------------------------------//
	void			Clear					();
	void			ExtractProps			( MOAIPartitionCell& cell, MOAIPartitionLevel* level );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 interfaceMask, u32 queryMask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLVec3D& point, u32 interfaceMask, u32 queryMask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLBox& box, u32 interfaceMask, u32 queryMask );
	void			GatherProps				( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLFrustum& frustum, u32 interfaceMask, u32 queryMask );
	void			InsertProp				( MOAIProp& prop );
	void			RemoveProp				( MOAIProp& prop );
	void			ScheduleProps			(); // schedule all props in cell for update
					
public:

	//----------------------------------------------------------------//
					MOAIPartitionCell		();
					~MOAIPartitionCell		();
};

#endif
