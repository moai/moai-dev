// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECPATHGRAPH_H
#define	MOAIVECPATHGRAPH_H

#include <moai-sim/MOAIPathGraph.h>

class MOAIPathFinder;

class MOAIVecPathGraph :
	public MOAIPathGraph {
private:

	friend class MOAIPathFinder;
	
	ZLLeanArray < ZLVec3D >	mNodes;
	ZLLeanArray < bool >	mNeighbors;
	
	//----------------------------------------------------------------//
	static int		_areNeighbors				( lua_State* L );
	static int		_getNode					( lua_State* L );
	static int		_getNodeCount				( lua_State* L );
	static int		_reserveNodes				( lua_State* L );
	static int		_setNeighbors				( lua_State* L );
	static int		_setNode					( lua_State* L );

	//----------------------------------------------------------------//
	bool			AreNeighbors				( u32 id1, u32 id2 );
	ZLVec3D			GetNode						( u32 id );
	u32				GetNodeCount				();
	void			PushNeighbors				( MOAIPathFinder& pathFinder, int nodeID );
	void			ReserveNodes				( u32 total );
	void			SetNeighbors				( u32 id1, u32 id2, bool value );
	void			SetNode						( u32 id, const ZLVec3D& node );

public:
	
	DECL_LUA_FACTORY ( MOAIVecPathGraph )
	
	//----------------------------------------------------------------//
					MOAIVecPathGraph			();
					~MOAIVecPathGraph			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
