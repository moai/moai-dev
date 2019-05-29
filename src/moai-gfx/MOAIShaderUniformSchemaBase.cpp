// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShaderUniformSchemaBase.h>

//================================================================//
// MOAIShaderUniformSchemaBase
//================================================================//

//----------------------------------------------------------------//
bool MOAIShaderUniformSchemaBase::ApplyAttrOp ( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const {
	
	MOAIShaderUniformHandle uniform = this->GetUniformHandleForAttributeID ( buffer, attrID );
	
	if ( uniform.IsValid ()) {
	
		switch ( op ) {

			case ZLAttribute::CHECK:
				attr.SetFlags ( ZLAttribute::ATTR_WRITE );
				return true;

			case ZLAttribute::SET:
				uniform.SetValue ( attr );
				return true;

			case ZLAttribute::ADD:
				uniform.AddValue ( attr );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIShaderUniformSchemaBase::MOAIShaderUniformSchemaBase () {
}

//----------------------------------------------------------------//
MOAIShaderUniformSchemaBase::~MOAIShaderUniformSchemaBase () {
}

//----------------------------------------------------------------//
void MOAIShaderUniformSchemaBase::SetUniform ( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const {

	MOAILuaState state ( L );

	MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, index );
	
	if ( uniform.IsValid ()) {
		
		int size = state.GetStackSize ( idx );
		
		if ( size <= 4 ) {
		
			ZLVec4D vec = state.GetValue < ZLVec4D >( idx, ZLVec4D::ZERO );
			
			uniform.SetValue ( vec );
		}
		else {

			ZLMatrix4x4 mtx = ZLMatrix4x4::IDENT;

			switch ( size ) {
			
				case 6:
					uniform.SetValue ( state.GetValue < ZLAffine2D >( idx, ZLAffine2D::IDENT ));
					break;
				
				case 9:
					uniform.SetValue ( state.GetValue < ZLMatrix3x3 >( idx, ZLMatrix3x3::IDENT ));
					break;
				
				case 12:
					uniform.SetValue ( state.GetValue < ZLAffine3D >( idx, ZLAffine3D::IDENT ));
					break;
				
				case 16:
					uniform.SetValue ( state.GetValue < ZLMatrix4x4 >( idx, ZLMatrix4x4::IDENT ));
					break;
				
				default:
					MOAILogF ( L, ZLLog::LOG_ERROR, "Error setting matrix. Did not recognize number of paramaters (6, 9, 12 or 16)." );
			}
		}
	}
}
