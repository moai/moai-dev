// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITION_H
#define	MOAIPARTITION_H

class MOAIPrim;

//================================================================//
// MOAIPartition
//================================================================//
/**	@brief Class for optimizing spatial queries against sets of primitives.
*/
class MOAIPartition :
	public virtual USLuaData,
	public USPartition < MOAIPrim > {
private:

	enum {
		MAX_RESULTS	= 64,
	};

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
	void			PushResultsList			( lua_State* L );
	void			PushSortedResultsList	( lua_State* L );

public:
	
	DECL_LUA_DATA ( MOAIPartition )
	
	//----------------------------------------------------------------//
	
	void			Clear					();
	void			InsertPrim				( MOAIPrim& prim );
					MOAIPartition			();
					~MOAIPartition			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			RemovePrim				( MOAIPrim& prim );
	STLString		ToString				();
};

#endif
