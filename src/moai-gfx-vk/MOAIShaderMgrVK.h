// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGRVK_H
#define	MOAISHADERMGRVK_H

class MOAIShaderVK;
class MOAIShaderProgramVK;

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
	
	MOAIShaderProgramVK*	mPrograms [ TOTAL_SHADERS ];
	MOAIShaderVK*			mShaders [ TOTAL_SHADERS ];

	//----------------------------------------------------------------//
//	static int				_getProgram				( lua_State* L );
//	static int				_getShader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					AffirmAll				();
	
	//----------------------------------------------------------------//
	void					MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgrVK )
	
	//----------------------------------------------------------------//
	MOAIShaderProgramVK*	GetProgram				( MOAIShaderPresetEnum shaderID );
	MOAIShaderVK*			GetShader				( MOAIShaderPresetEnum shaderID );
//	MOAIShaderVK*			GetShader				( lua_State* L, int idx );
							MOAIShaderMgrVK			();
							~MOAIShaderMgrVK		();
};

#endif
