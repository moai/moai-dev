// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHFINDER_H
#define	MOAIPATHFINDER_H

#include <moaicore/MOAILua.h>

class MOAIGrid;
class MOAIPathGraph;
class MOAIPathTerrainDeck;

//================================================================//
// MOAIPathWeight
//================================================================//
class MOAIPathWeight {
public:

	float			mDeltaScale;
	float			mPenaltyScale;
};

//================================================================//
// MOAIPathState
//================================================================//
class MOAIPathState {
private:

	friend class MOAIPathFinder;

	int					mNodeID;
	MOAIPathState*		mParent;
	MOAIPathState*		mNext;
	
	float				mScore;
};

//================================================================//
// MOAIPathFinder
//================================================================//
/**	@name	MOAIPathFinder
	@text	Object for maintaining pathfinding state.
*/
class MOAIPathFinder :
	public virtual MOAILuaObject {
private:

	friend class MOAIPathGraph;

	MOAILuaSharedPtr < MOAIPathTerrainDeck > mTerrainDeck;
	MOAILuaSharedPtr < MOAIPathGraph > mGraph;

	USLeanArray < MOAIPathWeight > mWeights;
	USLeanArray < int >	mPath;

	// TODO: optimize implementation with memory pool of path states
	// and binary heap of open paths

	MOAIPathState*		mOpen;
	MOAIPathState*		mClosed;
	
	int					mStartNodeID;
	int					mTargetNodeID;

	MOAIPathState*		mState; // used while expanding open set

	u32					mMask;

	u32					mHeuristic;
	u32					mFlags;

	float				mGWeight;
	float				mHWeight;

	//----------------------------------------------------------------//
	static int			_findPath					( lua_State* L );
	static int			_getGraph					( lua_State* L );
	static int			_getPathEntry				( lua_State* L );
	static int			_getPathSize				( lua_State* L );
	static int			_init						( lua_State* L );
	static int			_reserveTerrainWeights		( lua_State* L );
	static int			_setFlags					( lua_State* L );
	static int			_setGraph					( lua_State* L );
	static int			_setHeuristic				( lua_State* L );
	static int			_setTerrainMask				( lua_State* L );
	static int			_setTerrainDeck				( lua_State* L );
	static int			_setTerrainWeight			( lua_State* L );
	static int			_setWeight					( lua_State* L );

	//----------------------------------------------------------------//
	void				BuildPath			( MOAIPathState* state );
	void				ClearVisitation		();
	void				CloseState			( MOAIPathState* stateToClose );
	MOAIPathState*		NextState			();
	void				Reset				();

public:
	
	DECL_LUA_FACTORY ( MOAIPathFinder )
	
	GET ( const USLeanArray < MOAIPathWeight >&, Weights, mWeights );
	GET ( int, TargetNodeID, mTargetNodeID );
	GET ( u32, Mask, mMask );
	GET ( u32, Heuristic, mHeuristic )
	GET ( u32, Flags, mFlags )
	GET ( float, GWeight, mGWeight )
	GET ( float, HWeight, mHWeight )
	
	//----------------------------------------------------------------//
	bool		CheckMask				( u32 terrain );
	float		ComputeTerrainCost		( float moveCost, u32 terrain0, u32 terrain1 );
	bool		FindPath				( int iterations );
	bool		IsVisited				( int nodeID );
				MOAIPathFinder			();
				~MOAIPathFinder			();
	void		PushState				( int nodeID, float score );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
