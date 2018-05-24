// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moai-sim/MOAIShaderUniformSchema.h>
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
	public virtual MOAINode {
protected:

	friend class MOAIGfxStateCache;
	friend class MOAIShaderProgram;

	MOAILuaSharedPtr < MOAIShaderProgram >		mProgram;
	ZLLeanArray < u8 >							mUniformBuffer;
	ZLLeanArray < u8 >							mPendingUniformBuffer;

	//----------------------------------------------------------------//
	static int				_getAttributeID				( lua_State* L );
	static int				_setProgram					( lua_State* L );
	static int				_setUniform					( lua_State* L );
	static int				_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( u32 attrID, MOAIAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIShader )

	GET ( MOAIShaderProgram*, Program, mProgram )
	GET ( size_t, UniformBufferSize, mUniformBuffer.Size ())
	GET_CONST ( void*, UniformBuffer, mUniformBuffer.Data ())

	//----------------------------------------------------------------//
	void					ApplyUniforms			();
	static MOAIShader*		AffirmShader			( MOAILuaState& state, int idx );
	void					BindUniforms			();
	void					Bless					();
	bool					IsDirty					();
							MOAIShader				();
							~MOAIShader				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ResizeUniformArray		( u32 uniformID, u32 count );
	void					ScheduleTextures		();
	void					SetProgram				( MOAIShaderProgram* program );
	void					UpdateUniforms			();
};

#endif
