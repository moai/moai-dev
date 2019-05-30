// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMSCHEMA_H
#define	MOAISHADERUNIFORMSCHEMA_H

#include <moai-gfx/MOAIShaderUniformHandle.h>

class ZLAttribute;

//================================================================//
// MOAIShaderUniformSchema
//================================================================//
class MOAIShaderUniformSchema {
protected:

	static const ZLSize					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	virtual MOAIShaderUniformHandle	ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const = 0;

public:
	
	//----------------------------------------------------------------//
	u32								GetAttributeID						( u32 uniformID, u32 index ) const;
	MOAIShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID ) const;
	MOAIShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index ) const;
	MOAIShaderUniformHandle			GetUniformHandleForAttributeID		( void* buffer, ZLAttrID attrID ) const;
									MOAIShaderUniformSchema		();
	virtual							~MOAIShaderUniformSchema		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniform ( void* buffer, ZLIndex uniformID, const TYPE& value ) const {

		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, ZLIndexOp::ZERO );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
