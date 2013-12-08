// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathGraph.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIPathFinder
//================================================================//

//----------------------------------------------------------------//
MOAIPathGraph::MOAIPathGraph () {

	RTTI_SINGLE ( MOAIPathGraph )
}

//----------------------------------------------------------------//
MOAIPathGraph::~MOAIPathGraph () {
}

//----------------------------------------------------------------//
void MOAIPathGraph::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathGraph::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
