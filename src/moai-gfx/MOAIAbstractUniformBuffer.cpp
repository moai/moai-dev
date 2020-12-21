// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractUniformBuffer::_getAttributeID ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractUniformBuffer, "UN" )

	ZLIndex uniformID		= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex index			= state.GetValue < MOAILuaIndex >( 3, 0 );
	
	const MOAIUniformSchema* schema = self->GetSchema ();
	
	if ( schema ) {
		state.Push ( schema->GetAttributeID ( uniformID, index ));
	}
	return 1;
}

//----------------------------------------------------------------//
int MOAIAbstractUniformBuffer::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractUniformBuffer, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, 0 );

	const MOAIUniformSchema* schema = self->GetSchema ();

	if ( schema ) {
		schema->SetUniformValue ( L, 3, self->mUniforms.GetBuffer (), uniformID, 0 );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractUniformBuffer::_setUniformArrayItem ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractUniformBuffer, "U" )

	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 3, 0 );

	const MOAIUniformSchema* schema = self->GetSchema ();

	if ( schema ) {
		schema->SetUniformValue ( L, 4, self->mUniforms.GetBuffer (), uniformID, index );
	}
	return 0;
}

//================================================================//
// MOAIAbstractUniformBuffer
//================================================================//

//----------------------------------------------------------------//
const MOAIUniformSchema* MOAIAbstractUniformBuffer::GetSchema () const {

	return this->MOAIAbstractUniformBuffer_GetSchema ();
}

//----------------------------------------------------------------//
MOAIAbstractUniformBuffer::MOAIAbstractUniformBuffer () {

	RTTI_BEGIN ( MOAIAbstractUniformBuffer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractUniformBuffer >)
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractUniformBuffer::~MOAIAbstractUniformBuffer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractUniformBuffer::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractUniformBuffer::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getAttributeID",				_getAttributeID },
		{ "setUniform",					_setUniform },
		{ "setUniformArrayItem",		_setUniformArrayItem },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIAbstractUniformBuffer::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	const MOAIUniformSchema* schema = this->GetSchema ();
	
	if ( schema ) {
		return schema->ApplyAttrOp ( this->mUniforms.GetBuffer (), attrID, attr, op );
	}
	return false;
}
