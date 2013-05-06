// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifdef DOXYGEN

//================================================================//
// MOAILayer2D
//================================================================//
/**	@name	MOAILayer2D
	@text	2D layer.
	
	@const	SORT_NONE
	@const	SORT_PRIORITY_ASCENDING
	@const	SORT_PRIORITY_DESCENDING
	@const	SORT_X_ASCENDING
	@const	SORT_X_DESCENDING
	@const	SORT_Y_ASCENDING
	@const	SORT_Y_DESCENDING
	@const	SORT_VECTOR_ASCENDING
	@const	SORT_VECTOR_DESCENDING
*/
class MOAILayer2D :
	public MOAIProp2D {
protected:

	//----------------------------------------------------------------//
	static int	_clear				( lua_State* L );
	static int	_getFitting			( lua_State* L );
	static int	_getPartition		( lua_State* L );
	static int	_getSortMode		( lua_State* L );
	static int	_getSortScale		( lua_State* L );
	static int	_insertProp			( lua_State* L );
	static int	_removeProp			( lua_State* L );
	static int	_setBox2DWorld		( lua_State* L );
	static int	_setCamera			( lua_State* L );
	static int	_setCpSpace			( lua_State* L );
	static int	_setFrameBuffer		( lua_State* L );
	static int	_setParallax		( lua_State* L );
	static int	_setPartition		( lua_State* L );
	static int	_setPartitionCull2D	( lua_State* L );
	static int	_setSortMode		( lua_State* L );
	static int	_setSortScale		( lua_State* L );
	static int	_setViewport		( lua_State* L );
	static int	_showDebugLines		( lua_State* L );
	static int	_wndToWorld			( lua_State* L );
	static int	_worldToWnd			( lua_State* L );
};

#endif