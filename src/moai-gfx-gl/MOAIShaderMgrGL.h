// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGRGL_H
#define	MOAISHADERMGRGL_H

class MOAIShaderGL;
class MOAIShaderProgramGL;

//================================================================//
// MOAIShaderMgrGL
//================================================================//
/**	@lua	MOAIShaderMgrGL
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
class MOAIShaderMgrGL :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:
	
	MOAIShaderProgramGL*	mPrograms [ TOTAL_SHADERS ];
	MOAIShaderGL*			mShaders [ TOTAL_SHADERS ];
	
	//----------------------------------------------------------------//
	static int				_getProgram				( lua_State* L );
	static int				_getShader				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgrGL )
	
	//----------------------------------------------------------------//
	void					AffirmAll				();
	MOAIShaderProgramGL*	GetProgram				( MOAIShaderPresetEnum shaderID );
	MOAIShaderGL*			GetShader				( MOAIShaderPresetEnum shaderID );
	MOAIShaderGL*			GetShader				( lua_State* L, int idx );
							MOAIShaderMgrGL			( ZLContext& context );
							~MOAIShaderMgrGL		();
};

#endif
