// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDPATHGRAPH_H
#define	MOAIGRIDPATHGRAPH_H

#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIPathFinder.h>

//================================================================//
// MOAIGridPathGraph
//================================================================//
class MOAIGridPathGraph :
	public MOAIPathGraph {
private:

	friend class MOAIPathFinder;

	MOAIGrid*	mGrid;

	//----------------------------------------------------------------//
	void			PushNeighbor				( MOAIPathFinder& pathFinder, u32 tile0, int xTile, int yTile, float moveCost );
	void			PushNeighbors				( MOAIPathFinder& pathFinder, int nodeID );

public:
	
	//----------------------------------------------------------------//
					MOAIGridPathGraph			();
					~MOAIGridPathGraph			();
};

#endif
