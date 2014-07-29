// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGR_H
#define	MOAISHADERMGR_H

class MOAIShader;
class MOAIShaderProgram;

//================================================================//
// MOAIShaderMgr
//================================================================//
/**	@lua	MOAIShaderMgr
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
class MOAIShaderMgr :
	public MOAIGlobalClass < MOAIShaderMgr, MOAILuaObject > {
public:

	enum {
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

private:
	
	MOAIShaderProgram*	mPrograms [ TOTAL_SHADERS ];
	MOAIShader*			mShaders [ TOTAL_SHADERS ];
	
	//----------------------------------------------------------------//
	static int				_getProgram				( lua_State* L );
	static int				_getShader				( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgr )
	
	//----------------------------------------------------------------//
	void					BindShader				( u32 shaderID );
	MOAIShaderProgram&		GetProgram				( u32 shaderID );
	MOAIShader&				GetShader				( u32 shaderID );
							MOAIShaderMgr			();
							~MOAIShaderMgr			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
