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

	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIPathGraph, MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPathGraph::~MOAIPathGraph () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPathGraph::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIPathGraph::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}
