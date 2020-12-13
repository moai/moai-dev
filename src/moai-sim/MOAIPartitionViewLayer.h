// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONVIEWLAYER_H
#define	MOAIPARTITIONVIEWLAYER_H

#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIPartitionHolder.h>

//================================================================//
// MOAIPartitionViewLayer
//================================================================//
/**	@lua	MOAIPartitionViewLayer
	@text	Scene controls class.
	
	@const	SORT_NONE
	@const	SORT_ISO
	@const	SORT_PRIORITY_ASCENDING
	@const	SORT_PRIORITY_DESCENDING
	@const	SORT_X_ASCENDING
	@const	SORT_X_DESCENDING
	@const	SORT_Y_ASCENDING
	@const	SORT_Y_DESCENDING
	@const	SORT_Z_ASCENDING
	@const	SORT_Z_DESCENDING
	@const	SORT_VECTOR_ASCENDING
	@const	SORT_VECTOR_DESCENDING
*/
class MOAIPartitionViewLayer :
	public virtual MOAIPartitionHolder,
	public virtual MOAIAbstractViewLayer {
protected:

	u32			mSortMode;
	bool		mSortInViewSpace;

	float		mSortScale [ 4 ];

	bool		mPartitionCull2D;

	//----------------------------------------------------------------//
	static int		_getPropViewList		( lua_State* L );
	static int		_getSortMode			( lua_State* L );
	static int		_getSortScale			( lua_State* L );
	static int		_setPartitionCull2D		( lua_State* L );
	static int		_setSortMode			( lua_State* L );
	static int		_setSortScale			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			DrawPartition			( MOAIPartition& partition, u32 renderPhase );
	void			DrawProps				( MOAIPartitionResultBuffer& buffer, u32 renderPhase );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAbstractViewLayer_Render	( u32 renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAIPartitionViewLayer )
	
	//----------------------------------------------------------------//
					MOAIPartitionViewLayer			();
					~MOAIPartitionViewLayer			();
};

#endif
