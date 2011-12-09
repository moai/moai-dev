// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHGRAPH_H
#define	MOAIPATHGRAPH_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIPathFinder.h>

//================================================================//
// MOAIPathGraph
//================================================================//
class MOAIPathGraph :
	public virtual MOAILuaObject {
private:

	//----------------------------------------------------------------//

public:
	
	//----------------------------------------------------------------//
					MOAIPathGraph			();
					~MOAIPathGraph			();
	//virtual void	PushNeighbors			( int nodeID, int targetID ) = 0;
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
