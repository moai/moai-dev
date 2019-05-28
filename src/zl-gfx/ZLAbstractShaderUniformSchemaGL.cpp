// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLAbstractShaderUniformSchemaGL.h>

//================================================================//
// ZLAbstractShaderUniformSchemaGL
//================================================================//

//----------------------------------------------------------------//
u32 ZLAbstractShaderUniformSchemaGL::GetAttributeID ( u32 uniformID, u32 index ) const {

	return ( uniformID * MAX_UNIFORM_ARRAY_SIZE ) + index;
}

//----------------------------------------------------------------//
ZLShaderUniformHandleGL ZLAbstractShaderUniformSchemaGL::GetUniformHandle ( void* buffer, ZLIndex uniformID ) const {

	return this->ZLAbstractShaderUniformSchema_GetUniformHandle ( buffer, uniformID );
}

//----------------------------------------------------------------//
ZLShaderUniformHandleGL ZLAbstractShaderUniformSchemaGL::GetUniformHandle ( void* buffer, ZLIndex uniformID, ZLIndex index ) const {

	ZLShaderUniformHandleGL uniform = this->GetUniformHandle ( buffer, uniformID );
	return uniform.IsValid () ? uniform.Offset ( index ) : uniform;
}

//----------------------------------------------------------------//
ZLShaderUniformHandleGL ZLAbstractShaderUniformSchemaGL::GetUniformHandleForAttributeID ( void* buffer, ZLAttrID attrID ) const {

	// TODO: check for overflow

	u32 rawID = attrID.Unpack ();

	ZLSize uniformID = rawID / MAX_UNIFORM_ARRAY_SIZE;
	ZLSize index = rawID - ( uniformID * MAX_UNIFORM_ARRAY_SIZE );
	
	return this->GetUniformHandle ( buffer, ZLIndexCast ( uniformID ), ZLIndexCast ( index ));
}

//----------------------------------------------------------------//
ZLAbstractShaderUniformSchemaGL::ZLAbstractShaderUniformSchemaGL () {

	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
ZLAbstractShaderUniformSchemaGL::~ZLAbstractShaderUniformSchemaGL () {
}
