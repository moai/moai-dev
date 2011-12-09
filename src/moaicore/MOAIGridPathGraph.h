// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDPATHGRAPH_H
#define	MOAIGRIDPATHGRAPH_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIPathGraph.h>

//================================================================//
// MOAIGridPathGraph
//================================================================//
class MOAIGridPathGraph :
	public MOAIGridSpace,
	public MOAIPathGraph {
private:

	USLeanArray < float >	mVectors;
	u32						mVectorSize;

	//----------------------------------------------------------------//
	static int		_getVector					( lua_State* L );
	static int		_setVector					( lua_State* L );
	static int		_setVectorSize				( lua_State* L );

	//----------------------------------------------------------------//
	void			OnResize					();

public:
	
	DECL_LUA_FACTORY ( MOAIGridPathGraph )
	
	GET ( u32, VectorSize, mVectorSize )
	
	//----------------------------------------------------------------//
	float*			GetVector					( int xTile, int yTile );
					MOAIGridPathGraph			();
					~MOAIGridPathGraph			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
