// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGRVK_H
#define	MOAISHADERMGRVK_H

class MOAIDescriptorSetLayoutVK;
class MOAIGfxComposerVK;
class MOAIShaderVK;
class MOAIShaderProgramVK;
class MOAIPipelineLayoutVK;

//================================================================//
// MOAIShaderMgrVK
//================================================================//
/**	@lua	MOAIShaderMgrVK
	@text	Shader presets.
	
	@const DECK2D_SHADER
	@const DECK2D_SNAPPING_SHADER
	@const FONT_SHADER
	@const FONT_SNAPPING_SHADER
	@const FONT_EFFECTS_SHADER
	@const LINE_SHADER
	@const LINE_SHADER_3D
	@const MESH_SHADER
*/
class MOAIShaderMgrVK :
	public ZLContextClass < MOAIShaderMgrVK >,
	public virtual MOAILuaObject {
private:
	
	// common layouts for simple shaders that just use a single texture
	ZLStrongPtr < MOAIDescriptorSetLayoutVK >	mOneTexDescriptorSetLayout;
	ZLStrongPtr < MOAIPipelineLayoutVK >		mOneTexPipelineLayout;
	ZLStrongPtr < MOAIGfxComposerVK >			mOneTexComposer;
	
	MOAIShaderProgramVK*	mPrograms [ TOTAL_SHADERS ];
	MOAIShaderVK*			mShaders [ TOTAL_SHADERS ];

	//----------------------------------------------------------------//
//	static int				_getProgram				( lua_State* L );
//	static int				_getShader				( lua_State* L );
	
	//----------------------------------------------------------------//

public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgrVK )
	
	//----------------------------------------------------------------//
	void					AffirmAll				();
	MOAIShaderProgramVK*	GetProgram				( MOAIShaderPresetEnum shaderID );
	MOAIShaderVK*			GetShader				( MOAIShaderPresetEnum shaderID );
//	MOAIShaderVK*			GetShader				( lua_State* L, int idx );
							MOAIShaderMgrVK			();
							~MOAIShaderMgrVK		();
};

#endif
