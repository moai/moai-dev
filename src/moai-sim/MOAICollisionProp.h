// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONPROP_H
#define	MOAICOLLISIONPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>

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

	ZLBox					mBounds;
	MOAIPropOverlapLink		mLeft;
	MOAIPropOverlapLink		mRight;
	
	ZLLeanLink < MOAIPropOverlap* >	mOverlapListLink;
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
		OVERLAP_EVENTS_CONTINUOUS		= 0x01,		// will *not* create overlap links and will receive continuous status events
		OVERLAP_EVENTS_LIFECYCLE		= 0x02,		// will create overlap links and receive begin/end events
	};

	static const u32 DEFAULT_OVERLAP_FLAGS			= 0;

	//----------------------------------------------------------------//
						MOAICollisionProp		();
	virtual				~MOAICollisionProp		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
