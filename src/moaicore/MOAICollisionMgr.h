//[MOAI LICENSE]

#ifndef	MOAICOLLISIONMGR_H
#define	MOAICOLLISIONMGR_H

#include <moaicore/MOAIOverlapPrim2D.h>

class MOAIOverlapLink2D;
class MOAIOverlapPrim2D;

//================================================================//
// MOAICollisionMgr
//================================================================//
class MOAICollisionMgr :
	public USGlobalClass < MOAICollisionMgr > {
private:

	enum {
		BLOCK_SIZE		= 64,
		MAX_BLOCKS		= 64,
	};

	USLeanArray < MOAIOverlapLink2D* > mBlocks;
	
	MOAIOverlapLink2D*	mFreeLinks;
	MOAIOverlapPrim2D*	mOverlapPrims;

	//----------------------------------------------------------------//
	MOAIOverlapLink2D*		AllocLink				();
	void					FreeLink				( MOAIOverlapLink2D* link );
	void					MoreLinks				();
	void					ProcessOverlaps			( MOAIOverlapPrim2D& overlapPrim );

public:
	
	friend class MOAIOverlapPrim2D;
	
	//----------------------------------------------------------------//
							MOAICollisionMgr		();
							~MOAICollisionMgr		();
	void					ScheduleOverlap			( MOAIOverlapPrim2D& overlapPrim );
	void					Update					();
};

#endif
