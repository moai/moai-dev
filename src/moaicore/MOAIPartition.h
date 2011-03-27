// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITION_H
#define	MOAIPARTITION_H

#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>

//================================================================//
// MOAIPartition
//================================================================//
/**	@brief Class for optimizing spatial queries against sets of primitives.
*/
class MOAIPartition :
	public virtual USLuaData {
private:

	enum {
		MAX_RESULTS	= 256,
	};

	USLeanArray < MOAIPartitionLayer >		mLayers;
	MOAIPartitionCell						mEmpties;
	MOAIPartitionCell						mGlobals;

	u32										mTotalResults;
	MOAIProp*								mResults;

	//----------------------------------------------------------------//
	static int		_insertPrim					( lua_State* L );
	static int		_primForPoint				( lua_State* L );
	static int		_primListForPoint			( lua_State* L );
	static int		_primListForRect			( lua_State* L );
	static int		_removePrim					( lua_State* L );
	static int		_reserveLayers				( lua_State* L );
	static int		_setLayer					( lua_State* L );
	static int		_sortedPrimListForPoint		( lua_State* L );
	static int		_sortedPrimListForRect		( lua_State* L );

	//----------------------------------------------------------------//
	void			PushResult				( MOAIProp& result );
	void			PushResultsList			( lua_State* L );
	void			PushSortedResultsList	( lua_State* L );
	void			ResetResults			();
	void			UpdateProp				( MOAIProp& prop, u32 status );
	void			UpdateProp				( MOAIProp& prop, const USRect& bounds, u32 status );

public:
	
	friend class MOAIPartitionCell;
	friend class MOAIPartitionLayer;
	friend class MOAIProp;
	
	DECL_LUA_DATA ( MOAIPartition )
	
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
