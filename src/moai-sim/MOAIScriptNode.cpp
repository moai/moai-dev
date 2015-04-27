// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIScriptNode.h>

class MOAIMemberTableAttr {
protected:
	friend class MOAIScriptNode;
	MOAIScriptNode*		mSource;
	cc8*				mFieldName;
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveAttrs
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
	
	self->mAttrNames.Init ( size );
	self->mAttrNames.Fill ( 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets a Lua function to be called whenever the node is updated.
	
	@in		MOAIScriptNode self
	@in		function onUpdate
	@out	nil
*/
int MOAIScriptNode::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "UF" );

	self->mOnUpdate.SetRef ( *self, state, 2 );
	return 0;
}


int MOAIScriptNode::_setAttrName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptNode, "U" );
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	self->mAttrNames [ idx ] = state.GetValue < cc8* >( 3, 0 );
	
	return 0;
}

//================================================================//
// MOAIScriptNode
//================================================================//

//----------------------------------------------------------------//
bool MOAIScriptNode::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	attrID = UNPACK_ATTR(attrID) - 1;
	
	if ( attrID >= this->mAttributes.Size()) {
		return false;
	}
	
	if ( this->mAttrNames [ attrID ] == 0 ) {
		this->mAttributes [ attrID ] = attrOp.Apply ( this->mAttributes [ attrID ], op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
		return true;
	}
	else {
		switch ( op ) {
			case MOAIAttrOp::CHECK:
				attrOp.SetFlags ( MOAIAttrOp::ATTR_READ_WRITE );
				break;
				
			case MOAIAttrOp::ADD:
				this->NamedAttrAdd ( attrID, attrOp );
				break;
				
			case MOAIAttrOp::SET:
				this->NamedAttrSet ( attrID, attrOp );
				break;
				
			case MOAIAttrOp::GET:
				this->NamedAttrGet ( attrID, attrOp );
				break;
		}
		return true;
	}
}

//----------------------------------------------------------------//
MOAIScriptNode::MOAIScriptNode () {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIScriptNode::~MOAIScriptNode () {
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrAdd ( u32 attrID, MOAIAttrOp &attrOp ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	switch ( attrOp.GetTypeHint ()) {
		case MOAIAttrOp::ATTR_TYPE_FLOAT: {
			float value = attrOp.GetValue ( 0.0f );
			
			if ( value != 0.0f ) {
				
				MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
				this->PushMemberTable ( state );
				float cur = state.GetField < float >( -1, attrName, 0.0f );
				state.SetField ( -1, attrName, cur + value );
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_INT: {
			int value = ( int )attrOp.GetValue ( 0 );
			
			if ( value != 0 ) {
				MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
				this->PushMemberTable ( state );
				int cur = state.GetField < int >( -1, attrName, 0 );
				state.SetField ( -1, attrName, cur + value );
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrGet ( u32 attrID, MOAIAttrOp &attrOp ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushMemberTable ( state );
	state.GetField ( -1, attrName );
	
	if ( state.IsType ( -1, LUA_TNUMBER )) {
		
		float value = state.GetValue < float >( -1, 0.0f );
		attrOp.Apply ( value, MOAIAttrOp::GET, MOAIAttrOp::ATTR_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
	}
	else {
		
		MOAIMemberTableAttr value;
		value.mSource = this;
		value.mFieldName = attrName;
		attrOp.ApplyNoAdd ( value, MOAIAttrOp::GET, MOAIAttrOp::ATTR_WRITE, MOAIAttrOp::ATTR_TYPE_VARIANT );
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::NamedAttrSet ( u32 attrID, MOAIAttrOp &attrOp ) {
	
	cc8* attrName = this->mAttrNames [ attrID ];
	switch ( attrOp.GetTypeHint ()) {
		case MOAIAttrOp::ATTR_TYPE_FLOAT: {

			float value = attrOp.GetValue ( 0.0f );
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->PushMemberTable ( state );
			state.SetField ( -1, attrName, value );
			
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_INT: {
			
			int value = attrOp.GetValue ( 0 );
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->PushMemberTable ( state );
			state.SetField ( -1, attrName, value );
			
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_VARIANT: {
			
			MOAIMemberTableAttr value;
			value = attrOp.GetValue < MOAIMemberTableAttr >( value );
			MOAIScriptNode* source = value.mSource;
			cc8* sourceField = value.mFieldName;
			
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			source->PushMemberTable ( state );
			this->PushMemberTable ( state );
			state.GetField ( -2, sourceField );
			
			lua_setfield ( state, -2, attrName );
			
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIScriptNode::OnDepNodeUpdate () {

	if ( this->mOnUpdate ) {
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnUpdate.PushRef ( state )) {
			this->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
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
		{ "setAttrName",			_setAttrName },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
