// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONFACET_H
#define	MOAICOLLISIONFACET_H

#include <moai-sim/MOAIIndexedPropBase.h>
#include <moai-sim/MOAIPartitionHull.h>

class MOAICollisionShape;
class MOAICollisionProp;
class MOAICollisionWorld;
class MOAIPropOverlap;

//================================================================//
// MOAIPropOverlapLink
//================================================================//
class MOAIPropOverlapLink {
private:

	friend class MOAICollisionProp;
	friend class MOAICollisionWorld;

	MOAICollisionProp*		mOther;
	MOAIPropOverlap*		mOverlap;
	MOAIPropOverlapLink*	mNext;
};

//================================================================//
// MOAIPropOverlap
//================================================================//
class MOAIPropOverlap {
private:

	friend class MOAICollisionProp;
	friend class MOAICollisionWorld;

	MOAIPropOverlapLink		mLeft;
	MOAIPropOverlapLink		mRight;
	
	bool					mIsValid;
	
	ZLLeanLink < MOAIPropOverlap* >	mOverlapListLink;
};

//================================================================//
// MOAIOverlapInfo
//================================================================//
class MOAIOverlapInfo {
private:

	friend class MOAICollisionProp;
	friend class MOAICollisionWorld;

	ZLVec3D		mCenter;
	ZLBox		mBounds;
	bool		mHasBounds;
};

//================================================================//
// MOAICollisionProp
//================================================================//
// TODO: doxygen
class MOAICollisionProp :
	public MOAIPartitionHull,
	public MOAIIndexedPropBase {
private:
	
	friend class MOAICollisionWorld;
	
	u32									mGroupMask;			// collisions get filtered by bitwise AND
	u32									mOverlapFlags;
	u32									mOverlapPass;		// used to identify if prop has been processed in current cycle
	MOAIPropOverlapLink*				mOverlapLinks;		// singly-linked list of links to overlaps with this prop (if caching)
	
	ZLLeanLink < MOAICollisionProp* >	mActiveListLink;	// link in collision world's list of props with overlaps or in need of update
	
	bool								mInDrawList;
	MOAICollisionProp*					mNextInDrawList;
	
	bool								mStayActive;
	u32									mTouched;
	
	MOAICollisionWorld*					mCollisionWorld;
	
	//----------------------------------------------------------------//
	static int				_getOverlaps			( lua_State* L );
	static int				_hasOverlaps			( lua_State* L );
	static int				_setGroupMask			( lua_State* L );
	static int				_setOverlapFlags		( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ClearOverlapLink		( MOAICollisionProp& other );
	bool					IsActive				();
	
	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp						( u32 attrID, MOAIAttribute& attr, u32 op );
	void					MOAINode_Update								();
	void					MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32						MOAIPartitionHull_AffirmInterfaceMask		( MOAIPartition& partition );
	void					MOAIPartitionHull_BoundsDidChange			();
	ZLBounds				MOAIPartitionHull_GetModelBounds			();
	bool					MOAIPartitionHull_PrepareForInsertion		( const MOAIPartition& partition );
	void					MOAIPartitionHull_WasRemovedFromPartition	();
	
public:

	enum {
		DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS,
		DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS,
		DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS,
		DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS,
		TOTAL_DEBUG_LINE_STYLES,
	};

	DECL_LUA_FACTORY ( MOAICollisionProp )

	static const u32 OVERLAP_EVENTS_ON_UPDATE		= 0x01;		// will send overlap update events
	static const u32 OVERLAP_EVENTS_CONTINUOUS		= 0x02;		// both props will stay active as long as there's an overlap
	static const u32 OVERLAP_EVENTS_LIFECYCLE		= 0x04;		// will create overlap links and receive begin/end events
	static const u32 OVERLAP_GRANULARITY_FINE		= 0x08;		// will use higher resolution primitives if available
	static const u32 OVERLAP_CALCULATE_BOUNDS		= 0x10;		// will calculate bounds instead of just a point between both props

	static const u32 GROUP_MASK_ALL					= 0xffffffff;

	static const u32 DEFAULT_OVERLAP_FLAGS			= 0;

	//----------------------------------------------------------------//
	void					DrawDebug				();
	MOAICollisionShape*		GetCollisionShape		();
							MOAICollisionProp		();
	virtual					~MOAICollisionProp		();
	bool					RefineOverlap			( MOAICollisionProp& other, MOAIOverlapInfo& info );
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
