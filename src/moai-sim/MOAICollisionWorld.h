// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAIRenderable.h>

class MOAICollisionProp;
class MOAIOverlapInfo;
class MOAIPartition;
class MOAIProp;

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public MOAIAction,
	public MOAIRenderable {
private:

	friend class MOAICollisionProp;

	bool	mUpdated;
	u32		mOverlapPass;

	typedef ZLLeanList < MOAICollisionProp* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionProp* > mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* > mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap > mOverlapPool;

	MOAILuaSharedPtr < MOAIPartition > mPartition;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_insertProp				( lua_State* L );
	static int			_processOverlaps		( lua_State* L );
	static int			_setCallback			( lua_State* L );
	static int			_setPartition			( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo );
	void				ClearOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				ClearOverlaps			( MOAICollisionProp& prop );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo );
	void				DrawCollisionProp		( MOAICollisionProp& prop );
	void				InsertProp				( MOAICollisionProp& prop );
	bool				IsDone					();
	void				InvalidateOverlaps		( MOAICollisionProp& prop, u32 nextPass );
	void				MakeActive				( MOAICollisionProp& prop0 );
	void				MakeInactive			( MOAICollisionProp& prop0 );
	void				OnPropInserted			( MOAIProp& prop );	
	void				OnPropRemoved			( MOAIProp& prop );	
	void				OnPropUpdated			( MOAIProp& prop );
	void				OnUpdate				( double step );
	void				ProcessOverlaps			();
	void				PruneOverlaps			( MOAICollisionProp& prop );
	void				Render					();
	void				RemoveProp				( MOAICollisionProp& prop );

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	enum {
		OVERLAP_BEGIN,
		OVERLAP_END,
		OVERLAP_UPDATE,
	};
	
	enum {
		OVERLAP_PASS_INIT	= 0,
	};
	
	//----------------------------------------------------------------//
					MOAICollisionWorld		();
					~MOAICollisionWorld		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
