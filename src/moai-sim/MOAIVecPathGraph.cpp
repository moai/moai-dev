// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathFinder.h>
#include <moai-sim/MOAIVecPathGraph.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	areNeighbors
	@text	Checks if two nodes are neighbors.

	@in		MOAIVecPathGraph self
	@in		number nodeID1
	@in		number nodeID2
	@out	boolean
*/
int MOAIVecPathGraph::_areNeighbors ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "UNN" )

	u32 id1 = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 id2 = state.GetValue < u32 >( 3, 1 ) - 1;

	if ( MOAILogMessages::CheckIndexPlusOne ( id1, self->mNodes.Size (), L ) &&
		MOAILogMessages::CheckIndexPlusOne ( id2, self->mNodes.Size (), L )) {

		bool result = self->AreNeighbors ( id1, id2 );
		state.Push ( result );

		return 1;
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getNode
	@text	Returns the coordinates of a node.

	@in		MOAIVecPathGraph self
	@in		number nodeID
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIVecPathGraph::_getNode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "UN" )

	u32 id = state.GetValue < u32 >( 2, 1 ) - 1;

	if ( MOAILogMessages::CheckIndexPlusOne ( id, self->mNodes.Size (), L )) {
		ZLVec3D vec = self->GetNode( id );

		state.Push ( vec.mX );
		state.Push ( vec.mY );
		state.Push ( vec.mZ );

		return 3;
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getNodeCount
	@text	Returns the number of nodes in the graph.

	@in		MOAIVecPathGraph self
	@out	number count
*/
int MOAIVecPathGraph::_getNodeCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "U" )

	u32 count = self->GetNodeCount ();
	state.Push ( count );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveNodes
	@text	Reserves memory for a given number of nodes.

	@in		MOAIVecPathGraph self
	@in		number nNodes
	@out	nil
*/
int MOAIVecPathGraph::_reserveNodes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveNodes ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setNeighbors
	@text	Sets the neighborhood relation for two nodes.

	@in		MOAIVecPathGraph self
	@in		number nodeID1
	@in		number nodeID2
	@opt	boolean value			Whether the nodes are neighbors (true) or not (false). Defaults to true.
	@out	nil
*/
int MOAIVecPathGraph::_setNeighbors ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "UNN" )

	u32 id1 = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 id2 = state.GetValue < u32 >( 3, 1 ) - 1;
	bool neighbors = state.GetValue < bool >( 4, true );

	if ( MOAILogMessages::CheckIndexPlusOne ( id1, self->mNodes.Size (), L ) &&
		MOAILogMessages::CheckIndexPlusOne ( id2, self->mNodes.Size (), L )) {

		self->SetNeighbors ( id1, id2, neighbors );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setNode
	@text	Sets the coordinates of a node.

	@in		MOAIVecPathGraph self
	@in		number nodeID
	@opt	number x				Defaults to 0.
	@opt	number y				Defaults to 0.
	@opt	number z				Defaults to 0.
	@out	nil
*/
int MOAIVecPathGraph::_setNode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVecPathGraph, "UN" )

	u32 id = state.GetValue < u32 >( 2, 1 ) - 1;
	float x = state.GetValue < float >( 3, 0.0f );
	float y = state.GetValue < float >( 4, 0.0f );
	float z = state.GetValue < float >( 5, 0.0f );

	if ( MOAILogMessages::CheckIndexPlusOne ( id, self->mNodes.Size (), L )) {
		self->SetNode ( id, ZLVec3D ( x, y, z ));
	}

	return 0;
}

//================================================================//
// MOAIVecPathGraph
//================================================================//

//----------------------------------------------------------------//
bool MOAIVecPathGraph::AreNeighbors ( u32 id1, u32 id2 ) {
	
	u32 total = this->mNodes.Size ();

	if ( id1 < total && id2 < total ) {
		return this->mNeighbors [ id1 * total + id2 ];
	}

	return false;
}

//----------------------------------------------------------------//
ZLVec3D MOAIVecPathGraph::GetNode ( u32 id ) {
	
	if ( id < this->mNodes.Size ()) {
		return this->mNodes [ id ];
	}

	return ZLVec3D ( 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
u32 MOAIVecPathGraph::GetNodeCount () {
	
	return this->mNodes.Size ();
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::PushNeighbors ( MOAIPathFinder& pathFinder, int nodeID ) {

	u32 total = this->mNodes.Size ();

	ZLVec3D currentNode = this->GetNode ( nodeID );
	ZLVec3D targetNode = this->GetNode ( pathFinder.GetTargetNodeID ());

	for ( u32 neighborID = 0; neighborID < total; ++neighborID ) {
		if ( this->AreNeighbors ( nodeID, neighborID ) &&
			!pathFinder.IsVisited ( neighborID )) {

			ZLVec3D neighbor = this->GetNode ( neighborID );
			float g = neighbor.Dist ( currentNode ) * pathFinder.GetGWeight ();
			float h = neighbor.Dist ( targetNode ) * pathFinder.GetHWeight ();
			pathFinder.PushState ( neighborID, g, h );
		}
	}
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::RegisterLuaClass ( MOAILuaState& state ) {

	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIPathGraph::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "areNeighbors",			_areNeighbors },
		{ "getNode",				_getNode },
		{ "getNodeCount",			_getNodeCount },
		{ "reserveNodes",			_reserveNodes },
		{ "setNeighbors",			_setNeighbors },
		{ "setNode",				_setNode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::ReserveNodes ( u32 total ) {

	this->mNodes.Init ( total );
	this->mNeighbors.Init ( total * total );
	this->mNeighbors.Fill ( false );
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::SetNeighbors ( u32 id1, u32 id2, bool value ) {
	
	u32 total = this->mNodes.Size ();

	if ( id1 < total && id2 < total) {
		this->mNeighbors [ id1 * total + id2 ] = value;
		this->mNeighbors [ id2 * total + id1 ] = value;
	}
}

//----------------------------------------------------------------//
void MOAIVecPathGraph::SetNode ( u32 id, const ZLVec3D& node ) {
	
	if ( id < this->mNodes.Size ()) {
		this->mNodes [ id ] = node;
	}
}

//----------------------------------------------------------------//
MOAIVecPathGraph::MOAIVecPathGraph () {
	
	RTTI_SINGLE ( MOAIPathGraph )
}

//----------------------------------------------------------------//
MOAIVecPathGraph::~MOAIVecPathGraph () {

}
