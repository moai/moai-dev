// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMSCHEMABASEGL_H
#define	MOAISHADERUNIFORMSCHEMABASEGL_H

//================================================================//
// MOAIShaderUniformSchemaBaseGL
//================================================================//
class MOAIShaderUniformSchemaBaseGL :
	public virtual ZLAbstractShaderUniformSchemaGL {
public:
	
	//----------------------------------------------------------------//
	bool		ApplyAttrOp							( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const;
	void		SetUniform							( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const;
				MOAIShaderUniformSchemaBaseGL		();
				~MOAIShaderUniformSchemaBaseGL		();
};

#endif
