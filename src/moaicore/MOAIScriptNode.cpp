// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIScriptNode.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIScriptNode::_reserveAttrs ( lua_State* L ) {
	LUA_SETUP ( MOAIScriptNode, "UN" );

	u32 size = state.GetValue < u32 >( 2, 0 );
	self->mAttributes.Init ( size );
	self->mAttributes.Fill ( 0.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIScriptNode::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIScriptNode, "UF" );

	self->mOnUpdate.SetRef ( state, -1, USLuaRef::STRONG_REF );
	return 0;
}

//================================================================//
// MOAIScriptNode
//================================================================//

//----------------------------------------------------------------//
void MOAIScriptNode::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	if ( attrID < this->mAttributes.Size ()) {
		this->mAttributes [ attrID ] = attrOp.Op ( this->mAttributes [ attrID ]);
	}
}

//----------------------------------------------------------------//
u32 MOAIScriptNode::CountAttributes () {

	return this->mAttributes.Size ();
}

//----------------------------------------------------------------//
MOAIScriptNode::MOAIScriptNode () {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIScriptNode::~MOAIScriptNode () {
}

//----------------------------------------------------------------//
void MOAIScriptNode::OnDepNodeUpdate () {

	if ( this->mOnUpdate ) {
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		this->mOnUpdate.PushRef ( state );
		this->PushLuaInstance ( state );

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaClass ( USLuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "reserveAttrs",			_reserveAttrs },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
