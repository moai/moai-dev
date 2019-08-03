// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMSCHEMA_H
#define	MOAISHADERUNIFORMSCHEMA_H

#include <moai-gfx/MOAIShaderUniformHandle.h>

//================================================================//
// MOAIShaderUniformSchema
//================================================================//
class MOAIShaderUniformSchema {
protected:

	ZLLeanArray < MOAIShaderUniformDescriptor >	mUniformDescriptors;

public:
	
	static const ZLSize					MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	bool						ApplyAttrOp							( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const;
	u32							GetAttributeID						( u32 uniformID, ZLIndex index = ZLIndexOp::ZERO ) const;
	MOAIShaderUniformHandle		GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index = ZLIndexOp::ZERO ) const;
	MOAIShaderUniformHandle		GetUniformHandleForAttributeID		( void* buffer, ZLAttrID attrID ) const;
	void						SetUniformValue						( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index = ZLIndexOp::ZERO ) const;
								MOAIShaderUniformSchema				();
								~MOAIShaderUniformSchema			();
	ZLSize						UpdateUniformOffsets				();

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniformValue ( void* buffer, ZLIndex uniformID, const TYPE& value ) const {

		MOAIShaderUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, ZLIndexOp::ZERO );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
