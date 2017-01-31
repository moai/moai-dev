// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONLAYER_H
#define	MOAIPARTITIONLAYER_H

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAILayerBase.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIRenderPassBase.h>
#include <moai-sim/MOAIViewport.h>

class MOAICamera;

//================================================================//
// MOAIPartitionLayer
//================================================================//
/**	@lua	MOAIPartitionLayer
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
class MOAIPartitionLayer :
	public virtual MOAIPartitionHolder,
	public virtual MOAILayerBase {
private:

	u32			mSortMode;
	bool		mSortInViewSpace;

	float		mSortScale [ 4 ];

	bool		mPartitionCull2D;

	//----------------------------------------------------------------//
	static int		_draw					( lua_State* L );
	static int		_getPropViewList		( lua_State* L ); // TODO: update and restore this
	static int		_getSortMode			( lua_State* L );
	static int		_getSortScale			( lua_State* L );
	static int		_setPartitionCull2D		( lua_State* L );
	static int		_setSortMode			( lua_State* L );
	static int		_setSortScale			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			DrawPartition			( MOAIPartition& partition );
	void			DrawProps				( MOAIPartitionResultBuffer& buffer );
	void			DrawPropsDebug			( MOAIPartitionResultBuffer& buffer);

	//----------------------------------------------------------------//
	void			MOAILayerBase_Draw		( int subPrimID );

public:
		
	DECL_LUA_FACTORY ( MOAIPartitionLayer )
	
	//----------------------------------------------------------------//
					MOAIPartitionLayer		();
					~MOAIPartitionLayer		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
