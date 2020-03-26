// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERGL_H
#define	MOAISHADERGL_H

class MOAIShaderProgramGL;

// MOAIShaderUniformComposerInterface
// MOAIShaderUniformComposer
// MOAIHasShaderUniformComposer

//================================================================//
// MOAIShaderGL
//================================================================//
/**	@lua	MOAIShaderGL
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShaderGL :
	public virtual MOAIShader,
	public virtual MOAIAbstractUniformBuffer {
protected:

	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class MOAIShaderProgramGL;

	ZLStrongPtr < MOAIShaderProgramGL >	mProgram;

	//----------------------------------------------------------------//
	static int				_setProgram					( lua_State* L );
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript*			MOAIAbstractHasGfxScript_GetGfxScript	();
	const MOAIUniformSchema*		MOAIAbstractUniformBuffer_GetSchema		() const;
	MOAIUniformHandle				MOAIShader_GetUniformHandle				( ZLIndex uniformID, ZLIndex index );
	bool							MOAIShader_IsReadyForUse				() const;

public:

	GET ( MOAIShaderProgramGL*, Program, mProgram )

	DECL_LUA_FACTORY ( MOAIShaderGL )

	//----------------------------------------------------------------//
	void					ApplyUniforms				();
	void					BindUniforms				();
	void					Bless						();
	bool					HasDirtyUniforms			();
							MOAIShaderGL				();
							~MOAIShaderGL				();
	void					ResizeUniformArray			( u32 uniformID, u32 count );
	void					SetProgram					( MOAIShaderProgramGL* program );
};

#endif
