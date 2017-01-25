// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAIDrawable.h>
#include <moai-sim/MOAIPartition.h>

class MOAICollisionProp;
class MOAIOverlapInfo;
class MOAIPartitionHull;

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public MOAIAction,
	public MOAIDrawable,
	public MOAIPartition {
private:

	friend class MOAICollisionProp;

	bool	mUpdated;
	u32		mOverlapPass;

	typedef ZLLeanList < MOAICollisionProp* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionProp* > mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* > mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap > mOverlapPool;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_insertProp				( lua_State* L );
	static int			_processOverlaps		( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo );
	void				ClearOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				ClearOverlaps			( MOAICollisionProp& prop );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo );
	bool				IsDone					();
	void				InvalidateOverlaps		( MOAICollisionProp& prop, u32 nextPass );
	void				MakeActive				( MOAICollisionProp& prop0 );
	void				MakeInactive			( MOAICollisionProp& prop0 );
	void				OnPropInserted			( MOAIPartitionHull& prop );
	void				OnPropRemoved			( MOAIPartitionHull& prop );
	void				OnPropUpdated			( MOAIPartitionHull& prop );
	void				ProcessOverlaps			();
	void				PruneOverlaps			( MOAICollisionProp& prop );
	void				Render					();
	void				RemoveHull				( MOAICollisionProp& prop );

	//----------------------------------------------------------------//
	void				MOAIAction_Update				( double step );
	void				MOAIDrawable_Draw				( int subPrimID );
	void				MOAIPartition_OnInsertHull		( MOAIPartitionHull& hull );
	void				MOAIPartition_OnRemoveHull		( MOAIPartitionHull& hull );
	void				MOAIPartition_OnUpdateHull		( MOAIPartitionHull& hull );

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	enum {
		OVERLAP_BEGIN,
		OVERLAP_END,
		OVERLAP_UPDATE,
	};
	
	static const u32 OVERLAP_PASS_INIT = 0;
	
	//----------------------------------------------------------------//
					MOAICollisionWorld		();
					~MOAICollisionWorld		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
