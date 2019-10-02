// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERVK_H
#define	MOAISHADERVK_H

class MOAIShaderProgramVK;

//================================================================//
// MOAIShaderVK
//================================================================//
/**	@lua	MOAIShaderVK
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShaderVK :
	public virtual MOAIShader,
	public virtual MOAINode {
protected:

	ZLStrongPtr < MOAIShaderProgramVK >	mProgram;

	//----------------------------------------------------------------//
//	static int				_getAttributeID				( lua_State* L );
//	static int				_setProgram					( lua_State* L );
//	static int				_setUniform					( lua_State* L );
//	static int				_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
//	bool					IsDirty						();

	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	bool					MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAIShader_ComposeUniforms			();
	bool					MOAIShader_IsReadyForUse			() const;
	void					MOAIShader_SelectTextures			();

public:

	GET ( MOAIShaderProgramVK*, Program, mProgram )

	DECL_LUA_FACTORY ( MOAIShaderVK )

	//----------------------------------------------------------------//
//	void					ApplyUniforms				();
//	void					BindUniforms				();
//	void					Bless						();
//	bool					HasDirtyUniforms			();
							MOAIShaderVK				();
							~MOAIShaderVK				();
//	void					ResizeUniformArray			( u32 uniformID, u32 count );
//	void					ScheduleTextures			();
	void					SetProgram					( MOAIShaderProgramVK* program );
};

#endif
