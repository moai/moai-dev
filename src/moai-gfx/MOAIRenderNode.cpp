// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIRenderNode.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_getRenderRoot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	state.Push ( self->mRenderRoot );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderNode::_setRenderRoot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderNode, "U" )
	self->mRenderRoot.SetRef ( state, 2 );
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIRenderNode
//================================================================//

//----------------------------------------------------------------//
MOAIRenderNode::MOAIRenderNode () {

	RTTI_BEGIN ( MOAIRenderNode )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderNode >)
		RTTI_EXTEND ( MOAIAbstractRenderNode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderNode::~MOAIRenderNode () {
}

//----------------------------------------------------------------//
void MOAIRenderNode::Render ( u32 renderPhase, MOAILuaMemberRef& ref, MOAIRenderNode* caller ) {

	if ( ref ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( ref );
		MOAIRenderNode::Render ( renderPhase, state, -1, caller );
		state.Pop ( 1 );
	}
}


//----------------------------------------------------------------//
void MOAIRenderNode::Render ( u32 renderPhase, MOAILuaState& state, int idx, MOAIRenderNode* caller ) {

	idx = state.AbsIndex ( idx );
	int valType = lua_type ( state, idx );

	switch ( valType ) {

		case LUA_TUSERDATA: {

			MOAIAbstractRenderNode* node = state.GetLuaObject < MOAIAbstractRenderNode >( idx, false );
			if ( node ) {
				node->Render ( renderPhase );
			}
			break;
		}

		case LUA_TTABLE: {

			size_t tableSize = state.GetTableSize ( idx );
			for ( size_t i = 0; i < tableSize; ++i ) {
				lua_rawgeti ( state, idx, i + 1 );
				MOAIRenderNode::Render ( renderPhase, state, -1, caller );
				lua_pop ( state, 1 );
			}
			break;
		}

		case LUA_TFUNCTION: {

			state.CopyToTop ( idx ); // copy the function to the top
			MOAIDraw::Get ().PushCmdInterface ( state );
			if ( caller ) {
				state.Push ( caller );
				state.Push (( MOAILuaObject* )caller->mScope );
				state.DebugCall ( 3, 0 );
			}
			else {
				state.DebugCall ( 1, 0 );
			}
			break;
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderNode::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderNode::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getRenderRoot",				_getRenderRoot },
		{ "setRenderRoot",				_setRenderRoot },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderNode::MOAIAbstractRenderNode_Render ( u32 renderPhase ) {

	if ( this->mRenderRoot ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		MOAIRenderNode::Render ( renderPhase, state, -1, this );
	}
}
