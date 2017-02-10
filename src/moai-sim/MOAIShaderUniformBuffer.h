// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTSHADERUNIFORMARRAY_H
#define	MOAIABSTRACTSHADERUNIFORMARRAY_H

class MOAIAttribute;
class MOAIShaderUniformFormatter;

//================================================================//
// MOAIShaderUniformBuffer
//================================================================//
class MOAIShaderUniformBuffer {
protected:

	static const u32					MAX_UNIFORM_ARRAY_SIZE = 1024;

	//----------------------------------------------------------------//
	virtual MOAIShaderUniformFormatter*			MOAIShaderUniformBuffer_GetUniform			( u32 uniformID, void*& buffer ) = 0;

public:
	
	//----------------------------------------------------------------//
	bool								ApplyAttrOp						( u32 attrID, MOAIAttribute& attr, u32 op );
	u32									GetAttributeID					( u32 uniformID, u32 index );
	MOAIShaderUniformFormatter*			GetUniform						( u32 uniformID, void*& buffer );
	MOAIShaderUniformFormatter*			GetUniform						( u32 uniformID, u32 index, void*& buffer );
	MOAIShaderUniformFormatter*			GetUniformForAttributeID		( u32 attrID, void*& buffer );
										MOAIShaderUniformBuffer			();
	virtual								~MOAIShaderUniformBuffer		();
	void								SetUniform						( lua_State* L, int idx, u32 uniformID, u32 index );
};

#endif
