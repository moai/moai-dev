// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractLayer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractLayer::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )

	self->Draw ( MOAIPartitionHull::NO_SUBPRIM_ID );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractLayer::_pushRenderPass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractLayer, "U" )
	MOAIGfxMgr::Get ().PushDrawable ( self );
	return 0;
}

//================================================================//
// MOAIAbstractLayer
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractLayer::MOAIAbstractLayer () {
	
	this->mClearFlags = 0; // no clear on default
	
	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractLayer >)
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIAbstractDrawable )
		RTTI_EXTEND ( MOAISurfaceClearColor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractLayer::~MOAIAbstractLayer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractLayer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractLayer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "draw",						_draw },
		{ "pushRenderPass",				_pushRenderPass },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractLayer::MOAIDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );
}
