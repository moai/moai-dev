// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moai-sim/MOAIShaderProgram.h>

//================================================================//
// MOAIShader
//================================================================//
/**	@lua	MOAIShader
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShader :
	public virtual MOAINode,
	public virtual ZLShader {
protected:

	//----------------------------------------------------------------//
	static int				_getAttributeID				( lua_State* L );
	static int				_setProgram					( lua_State* L );
	static int				_setUniform					( lua_State* L );
	static int				_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIShader )

	//----------------------------------------------------------------//
	static MOAIShader*		AffirmShader			( MOAILuaState& state, int idx );
							MOAIShader				();
							~MOAIShader				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
