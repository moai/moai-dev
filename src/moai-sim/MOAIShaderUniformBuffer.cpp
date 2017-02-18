// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAttribute.h>
#include <moai-sim/MOAIShaderUniformBuffer.h>

//================================================================//
// MOAIShaderUniformBuffer
//================================================================//

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {
			
	void* element;
	MOAIShaderUniformFormatter* uniform = this->GetUniformForAttributeID ( attrID, element );
	
	if ( uniform ) {
	
		switch ( op ) {

			case MOAIAttribute::CHECK:
				attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
				return true;

			case MOAIAttribute::SET:
				uniform->SetValue ( element, attr );
				return true;

			case MOAIAttribute::ADD:
				uniform->AddValue ( element, attr );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::GetAttributeID ( u32 uniformID, u32 index ) {

	return ( uniformID * MAX_UNIFORM_ARRAY_SIZE ) + index;
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter* MOAIShaderUniformBuffer::GetUniform ( u32 uniformID, void*& buffer ) {

	return this->MOAIShaderUniformBuffer_GetUniform ( uniformID, buffer );
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter* MOAIShaderUniformBuffer::GetUniform ( u32 uniformID, u32 index, void*& buffer ) {

	MOAIShaderUniformFormatter* uniform = this->GetUniform ( uniformID, buffer );
	buffer = uniform ? uniform->GetArrayItem ( buffer, index ) : 0;

	return uniform;
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter* MOAIShaderUniformBuffer::GetUniformForAttributeID ( u32 attrID, void*& buffer ) {

	attrID = ( attrID & MOAIAttribute::ATTR_ID_MASK ) - 1;

	u32 uniformID = attrID / MAX_UNIFORM_ARRAY_SIZE;
	u32 index = attrID - ( uniformID * MAX_UNIFORM_ARRAY_SIZE );
	
	return this->GetUniform ( uniformID, index, buffer );
}

//----------------------------------------------------------------//
MOAIShaderUniformBuffer::MOAIShaderUniformBuffer () {
}

//----------------------------------------------------------------//
MOAIShaderUniformBuffer::~MOAIShaderUniformBuffer () {
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::SetUniform ( lua_State* L, int idx, u32 uniformID, u32 index ) {

	MOAILuaState state ( L );

	void* element;
	MOAIShaderUniformFormatter* uniform = this->GetUniform ( uniformID, index, element );
	
	if ( uniform ) {
		
		int size = state.GetStackSize ( idx );
		
		if ( size <= 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( idx, ZLVec4D::ZERO );
			
			uniform->SetValue ( element, vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					uniform->SetValue ( element, state.GetValue < ZLAffine2D >( idx, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					uniform->SetValue ( element, state.GetValue < ZLMatrix3x3 >( idx, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					uniform->SetValue ( element, state.GetValue < ZLAffine3D >( idx, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					uniform->SetValue ( element, state.GetValue < ZLMatrix4x4 >( idx, ZLMatrix4x4::IDENT ));
					break;
				
				default:
					MOAILogF ( L, ZLLog::LOG_ERROR, "Error setting matrix. Did not recognize number of paramaters (6, 9, 12 or 16)." );
			}
		}
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//

