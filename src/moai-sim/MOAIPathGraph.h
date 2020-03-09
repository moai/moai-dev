// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHGRAPH_H
#define	MOAIPATHGRAPH_H

class MOAIPathFinder;

//================================================================//
// MOAIPathGraph
//================================================================//
class MOAIPathGraph :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIPathFinder;

	u32		mHeuristic;

	//----------------------------------------------------------------//
	static int		_setHeuristic			( lua_State* L );

	//----------------------------------------------------------------//
	virtual void	PushNeighbors			( MOAIPathFinder& pathFinder, ZLIndex nodeID ) = 0;

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIPathGraph			();
					~MOAIPathGraph			();
};

#endif
