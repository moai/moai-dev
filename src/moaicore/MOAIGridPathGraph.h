// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDPATHGRAPH_H
#define	MOAIGRIDPATHGRAPH_H

#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIPathGraph.h>

class MOAIGridPathGraphParams;
class MOAIPathFinder;

//================================================================//
// MOAIGridPathGraph
//================================================================//
/**	@name	MOAIGridPathGraph
	@text	Pathfinder graph adapter for MOAIGrid.
*/
class MOAIGridPathGraph :
	public MOAIPathGraph {
private:

	friend class MOAIPathFinder;

	enum {
		H_MOVE_COST,
		V_MOVE_COST,
		D_MOVE_COST,
	};

	MOAILuaSharedPtr < MOAIGrid > mGrid;

	//----------------------------------------------------------------//
	static int		_setGrid					( lua_State* L );

	//----------------------------------------------------------------//
	float			ComputeHeuristic			( MOAIGridPathGraphParams& params, const MOAICellCoord& c0, const MOAICellCoord& c1 );
	void			PushNeighbor				( MOAIPathFinder& pathFinder, MOAIGridPathGraphParams& params, u32 tile0, int xTile, int yTile, float moveCost );
	void			PushNeighbors				( MOAIPathFinder& pathFinder, int nodeID );

public:
	
	enum DISTANCE {
		MANHATTAN_DISTANCE,
		DIAGONAL_DISTANCE,
		EUCLIDEAN_DISTANCE,
	};
	
	static const u32 NO_DIAGONALS = 0x00000001;
	
	DECL_LUA_FACTORY ( MOAIGridPathGraph )
	
	//----------------------------------------------------------------//
					MOAIGridPathGraph			();
					~MOAIGridPathGraph			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SetGrid						( MOAIGrid* grid );
};

#endif
