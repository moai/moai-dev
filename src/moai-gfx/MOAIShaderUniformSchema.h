// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMSCHEMA_H
#define	MOAISHADERUNIFORMSCHEMA_H

#include <moai-gfx/MOAIAbstractShaderUniformSchema.h>

//================================================================//
// MOAIShaderUniformSchema
//================================================================//
class MOAIShaderUniformSchema :
	public virtual MOAIAbstractShaderUniformSchema {
public:
	
	//----------------------------------------------------------------//
	bool		ApplyAttrOp						( void* buffer, ZLAttrID attrID, ZLAttribute& attr, u32 op ) const;
	void		SetUniform						( lua_State* L, int idx, void* buffer, ZLIndex uniformID, ZLIndex index ) const;
				MOAIShaderUniformSchema			();
				~MOAIShaderUniformSchema		();
};

#endif
