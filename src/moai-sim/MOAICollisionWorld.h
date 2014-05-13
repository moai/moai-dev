// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIPartition.h>

class MOAICollisionProp;
class MOAIOverlapInfo;

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public MOAIPartition,
	public MOAIAction,
	public MOAIRenderable {
private:

	enum {
		OVERLAP_PASS_INIT	= 0x01,
		OVERLAP_PASS_XOR	= 0x03,
	};

	bool	mUpdated;
	u32		mOverlapPass;

	typedef ZLLeanList < MOAICollisionProp* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionProp* > mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* > mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap > mOverlapPool;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_processOverlaps		( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				ClearOverlaps			( MOAICollisionProp& prop );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const MOAIOverlapInfo& overlapInfo );
	void				HandleOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1, MOAIOverlapInfo& overlapInfo );
	bool				IsDone					();
	void				MakeActive				( MOAICollisionProp& prop );
	void				MakeInactive			( MOAICollisionProp& prop );
	void				OnPropInserted			( MOAIProp& prop );	
	void				OnPropRemoved			( MOAIProp& prop );	
	void				OnPropUpdated			( MOAIProp& prop );
	void				OnUpdate				( float step );
	void				ProcessOverlaps			();
	void				Render					();

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	enum {
		OVERLAP_BEGIN,
		OVERLAP_END,
		OVERLAP_UPDATE,
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
