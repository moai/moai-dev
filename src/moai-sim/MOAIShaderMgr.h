// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERMGR_H
#define	MOAISHADERMGR_H

class MOAIShader;

//================================================================//
// MOAIShaderMgr
//================================================================//
/**	@name	MOAIShaderMgr
	@text	Shader presets.
	
	const DECK2D_SHADER
	const DECK2D_TEX_ONLY_SHADER
	const FONT_SHADER
	const LINE_SHADER
	const MESH_SHADER
*/
class MOAIShaderMgr :
	public MOAIGlobalClass < MOAIShaderMgr, MOAILuaObject > {
public:

	enum {
		DECK2D_SHADER,
		DECK2D_TEX_ONLY_SHADER,
		FONT_SHADER,
		LINE_SHADER,
		MESH_SHADER,
		TOTAL_SHADERS,
	};

private:
	
	MOAIShader* mShaders [ TOTAL_SHADERS ];
	
	//----------------------------------------------------------------//
	static int			_getShader			( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIShaderMgr )
	
	//----------------------------------------------------------------//
	void				BindShader			( u32 shaderID );
	MOAIShader&			GetShader			( u32 shaderID );
						MOAIShaderMgr		();
						~MOAIShaderMgr		();
	void				RegisterLuaClass	( MOAILuaState& state );
	void				RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
