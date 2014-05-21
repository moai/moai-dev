// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAICollisionFacet.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIRenderable.h>

class MOAICollisionFacet;
class MOAIOverlapInfo;
class MOAIProp;

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public MOAIPartition,
	public MOAIAction,
	public MOAIRenderable {
private:

	bool	mUpdated;
	u32		mOverlapPass;

	typedef ZLLeanList < MOAICollisionFacet* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionFacet* > mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* > mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap > mOverlapPool;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_processOverlaps		( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmOverlap			( MOAICollisionFacet& facet0, MOAICollisionFacet& facet1, const MOAIOverlapInfo& overlapInfo );
	void				ClearOverlap			( MOAICollisionFacet& facet0, MOAICollisionFacet& facet1 );
	void				ClearOverlaps			( MOAICollisionFacet& facet );
	void				DoCallback				( u32 eventID, MOAICollisionFacet& facet0, MOAICollisionFacet& facet1 );
	void				DoCallback				( u32 eventID, MOAICollisionFacet& facet0, MOAICollisionFacet& facet1, const MOAIOverlapInfo& overlapInfo );
	void				DrawFacet				( MOAICollisionFacet& facet );
	bool				IsDone					();
	void				InvalidateOverlaps		( MOAICollisionFacet& facet, u32 nextPass );
	void				MakeActive				( MOAICollisionFacet& facet0 );
	void				MakeInactive			( MOAICollisionFacet& facet0 );
	void				OnPropInserted			( MOAIProp& prop );	
	void				OnPropRemoved			( MOAIProp& prop );	
	void				OnPropUpdated			( MOAIProp& prop );
	void				OnUpdate				( float step );
	void				ProcessOverlaps			();
	void				PruneOverlaps			( MOAICollisionFacet& facet );
	void				Render					();

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
