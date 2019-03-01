// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTSHADERUNIFORMSCHEMA_H
#define	ZLABSTRACTSHADERUNIFORMSCHEMA_H

#include <moai-sim/ZLShaderUniformHandle.h>

class MOAIAttribute;

//================================================================//
// ZLAbstractShaderUniformSchema
//================================================================//
class ZLAbstractShaderUniformSchema {
protected:

	static const ZLSize					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	virtual ZLShaderUniformHandle		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const = 0;

public:
	
	//----------------------------------------------------------------//
	bool							ApplyAttrOp							( void* buffer, MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) const;
	u32								GetAttributeID						( u32 uniformID, u32 index ) const;
	ZLShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID ) const;
	ZLShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index ) const;
	ZLShaderUniformHandle			GetUniformHandleForAttributeID		( void* buffer, MOAIAttrID attrID ) const;
	void							SetUniform							( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const;
									ZLAbstractShaderUniformSchema		();
	virtual							~ZLAbstractShaderUniformSchema		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniform ( void* buffer, ZLIndex uniformID, const TYPE& value ) const {

		ZLShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, ZLIndexOp::ZERO );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
