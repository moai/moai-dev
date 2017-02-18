// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moai-sim/MOAIShaderUniformBuffer.h>
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
	public MOAIShaderUniformBuffer {
protected:

	friend class MOAIGfxStateCache;
	friend class MOAIShaderProgram;

	MOAILuaSharedPtr < MOAIShaderProgram >		mProgram;
	ZLLeanArray < MOAIShaderUniformInstance >	mUniformInstances;
	ZLLeanArray < u8 >							mUniformBuffer;
	size_t										mMaxCount;

	//----------------------------------------------------------------//
	static int				_getAttributeID				( lua_State* L );
	static int				_resizeUniformArray			( lua_State* L );
	static int				_setProgram					( lua_State* L );
	static int				_setUniform					( lua_State* L );
	static int				_setUniformArrayItem		( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ApplyGlobals				();
	void					BindUniforms				();
	void					UpdateAndBindUniforms		();

	//----------------------------------------------------------------//
	bool								MOAINode_ApplyAttrOp						( u32 attrID, MOAIAttribute& attr, u32 op );
	MOAIShaderUniformFormatter*			MOAIShaderUniformBuffer_GetUniform			( u32 uniformID, void*& buffer );

public:

	DECL_LUA_FACTORY ( MOAIShader )

	GET ( MOAIShaderProgram*, Program, mProgram )
	GET ( size_t, UniformBufferSize, mUniformBuffer.Size ())
	GET_CONST ( void*, UniformBuffer, mUniformBuffer.Data ())

	//----------------------------------------------------------------//
	static MOAIShader*		AffirmShader			( MOAILuaState& state, int idx );
	void					BindTextures			();
	void					Bless					();
							MOAIShader				();
							~MOAIShader				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ResizeUniformArray		( u32 uniformID, u32 count );
	void					SetProgram				( MOAIShaderProgram* program );
};

#endif
