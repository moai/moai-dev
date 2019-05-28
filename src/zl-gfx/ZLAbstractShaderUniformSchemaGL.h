// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTSHADERUNIFORMSCHEMAGL_H
#define	ZLABSTRACTSHADERUNIFORMSCHEMAGL_H

#include <zl-gfx/ZLShaderUniformHandleGL.h>

class ZLAttribute;

//================================================================//
// ZLAbstractShaderUniformSchemaGL
//================================================================//
class ZLAbstractShaderUniformSchemaGL :
	public virtual RTTIBase {
protected:

	static const ZLSize					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	virtual ZLShaderUniformHandleGL		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const = 0;

public:
	
	//----------------------------------------------------------------//
	u32								GetAttributeID						( u32 uniformID, u32 index ) const;
	ZLShaderUniformHandleGL			GetUniformHandle					( void* buffer, ZLIndex uniformID ) const;
	ZLShaderUniformHandleGL			GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index ) const;
	ZLShaderUniformHandleGL			GetUniformHandleForAttributeID		( void* buffer, ZLAttrID attrID ) const;
									ZLAbstractShaderUniformSchemaGL		();
	virtual							~ZLAbstractShaderUniformSchemaGL	();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniform ( void* buffer, ZLIndex uniformID, const TYPE& value ) const {

		ZLShaderUniformHandleGL uniform = this->GetUniformHandle ( buffer, uniformID, ZLIndexOp::ZERO );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
