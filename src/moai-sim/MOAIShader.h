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
	public MOAIShaderGlobals {
protected:

	friend class MOAIGfxStateCache;
	friend class MOAIShaderProgram;

	MOAILuaSharedPtr < MOAIShaderProgram >		mProgram;
	ZLLeanArray < MOAIShaderUniformInstance >	mUniformInstances;
	ZLLeanArray < u8 >							mUniformBuffer;
	size_t										mMaxCount;

	//----------------------------------------------------------------//
	static int				_getAttributeID			( lua_State* L );
	static int				_reserveGlobals			( lua_State* L );
	static int				_resizeUniformArray		( lua_State* L );
	static int				_setGlobal				( lua_State* L );
	static int				_setProgram				( lua_State* L );
	static int				_setUniform				( lua_State* L );
	static int				_setUniformComponent	( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ApplyGlobals				();
	void					BindUniforms				();
	u32						GetAttributeID				( u32 uniformID, u32 index );
	MOAIShaderUniform*		GetUniform					( u32 uniformID );
	MOAIShaderUniform*		GetUniform					( u32 uniformID, u32 index, void*& element );
	MOAIShaderUniform*		GetUniformForAttributeID	( u32 attrID, void*& element );
	void					SetUniform					( lua_State* L, int idx, u32 uniformID, u32 index );
	void					UpdateAndBindUniforms		();

	//----------------------------------------------------------------//
	bool					MOAINode_ApplyAttrOp		( u32 attrID, MOAIAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIShader )

	GET ( MOAIShaderProgram*, Program, mProgram )

	//----------------------------------------------------------------//
	static MOAIShader*		AffirmShader			( MOAILuaState& state, int idx );
	void					Bless					();
							MOAIShader				();
							~MOAIShader				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ResizeUniformArray		( u32 uniformID, u32 count );
	void					SetProgram				( MOAIShaderProgram* program );
};

#endif
