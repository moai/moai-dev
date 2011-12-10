// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGridPathGraph.h>
#include <moaicore/MOAIPathFinder.h>
#include <moaicore/MOAIPathGraph.h>
#include <moaicore/MOAIPathTerrainDeck.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	findPath
	@text	Attempts to find an efficient path from the start node
			to the finish node. May be called incrementally.

	@in		MOAIPathFinder self
	@opt	number iterations
	@out	boolean more
*/
int MOAIPathFinder::_findPath ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "U" )
	
	int iterations = state.GetValue < int >( 2, -1 );
	
	state.Push ( self->FindPath ( iterations ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPathEntry
	@text	Returns a path entry. This is a node ID that may be
			passed back to the graph to get a location.

	@in		MOAIPathFinder self
	@in		number index
	@out	number entry
*/
int MOAIPathFinder::_getPathEntry ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "UN" )
	
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( index < self->mPath.Size ()) {
	
		state.Push ( self->mPath [ index ] + 1 );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getPathSize
	@text	Returns the size of the path (in nodes).

	@in		MOAIPathFinder self
	@out	number size
*/
int MOAIPathFinder::_getPathSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "U" )
	
	state.Push ( self->mPath.Size ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Specify the ID of the start and target node.

	@in		MOAIPathFinder self
	@in		number startNodeID
	@in		number targetNodeID
	@out	nil
*/
int MOAIPathFinder::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "UNN" )
	
	self->mStartNodeID = state.GetValue < int >( 2, 1 ) - 1;
	self->mTargetNodeID = state.GetValue < int >( 3, 1 ) - 1;
	
	self->Reset ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveTerrainWeights
	@text	Specify the size of the terrain weight vector. 

	@in		MOAIPathFinder self
	@opt	number size				Default value is 0.
	@out	nil
*/
int MOAIPathFinder::_reserveTerrainWeights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	
	self->mWeights.Init ( size );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGraph
	@text	Set graph data to use for pathfinding. 

	@overload

		@in		MOAIPathFinder self
		@opt	MOAIGrid graphData				Default value is nil.
		@out	nil
	
	@overload
	
		@in		MOAIPathFinder self
		@opt	MOAISparseGraph graphData		Default value is nil.
		@out	nil
*/
int MOAIPathFinder::_setGraph ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "U" )

	self->ClearGraph ();

	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2 );
	if ( grid ) {
		MOAIGridPathGraph* graph = new MOAIGridPathGraph ();
		graph->mGrid = grid;
		self->mGraph = graph;
		
		self->mGraphData.Set ( *self, grid );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTerrainDeck
	@text	Set terrain deck to use with graph.

	@in		MOAIPathFinder self
	@opt	MOAIPathTerrainDeck terrainDeck		Default value is nil.
	@out	nil
*/
int MOAIPathFinder::_setTerrainDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "U" )
	
	self->mTerrainDeck.Set ( *self, state.GetLuaObject < MOAIPathTerrainDeck >( 2 ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTerrainWeight
	@text	Set a component of the terrain weight vector.

	@in		MOAIPathFinder self
	@in		number index
	@opt	number deltaScalar			Default value is 0.
	@opt	number penaltyScalar		Default value is 0.
	@out	nil
*/
int MOAIPathFinder::_setTerrainWeight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathFinder, "UNNN" )
	
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;

	if ( index < self->mWeights.Size ()) {
	
		MOAIPathWeight& weight = self->mWeights [ index ];
		weight.mDeltaScalar = state.GetValue < float >( 3, 0.0f );
		weight.mPenaltyScalar = state.GetValue < float >( 4, 0.0f );
	}
	return 0;
}

//================================================================//
// MOAIPathFinder
//================================================================//

//----------------------------------------------------------------//
void MOAIPathFinder::BuildPath ( MOAIPathState* state ) {

	u32 size = 0;
	for ( MOAIPathState* cursor = state; cursor; cursor = cursor->mParent, ++size );
	
	this->mPath.Init ( size );
	for ( MOAIPathState* cursor = state; cursor; cursor = cursor->mParent ) {
		this->mPath [ --size ] = cursor->mNodeID;
	}
	
	this->ClearVisitation ();
}

