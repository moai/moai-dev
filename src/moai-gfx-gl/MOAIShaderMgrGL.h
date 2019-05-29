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
	@const DECK2D_TEX_ONLY_SHADER
	@const FONT_SHADER
	@const FONT_SNAPPING_SHADER
	@const FONT_EFFECTS_SHADER
	@const LINE_SHADER
	@const LINE_SHADER_3D
	@const MESH_SHADER
*/
class MOAIShaderMgrGL :
	public ZLContextClass < MOAIShaderMgrGL >,
	public virtual MOAILuaObject {
public:

	enum Preset {
		DECK2D_SHADER,
		DECK2D_SNAPPING_SHADER,
		DECK2D_TEX_ONLY_SHADER,
		FONT_SHADER,
		FONT_SNAPPING_SHADER,
		FONT_EFFECTS_SHADER,
		LINE_SHADER,
		LINE_SHADER_3D,
		MESH_SHADER,
		TOTAL_SHADERS,
	};

	static const u32 UNKNOWN_SHADER = 0xffffffff;

private:
	
	MOAIShaderProgramGL*	mPrograms [ TOTAL_SHADERS ];
	MOAIShaderGL*			mShaders [ TOTAL_SHADERS ];
	
	//----------------------------------------------------------------//
	static int				_getProgram				( lua_State* L );
	static int				_getShader				( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgrGL )
	
	//----------------------------------------------------------------//
	void					AffirmAll				();
	MOAIShaderProgramGL*	GetProgram				( u32 shaderID );
	MOAIShaderGL*			GetShader				( u32 shaderID );
	MOAIShaderGL*			GetShader				( lua_State* L, int idx );
							MOAIShaderMgrGL			();
							~MOAIShaderMgrGL		();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
