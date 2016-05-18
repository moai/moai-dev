// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
			
			As uniform values change, they will "dirty" the underlying shader
			program and cause its values to re-bind prior to drawing. If minimal
			re-binds are desired, simply create one shader program per shader
			instance.
*/
class MOAIShader :
	public virtual MOAINode {
protected:

	friend class MOAIGfxStateCache;
	friend class MOAIShaderProgram;

	MOAILuaSharedPtr < MOAIShaderProgram >		mProgram;
	ZLLeanArray < MOAIShaderUniformBuffer >		mUniformBuffers;

	//----------------------------------------------------------------//
	static int				_setProgram				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					UpdateAndBindUniforms	();

public:

	DECL_LUA_FACTORY ( MOAIShader )

	GET ( MOAIShaderProgram*, Program, mProgram )

	//----------------------------------------------------------------//
	static MOAIShader*		AffirmShader			( MOAILuaState& state, int idx );
	bool					ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
							MOAIShader				();
							~MOAIShader				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SetProgram				( MOAIShaderProgram* program );
};

#endif
