// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNIFORMSCHEMA_H
#define	MOAIUNIFORMSCHEMA_H

#include <moai-gfx/MOAIUniformHandle.h>

//================================================================//
// MOAIUniformSchema
//================================================================//
class MOAIUniformSchema {
protected:

	ZLLeanArray < MOAIUniformDescriptor >	mUniformDescriptors;

public:
	
	static const ZLSize			MAX_UNIFORM_ARRAY_SIZE = 256;

	//----------------------------------------------------------------//
	bool						ApplyAttrOp							( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const;
	u32							GetAttributeID						( u32 uniformID, ZLIndex index = 0 ) const;
	MOAIUniformHandle			GetUniformHandle					( void* buffer, ZLIndex uniformID, ZLIndex index = 0 ) const;
	MOAIUniformHandle			GetUniformHandleForAttributeID		( void* buffer, ZLAttrID attrID ) const;
	void						SetUniformValue						( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index = 0 ) const;
								MOAIUniformSchema					();
								~MOAIUniformSchema					();
	ZLSize						UpdateUniformOffsets				();

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetUniformValue ( void* buffer, ZLIndex uniformID, const TYPE& value ) const {

		MOAIUniformHandle uniform = this->GetUniformHandle ( buffer, uniformID, 0 );
		assert ( uniform.IsValid ());
		uniform.SetValue ( value );
	}
};

#endif
