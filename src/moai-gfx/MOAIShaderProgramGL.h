// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMGL_H
#define	MOAISHADERPROGRAMGL_H

#include <moai-gfx/MOAIGfxResourceGL.h>
#include <moai-gfx/MOAIShaderUniformSchemaBaseGL.h>

//================================================================//
// MOAIShaderProgramGL
//================================================================//
/**	@lua	MOAIShaderProgramGL
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShaderGL instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class MOAIShaderProgramGL :
	public MOAIGfxResourceGL < ZLShaderProgramGL >,
	public virtual MOAIShaderUniformSchemaBaseGL {
protected:

	//----------------------------------------------------------------//
	static int			_declareUniform				( lua_State* L );
	static int			_load						( lua_State* L );
	static int			_reserveGlobals				( lua_State* L );
	static int			_reserveTextures			( lua_State* L );
	static int			_reserveUniforms			( lua_State* L );
	static int			_setGlobal					( lua_State* L );
	static int			_setTexture					( lua_State* L );
	static int			_setVertexAttribute			( lua_State* L );
	
	//----------------------------------------------------------------//
//	int					ReserveGlobals 				( lua_State* L, int idx );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramGL )

	//----------------------------------------------------------------//
						MOAIShaderProgramGL			();
						~MOAIShaderProgramGL		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
