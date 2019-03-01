// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAM_H
#define	MOAISHADERPROGRAM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/ZLShaderProgram.h>
#include <moai-sim/ZLShaderUniform.h>

//================================================================//
// MOAIShaderProgram
//================================================================//
/**	@lua	MOAIShaderProgram
	@text	Programmable shader class. This represents the shader
			program itself, which may be shared by one or more
			MOAIShader instances. The shader program holds the graphics
			resource, and the shader instances apply the (non-global)
			uniform values.
*/
class MOAIShaderProgram :
	public MOAIGfxResource < ZLShaderProgram > {
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

	DECL_LUA_FACTORY ( MOAIShaderProgram )

	//----------------------------------------------------------------//
						MOAIShaderProgram			();
						~MOAIShaderProgram			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