//----------------------------------------------------------------//
void MOAIPathFinder::ClearGraph () {

	if ( this->mGraph ) {
		delete this->mGraph;
	}
	
	this->mGraph = 0;
	this->mGraphData.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPathFinder::ClearVisitation () {

	while ( this->mOpen ) {
		MOAIPathState* state = this->mOpen;
		this->mOpen = this->mOpen->mNext;
		delete state;
	}
	
	while ( this->mClosed ) {
		MOAIPathState* state = this->mClosed;
		this->mClosed = this->mClosed->mNext;
		delete state;
	}
}

//----------------------------------------------------------------//
void MOAIPathFinder::CloseState ( MOAIPathState* stateToClose ) {
	
	MOAIPathState* cursor = this->mOpen;
	this->mOpen = 0;
	
	while ( cursor ) {
		MOAIPathState* state = cursor;
		cursor = cursor->mNext;
		
		if ( state == stateToClose ) {
			state->mNext = this->mClosed;
			this->mClosed = state;
		}
		else {
			state->mNext = this->mOpen;
			this->mOpen = state;
		}
	}
}

//----------------------------------------------------------------//
bool MOAIPathFinder::CheckMask ( u32 terrain ) {

	if ( !terrain || ( terrain & MOAITileFlags::HIDDEN )) return false;

	if ( this->mTerrainDeck ) {
		return this->mMask & this->mTerrainDeck->GetMask ( terrain & MOAITileFlags::CODE_MASK ) ? true : false;
	}
	return true;
}

//----------------------------------------------------------------//
float MOAIPathFinder::ComputeTerrainCost ( float moveCost, u32 terrain0, u32 terrain1 ) {

	if ( !this->mTerrainDeck ) return 0.0f;

	if ( terrain0 & MOAITileFlags::HIDDEN ) return 0.0f;
	if ( terrain1 & MOAITileFlags::HIDDEN ) return 0.0f;

	u32 total = MIN ( this->mTerrainDeck->GetVectorSize (), this->mWeights.Size ());
	
	float* v0 = this->mTerrainDeck->GetVector ( terrain0 & MOAITileFlags::CODE_MASK );
	float* v1 = this->mTerrainDeck->GetVector ( terrain1 & MOAITileFlags::CODE_MASK );
	
	float terrainCost = 0.0f;
	for ( u32 i = 0; i < total; ++i ) {
		
		const MOAIPathWeight& weight = this->mWeights [ i ];
		
		float c0 = v0 [ i ];
		float c1 = v1 [ i ];
		
		float delta = ( c1 - c0 ) * weight.mDeltaScalar;
		float penalty = ( c1 + c0 ) * 0.5f * weight.mPenaltyScalar;
		
		terrainCost += ( moveCost * delta ) + ( moveCost * penalty );
	}
	return terrainCost;
}

//----------------------------------------------------------------//
bool MOAIPathFinder::FindPath ( int iterations ) {
	
	if ( !this->mState ) {
		this->PushState ( this->mStartNodeID, 0.0f );
	}
	
	bool noIterations = iterations <= 0;
	
	for ( ; this->mOpen && (( iterations > 0 ) || noIterations ); iterations-- ) {
		
		this->mState = this->NextState ();
		this->CloseState ( this->mState );
		
		if ( this->mState->mNodeID == this->mTargetNodeID ) {
			this->BuildPath ( this->mState );
			return false;
		}
		this->mGraph->PushNeighbors ( *this, this->mState->mNodeID );
	}
	return this->mOpen ? true : false;
}

//----------------------------------------------------------------//
bool MOAIPathFinder::IsVisited ( int nodeID ) {

	for ( MOAIPathState* state = this->mOpen; state; state = state->mNext ) {
		if ( state->mNodeID == nodeID ) return true;
	}
	
	for ( MOAIPathState* state = this->mClosed; state; state = state->mNext ) {
		if ( state->mNodeID == nodeID ) return true;
	}
	
	return false;
}

//----------------------------------------------------------------//
MOAIPathFinder::MOAIPathFinder () :
	mOpen ( 0 ),
	mClosed ( 0 ),
	mStartNodeID ( 0 ),
	mTargetNodeID ( 0 ),
	mState ( 0 ),
	mGraph ( 0 ),
	mMask ( 0xffffffff ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPathFinder::~MOAIPathFinder () {

	this->ClearGraph ();
	this->ClearVisitation ();
	
	this->mTerrainDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
MOAIPathState* MOAIPathFinder::NextState () {

	MOAIPathState* state = this->mOpen;
	MOAIPathState* best = state;
	state = state->mNext;

	for ( ; state; state = state->mNext ) {
		if ( state->mScore < best->mScore ) {
			best = state;
		}
	}
	return best;
}

//----------------------------------------------------------------//
void MOAIPathFinder::PushState ( int nodeID, float score ) {
	
	MOAIPathState* state = new MOAIPathState ();
	state->mNodeID = nodeID;
	state->mParent = this->mState;

	state->mNext = this->mOpen;
	this->mOpen = state;
	
	state->mScore = score;
}

//----------------------------------------------------------------//
void MOAIPathFinder::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathFinder::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
	
	luaL_Reg regTable [] = {
		{ "findPath",					_findPath },
		{ "getPathEntry",				_getPathEntry },
		{ "getPathSize",				_getPathSize },
		{ "init",						_init },
		{ "reserveTerrainWeights",		_reserveTerrainWeights },
		{ "setGraph",					_setGraph },
		{ "setTerrainDeck",				_setTerrainDeck },
		{ "setTerrainWeight",			_setTerrainWeight },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPathFinder::Reset () {

	this->mState = 0;
	this->mPath.Clear ();

	this->ClearVisitation ();
}
