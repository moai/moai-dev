// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIShaderNode.h>

//================================================================//
// MOAIShaderNode
//================================================================//

//----------------------------------------------------------------//
MOAIShaderNode::MOAIShaderNode () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderNode::~MOAIShaderNode () {
}

//----------------------------------------------------------------//
void MOAIShaderNode::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIShader::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIShaderNode::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIShader::RegisterLuaFuncs ( state );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIShaderNode::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	MOAIShaderProgram* program = MOAICast < MOAIShaderProgram >( this->mProgram );
	if ( program ) {
		return program->ApplyAttrOp ( this->mPendingUniformBuffer.GetBuffer (), attrID, attr, op );
	}
	return false;
}
