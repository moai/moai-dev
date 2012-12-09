// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIScriptNode.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveAttrs
	@text	Reserve memory for custom attributes and initializes them to 0.
	
	@in		MOAIScriptNode self
	@in		number nAttributes
	@out	nil
*/
int MOAIScriptNode::_reserveAttrs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "UN" );

	u32 size = state.GetValue < u32 >( 2, 0 );
	self->mAttributes.Init ( size );
	self->mAttributes.Fill ( 0.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets a Lua function to be called whenever the node is updated.
	
	@in		MOAIScriptNode self
	@in		function onUpdate
	@out	nil
*/
int MOAIScriptNode::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "UF" );

	self->SetLocal ( state, 2, self->mOnUpdate );
	return 0;
}

//================================================================//
// MOAIScriptNode
//================================================================//

//----------------------------------------------------------------//
bool MOAIScriptNode::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	attrID = UNPACK_ATTR(attrID);

	if ( attrID < this->mAttributes.Size ()) {
		this->mAttributes [ attrID ] = attrOp.Apply ( this->mAttributes [ attrID ], op, MOAIAttrOp::ATTR_READ_WRITE );
		return true;
	}
	return false;
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
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		this->PushLocal ( state, this->mOnUpdate );
		this->PushLuaUserdata ( state );

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptNode::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveAttrs",			_reserveAttrs },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
