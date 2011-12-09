// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPathGraph.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIPathGraph
//================================================================//

//----------------------------------------------------------------//
MOAIPathGraph::MOAIPathGraph () {

	RTTI_SINGLE ( MOAILuaObject )
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

	//luaL_Reg regTable [] = {
	//	{ "getLength",		_getLength },
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}
