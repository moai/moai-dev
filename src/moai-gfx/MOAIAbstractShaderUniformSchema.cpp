// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractShaderUniformSchema.h>

//================================================================//
// MOAIAbstractShaderUniformSchema
//================================================================//

//----------------------------------------------------------------//
u32 MOAIAbstractShaderUniformSchema::GetAttributeID ( u32 uniformID, u32 index ) const {

	return ( uniformID * MAX_UNIFORM_ARRAY_SIZE ) + index;
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAIAbstractShaderUniformSchema::GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	return this->MOAIAbstractShaderUniformSchema_GetUniformHandle ( buffer, uniformID );
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAIAbstractShaderUniformSchema::GetUniformHandle ( void* buffer, ZLIndex uniformID, ZLIndex index ) const {

	MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID );
	return uniform.IsValid () ? uniform.Offset ( index ) : uniform;
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAIAbstractShaderUniformSchema::GetUniformHandleForAttributeID ( void* buffer, ZLAttrID attrID ) const {

	// TODO: check for overflow

	u32 rawID = attrID.Unpack ();

	ZLSize uniformID = rawID / MAX_UNIFORM_ARRAY_SIZE;
	ZLSize index = rawID - ( uniformID * MAX_UNIFORM_ARRAY_SIZE );
	
	return this->GetUniformHandle ( buffer, ZLIndexCast ( uniformID ), ZLIndexCast ( index ));
}

//----------------------------------------------------------------//
MOAIAbstractShaderUniformSchema::MOAIAbstractShaderUniformSchema () {
}

//----------------------------------------------------------------//
MOAIAbstractShaderUniformSchema::~MOAIAbstractShaderUniformSchema () {
}
