// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	bool			AreNeighbors				( ZLIndex id1, ZLIndex id2 );
	ZLVec3D			GetNode						( ZLIndex id );
	u32				GetNodeCount				();
	void			PushNeighbors				( MOAIPathFinder& pathFinder, ZLIndex nodeID );
	void			ReserveNodes				( ZLSize total );
	void			SetNeighbors				( ZLIndex id1, ZLIndex id2, bool value );
	void			SetNode						( ZLIndex id, const ZLVec3D& node );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIVecPathGraph )
	
	//----------------------------------------------------------------//
					MOAIVecPathGraph			( ZLContext& context );
					~MOAIVecPathGraph			();
};

#endif
