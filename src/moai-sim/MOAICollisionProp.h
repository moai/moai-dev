// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONFACET_H
#define	MOAICOLLISIONFACET_H

#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAITransform.h>

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
class MOAICollisionProp:
	public MOAIProp {
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
	u32						AffirmInterfaceMask		( MOAIPartition& partition );
	void					ClearOverlapLink		( MOAICollisionProp& other );
	bool					IsActive				();
	void					OnBoundsChanged			();
	void					OnRemoved				();
	bool					PrepareForInsertion		( const MOAIPartition& partition );
	
public:

	DECL_LUA_FACTORY ( MOAICollisionProp )

	enum {
		OVERLAP_EVENTS_ON_UPDATE		= 0x01,		// will send overlap update events
		OVERLAP_EVENTS_CONTINUOUS		= 0x02,		// both props will stay active as long as there's an overlap
		OVERLAP_EVENTS_LIFECYCLE		= 0x04,		// will create overlap links and receive begin/end events
		OVERLAP_GRANULARITY_FINE		= 0x08,		// will use higher resolution primitives if available
		OVERLAP_CALCULATE_BOUNDS		= 0x10,		// will calculate bounds instead of just a point between both props
	};
	
	enum {
		GROUP_MASK_ALL		= 0xffffffff,
	};

	static const u32 DEFAULT_OVERLAP_FLAGS	= 0;

	//----------------------------------------------------------------//
							MOAICollisionProp		();
	virtual					~MOAICollisionProp		();
	bool					RefineOverlap			( const MOAICollisionProp& other, MOAIOverlapInfo& info ) const;
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
