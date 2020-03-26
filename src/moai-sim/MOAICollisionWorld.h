// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIPartition.h>

class MOAIPartitionHull;
class MOAICollisionWorld;

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public virtual MOAIAction,
	public virtual MOAIPartition {
protected:

	friend class MOAICollisionProp;
	friend class MOAIOverlapHandler;

	bool	mUpdated;
	u32		mOverlapPass;

	MOAIGfxScriptRetained					mDebugDraw;

	typedef ZLLeanList < MOAICollisionProp* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionProp* >		mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* >			mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap >			mOverlapPool;

	MOAILuaStrongRef						mCallback;

	//----------------------------------------------------------------//
	static int		_insertProp				( lua_State* L );
	static int		_processOverlaps		( lua_State* L );
	static int		_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void			AffirmOverlap			( MOAICollisionProp& prop0, u32 type0, MOAICollisionProp& prop1, u32 type1, const ZLBounds& bounds );
	void			ClearOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void			ClearOverlaps			( MOAICollisionProp& prop );
	void			DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void			DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const ZLBounds& bounds );
	void			HandleOverlap			( MOAICollisionProp& prop0, u32 type0, MOAICollisionProp& prop1, u32 type1, const ZLBounds& bounds );
	void			InvalidateOverlaps		( MOAICollisionProp& prop, u32 nextPass );
	void			MakeActive				( MOAICollisionProp& prop0 );
	void			MakeInactive			( MOAICollisionProp& prop0 );
	void			ProcessOverlaps			();
	void			PruneOverlaps			( MOAICollisionProp& prop );
	void			Render					();
	void			RemoveHull				( MOAICollisionProp& prop );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	bool			MOAIAction_IsDone					();
	void			MOAIAction_Update					( double step );
	void			MOAIPartition_DrawDebugFront		();
	void			MOAIPartition_OnInsertHull			( MOAIPartitionHull& hull );
	void			MOAIPartition_OnRemoveHull			( MOAIPartitionHull& hull );
	void			MOAIPartition_OnUpdateHull			( MOAIPartitionHull& hull );

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	enum {
		OVERLAP_BEGIN,
		OVERLAP_END,
		OVERLAP_UPDATE,
	};
	
	static const u32 OVERLAP_PASS_INIT = 0;
	
	GET ( u32, CurrentPass, mOverlapPass )
	GET ( u32, NextPass, ( mOverlapPass + 1 ))
	
	//----------------------------------------------------------------//
					MOAICollisionWorld		();
					~MOAICollisionWorld		();
};

#endif
