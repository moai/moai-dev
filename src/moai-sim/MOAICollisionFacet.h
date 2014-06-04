// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONFACET_H
#define	MOAICOLLISIONFACET_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

class MOAICollisionShape;
class MOAICollisionFacet;
class MOAIPropOverlap;

//================================================================//
// MOAIPropOverlapLink
//================================================================//
class MOAIPropOverlapLink {
private:

	friend class MOAICollisionFacet;
	friend class MOAICollisionWorld;

	MOAICollisionFacet*		mOther;
	MOAIPropOverlap*		mOverlap;
	MOAIPropOverlapLink*	mNext;
};

//================================================================//
// MOAIPropOverlap
//================================================================//
class MOAIPropOverlap {
private:

	friend class MOAICollisionFacet;
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

	friend class MOAICollisionFacet;
	friend class MOAICollisionWorld;

	ZLVec3D		mCenter;
	ZLBox		mBounds;
	bool		mHasBounds;
};

//================================================================//
// MOAICollisionFacet
//================================================================//
// TODO: doxygen
class MOAICollisionFacet:
	public MOAIFacet {
private:
	
	friend class MOAICollisionWorld;
	
	u32									mGroupMask;			// collisions get filtered by bitwise AND
	u32									mOverlapFlags;
	u32									mOverlapPass;		// used to identify if prop has been processed in current cycle
	MOAIPropOverlapLink*				mOverlapLinks;		// singly-linked list of links to overlaps with this prop (if caching)
	
	ZLLeanLink < MOAICollisionFacet* >	mActiveListLink;	// link in collision world's list of props with overlaps or in need of update
	
	bool								mInDrawList;
	MOAICollisionFacet*					mNextInDrawList;
	
	bool								mStayActive;
	u32									mTouched;
	
	//----------------------------------------------------------------//
	static int				_getOverlaps			( lua_State* L );
	static int				_hasOverlaps			( lua_State* L );
	static int				_setGroupMask			( lua_State* L );
	static int				_setOverlapFlags		( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ClearOverlapLink		( MOAICollisionFacet& other );
	MOAICollisionFacet*		GetCollisionProp		();
	bool					IsActive				();
	void					OnAttach				( MOAIProp& prop );
	void					OnDetach				( MOAIProp& prop );
	
public:

	DECL_LUA_FACTORY ( MOAICollisionFacet )

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
							MOAICollisionFacet		();
	virtual					~MOAICollisionFacet		();
	bool					RefineOverlap			( const MOAICollisionFacet& other, MOAIOverlapInfo& info ) const;
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
