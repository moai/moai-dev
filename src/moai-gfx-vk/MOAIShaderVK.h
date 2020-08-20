// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERVK_H
#define	MOAISHADERVK_H

class MOAIDescriptorSetArrayVK;
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

	ZLStrongPtr < MOAIShaderProgramVK >				mProgram;
	ZLStrongPtr < MOAIDescriptorSetArrayVK >		mDescriptorSetArray;

	//----------------------------------------------------------------//
//	static int					_getAttributeID				( lua_State* L );
//	static int					_setProgram					( lua_State* L );
//	static int					_setUniform					( lua_State* L );
//	static int					_setUniformArrayItem		( lua_State* L );

	//----------------------------------------------------------------//
//	bool						IsDirty						();

	//----------------------------------------------------------------//
	MOAIGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		();
	bool						MOAINode_ApplyAttrOp						( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void						MOAIShader_ComposeUniforms					();
	bool						MOAIShader_IsReadyForUse					() const;
	void						MOAIShader_SelectTextures					();

public:

	GET ( MOAIShaderProgramVK*, Program, mProgram )
	GET ( MOAIDescriptorSetArrayVK*, DescriptorSetArray, mDescriptorSetArray )

	DECL_LUA_FACTORY ( MOAIShaderVK )

	//----------------------------------------------------------------//
								MOAIShaderVK				();
								~MOAIShaderVK				();
	void						SetProgram					( MOAIShaderProgramVK* program );
};

#endif
