// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHGRAPH_H
#define	MOAIPATHGRAPH_H

#include <moaicore/MOAILua.h>

class MOAIPathFinder;

//================================================================//
// MOAIPathGraph
//================================================================//
class MOAIPathGraph :
	public virtual MOAILuaObject {
protected:

	friend class MOAIPathFinder;

	u32		mHeuristic;

	//----------------------------------------------------------------//
	static int		_setHeuristic			( lua_State* L );

	//----------------------------------------------------------------//
	virtual void	PushNeighbors			( MOAIPathFinder& pathFinder, int nodeID ) = 0;

public:

	//----------------------------------------------------------------//
					MOAIPathGraph			();
					~MOAIPathGraph			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
