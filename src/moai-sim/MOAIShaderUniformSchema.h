// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTSHADERUNIFORMSCHEMA_H
#define	MOAIABSTRACTSHADERUNIFORMSCHEMA_H

#include <moai-sim/MOAIShaderUniformHandle.h>

class MOAIAttribute;

//================================================================//
// MOAIShaderUniformSchema
//================================================================//
class MOAIShaderUniformSchema {
protected:

	static const u32					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	virtual MOAIShaderUniformHandle		MOAIShaderUniformSchema_GetUniformHandle		( void* buffer, u32 uniformID ) const = 0;

public:
	
	//----------------------------------------------------------------//
	bool							ApplyAttrOp							( void* buffer, u32 attrID, MOAIAttribute& attr, u32 op ) const;
	u32								GetAttributeID						( u32 uniformID, u32 index ) const;
	MOAIShaderUniformHandle			GetUniformHandle					( void* buffer, u32 uniformID ) const;
	MOAIShaderUniformHandle			GetUniformHandle					( void* buffer, u32 uniformID, u32 index ) const;
	MOAIShaderUniformHandle			GetUniformHandleForAttributeID		( void* buffer, u32 attrID ) const;
									MOAIShaderUniformSchema				();
	virtual							~MOAIShaderUniformSchema			();
	void							SetUniform							( lua_State* L, int idx, void* buffer, u32 uniformID, u32 index ) const;
	
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniform ( void* buffer, u32 uniformID, const TYPE& value ) const {

		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, 0 );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
