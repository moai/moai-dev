// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMGL_H
#define	MOAISHADERPROGRAMGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>
#include <moai-gfx-gl/MOAIShaderUniformBindingGL.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class MOAIShaderGL;
class MOAITextureGL;

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
	public virtual MOAIGfxResourceGL,
	public virtual MOAIUniformSchema,
	public virtual MOAIHasUniformComposer {
protected:

	friend class MOAIShaderGL;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle		mProgram;
	ZLGfxHandle		mVertexShader;
	ZLGfxHandle		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniformBindingGL >	mUniformBindings;

	size_t										mUniformBufferSize;
	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
	static int					_declareUniform				( lua_State* L );
	static int					_load						( lua_State* L );
	static int					_reserveUniforms			( lua_State* L );
	static int					_setVertexAttribute			( lua_State* L );

	//----------------------------------------------------------------//
	void						AffirmUniforms				();
	void						ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void						BindUniforms				();
	ZLGfxHandle					CompileShader				( ZLGfxEnum::_ type, cc8* source );
	void						InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
	
	//----------------------------------------------------------------//
	void						MOAIGfxResourceGL_OnGPUBind							();
	bool						MOAIGfxResourceGL_OnGPUCreate						();
	void						MOAIGfxResourceGL_OnGPUDeleteOrDiscard				( bool shouldDelete );
	void						MOAIGfxResourceGL_OnGPUUnbind						();
	bool						MOAIGfxResourceGL_OnGPUUpdate						();
	void						MOAILuaObject_RegisterLuaClass						( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs						( MOAIComposer& composer, MOAILuaState& state );
	void						ZLGfxListener_OnUniformLocation						( u32 addr, void* userdata );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramGL )

	//----------------------------------------------------------------//
	void				DeleteShaders				();
	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	void				Load						( cc8* vshSource, cc8* fshSource );
	void				ReserveAttributes			( ZLSize nAttributes );
	void				ReserveUniforms				( ZLSize nUniforms );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						MOAIShaderProgramGL			();
						~MOAIShaderProgramGL		();
};

#endif
