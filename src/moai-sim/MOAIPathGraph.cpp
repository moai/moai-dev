// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathGraph.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIPathFinder
//================================================================//

//----------------------------------------------------------------//
MOAIPathGraph::MOAIPathGraph () {

	RTTI_SINGLE ( MOAIPathGraph, MOAIPathGraph )
}

//----------------------------------------------------------------//
MOAIPathGraph::~MOAIPathGraph () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPathGraph::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIPathGraph::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}
