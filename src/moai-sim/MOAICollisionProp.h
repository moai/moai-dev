// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONPROP_H
#define	MOAICOLLISIONPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

class MOAICollisionShape;
class MOAICollisionProp;
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
	public MOAIProp {
private:
	
	friend class MOAICollisionWorld;
	
	u32									mOverlapFlags;
	u32									mOverlapPass; // used to identify if prop has been processed in current cycle
	MOAIPropOverlapLink*				mOverlapLinks; // singly-linked list of links to overlaps with this prop (if caching)
	
	ZLLeanLink < MOAICollisionProp* >	mActiveListLink; // link in collision world's list of props with overlaps or in need of update
	
	//----------------------------------------------------------------//
	static int			_setOverlapFlags		( lua_State* L );
	
	//----------------------------------------------------------------//
	void				ClearOverlapLink		( MOAICollisionProp& other );
	MOAICollisionProp*	GetCollisionProp		();
	
public:

	DECL_LUA_FACTORY ( MOAICollisionProp )

	enum {
		OVERLAP_EVENTS_ON_UPDATE		= 0x01,		// will send overlap update events
		OVERLAP_EVENTS_LIFECYCLE		= 0x02,		// will create overlap links and receive begin/end events
		OVERLAP_GRANULARITY_FINE		= 0x04,		// will use higher resolution primitives if available
		OVERLAP_CALCULATE_BOUNDS		= 0x08,		// will calculate bounds instead of just a point between both props
	};

	static const u32 DEFAULT_OVERLAP_FLAGS	= 0;

	//----------------------------------------------------------------//
						MOAICollisionProp		();
	virtual				~MOAICollisionProp		();
	bool				RefineOverlap			( const MOAICollisionProp& other, MOAIOverlapInfo& info ) const;
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
