// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTSHADERUNIFORMSCHEMA_H
#define	ZLABSTRACTSHADERUNIFORMSCHEMA_H

#include <zl-gfx/ZLShaderUniformHandle.h>

class ZLAttribute;

//================================================================//
// ZLAbstractShaderUniformSchema
//================================================================//
class ZLAbstractShaderUniformSchema :
	public virtual RTTIBase {
protected:

	static const ZLSize					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	virtual ZLShaderUniformHandle		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const = 0;

public:
	
	//----------------------------------------------------------------//
	u32								GetAttributeID						( u32 uniformID, u32 index ) const;
	ZLShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID ) const;
	ZLShaderUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index ) const;
	ZLShaderUniformHandle			GetUniformHandleForAttributeID		( void* buffer, ZLAttrID attrID ) const;
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
