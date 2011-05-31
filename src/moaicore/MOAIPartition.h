// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITION_H
#define	MOAIPARTITION_H

#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>

//================================================================//
// MOAIPartition
//================================================================//
/**	@name	MOAIPartition
	@text	Class for optimizing spatial queries against sets of primitives.
			Configure for performance; default behavior is a simple list.
*/
class MOAIPartition :
	public virtual USLuaObject {
private:

	friend class MOAIPartitionCell;
	friend class MOAIPartitionLayer;
	friend class MOAIProp;

	enum {
		MAX_RESULTS	= 256,
	};

	USLeanArray < MOAIPartitionLayer >		mLayers;
	MOAIPartitionCell						mEmpties;
	MOAIPartitionCell						mGlobals;

	u32					mTotalResults;
	MOAIProp*			mResults;

	s32					mPriorityCounter;
	static const s32	PRIORITY_MASK = 0x7fffffff;

	//----------------------------------------------------------------//
	static int		_clear						( lua_State* L );
	static int		_insertProp					( lua_State* L );
	static int		_propForPoint				( lua_State* L );
	static int		_propListForPoint			( lua_State* L );
	static int		_propListForRect			( lua_State* L );
	static int		_removeProp					( lua_State* L );
	static int		_reserveLayers				( lua_State* L );
	static int		_setLayer					( lua_State* L );
	static int		_sortedPropListForPoint		( lua_State* L );
	static int		_sortedPropListForRect		( lua_State* L );

	//----------------------------------------------------------------//
	void			AffirmPriority			( MOAIProp& prop );
	void			PushResult				( MOAIProp& result );
	void			PushResultsList			( lua_State* L );
	void			PushSortedResultsList	( lua_State* L );
	void			ResetResults			();
	void			UpdateProp				( MOAIProp& prop, u32 status );
	void			UpdateProp				( MOAIProp& prop, const USRect& bounds, u32 status );

public:
	
	DECL_LUA_FACTORY ( MOAIPartition )
	
	GET ( u32, TotalResults, mTotalResults )
	
	//----------------------------------------------------------------//
	void			Clear					();
	u32				GatherProps				( MOAIProp* ignore, u32 mask = 0xffffffff );
	u32				GatherProps				( USVec2D& point, MOAIProp* ignore, u32 mask = 0xffffffff );
	u32				GatherProps				( USRect& rect, MOAIProp* ignore, u32 mask = 0xffffffff );
	void			InsertProp				( MOAIProp& prop );
					MOAIPartition			();
					~MOAIPartition			();
	MOAIProp*		PopResult				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			RemoveProp				( MOAIProp& prop );
	void			ReserveLayers			( int totalLayers );
	void			SetLayer				( int layerID, float cellSize, int width, int height );
	STLString		ToString				();
};

#endif
