// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERGL_H
#define	MOAISHADERGL_H

#include <moai-gfx/MOAIShaderProgramGL.h>

//================================================================//
// MOAIShaderGL
//================================================================//
/**	@lua	MOAIShaderGL
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShaderGL :
	public virtual MOAINode,
	public virtual ZLShaderGL {
protected:

	//----------------------------------------------------------------//
	static int				_getAttributeID				( lua_State* L );
	static int				_setProgram					( lua_State* L );
	static int				_setUniform					( lua_State* L );
	static int				_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIShaderGL )

	//----------------------------------------------------------------//
							MOAIShaderGL			();
							~MOAIShaderGL			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
