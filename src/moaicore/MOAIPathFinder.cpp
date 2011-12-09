// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPathFinder.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIPathFinder
//================================================================//

//----------------------------------------------------------------//
bool MOAIPathFinder::IsCandidate ( int nodeID ) {
	UNUSED ( nodeID );
	return true; // TODO
}

//----------------------------------------------------------------//
MOAIPathFinder::MOAIPathFinder () :
	mOpen ( 0 ),
	mClosed ( 0 ),
	mPath ( 0 ),
	mTargetNodeID ( 0 ) {
}

//----------------------------------------------------------------//
MOAIPathFinder::~MOAIPathFinder () {
}

//----------------------------------------------------------------//
void MOAIPathFinder::PushState ( int nodeID, float score ) {
	UNUSED ( nodeID );
	UNUSED ( score );
	
	//if ( this->IsCandidate ( nodeID )) {
	//	
	//	MOAIPathState* state = new MOAIPathState ();
	//	state->mNodeID = nodeID;
	//	state->mParent = parent;

	//	state->mNext = this->mOpen;
	//	this->mOpen = state;
	//	
	//	state->mScore = score;
	//}
}

//----------------------------------------------------------------//
void MOAIPathFinder::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathFinder::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );

	//luaL_Reg regTable [] = {
	//	{ "getLength",		_getLength },
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}
