// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moai-sim/MOAIShaderProgram.h>

//================================================================//
// MOAIShader
//================================================================//
// TODO: doxygen
class MOAIShader :
	public virtual MOAINode {
protected:

	friend class MOAIGfxDevice;

	MOAILuaSharedPtr < MOAIShaderProgram >		mProgram;
	ZLLeanArray < MOAIShaderUniformBuffer >		mUniformBuffers;

	//----------------------------------------------------------------//
	static int		_setProgram				( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIShader )

	GET ( MOAIShaderProgram*, Program, mProgram )

	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			BindUniforms			();
					MOAIShader				();
					~MOAIShader				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetProgram				( MOAIShaderProgram* program );
};

#endif
