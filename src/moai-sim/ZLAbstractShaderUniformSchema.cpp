// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAttribute.h>
#include <moai-sim/ZLAbstractShaderUniformSchema.h>

//================================================================//
// ZLAbstractShaderUniformSchema
//================================================================//

//----------------------------------------------------------------//
bool ZLAbstractShaderUniformSchema::ApplyAttrOp ( void* buffer, MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) const {
			
	ZLShaderUniformHandle uniform = this->GetUniformHandleForAttributeID ( buffer, attrID );
	
	if ( uniform.IsValid ()) {
	
		switch ( op ) {

			case MOAIAttribute::CHECK:
				attr.SetFlags ( MOAIAttribute::ATTR_WRITE );
				return true;

			case MOAIAttribute::SET:
				uniform.SetValue ( attr );
				return true;

			case MOAIAttribute::ADD:
				uniform.AddValue ( attr );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
u32 ZLAbstractShaderUniformSchema::GetAttributeID ( u32 uniformID, u32 index ) const {

	return ( uniformID * MAX_UNIFORM_ARRAY_SIZE ) + index;
}

//----------------------------------------------------------------//
ZLShaderUniformHandle ZLAbstractShaderUniformSchema::GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	return this->ZLAbstractShaderUniformSchema_GetUniformHandle ( buffer, uniformID );
}

//----------------------------------------------------------------//
ZLShaderUniformHandle ZLAbstractShaderUniformSchema::GetUniformHandle ( void* buffer, ZLIndex uniformID, ZLIndex index ) const {

	ZLShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID );
	return uniform.IsValid () ? uniform.Offset ( index ) : uniform;
}

//----------------------------------------------------------------//
ZLShaderUniformHandle ZLAbstractShaderUniformSchema::GetUniformHandleForAttributeID ( void* buffer, MOAIAttrID attrID ) const {

	// TODO: check for overflow

	u32 rawID = attrID.Unpack ();

	ZLSize uniformID = rawID / MAX_UNIFORM_ARRAY_SIZE;
	ZLSize index = rawID - ( uniformID * MAX_UNIFORM_ARRAY_SIZE );
	
	return this->GetUniformHandle ( buffer, ZLIndexCast ( uniformID ), ZLIndexCast ( index ));
}

//----------------------------------------------------------------//
ZLAbstractShaderUniformSchema::ZLAbstractShaderUniformSchema () {
}

//----------------------------------------------------------------//
ZLAbstractShaderUniformSchema::~ZLAbstractShaderUniformSchema () {
}

//----------------------------------------------------------------//
void ZLAbstractShaderUniformSchema::SetUniform ( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const {

	MOAILuaState state ( L );

	ZLShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, index );
	
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
