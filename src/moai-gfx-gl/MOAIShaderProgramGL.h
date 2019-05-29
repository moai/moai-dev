// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAMGL_H
#define	MOAISHADERPROGRAMGL_H

#include <moai-gfx-gl/MOAIAbstractGfxResourceGL.h>
#include <moai-gfx-gl/MOAIShaderUniformGL.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIShaderProgramGlobalGL
//================================================================//
class MOAIShaderProgramGlobalGL {
private:

	friend class MOAIShaderGL;
	friend class MOAIShaderGlobals;
	friend class MOAIShaderProgramGL;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramGlobalGL		();
};

//================================================================//
// MOAIShaderProgramTextureGL
//================================================================//
class MOAIShaderProgramTextureGL {
private:

	friend class MOAIShaderProgramGL;

	u32							mName;
	ZLIndex						mUnit;
	ZLStrongPtr < MOAITextureBaseGL >	mTexture;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramTextureGL		();
};

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
	public virtual MOAIAbstractGfxResourceGL,
	public virtual MOAIShaderUniformSchemaBase {
protected:

	friend class MOAIShaderGL;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle		mProgram;
	ZLGfxHandle		mVertexShader;
	ZLGfxHandle		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniformGL >			mUniforms;
	ZLLeanArray < MOAIShaderProgramGlobalGL >	mGlobals;
	ZLLeanArray < MOAIShaderProgramTextureGL >	mTextures;

	size_t										mMaxCount;
	size_t										mUniformBufferSize;

	ZLLeanArray < u8 >							mUniformBuffer;

	//----------------------------------------------------------------//
	static int					_declareUniform				( lua_State* L );
	static int					_load						( lua_State* L );
	static int					_reserveGlobals				( lua_State* L );
	static int					_reserveTextures			( lua_State* L );
	static int					_reserveUniforms			( lua_State* L );
	static int					_setGlobal					( lua_State* L );
	static int					_setTexture					( lua_State* L );
	static int					_setVertexAttribute			( lua_State* L );

	//----------------------------------------------------------------//
	void						AffirmUniforms				();
	void						ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void						BindUniforms				();
	ZLGfxHandle					CompileShader				( u32 type, cc8* source );
	MOAIShaderUniformGL*			GetUniform					( ZLIndex uniformID );
	void						InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
	int							ReserveGlobals 				( lua_State* L, int idx );
	void						ScheduleTextures			();
	void						UpdateUniforms				( ZLLeanArray < u8 >& buffer );
	
	//----------------------------------------------------------------//
	bool						ZLAbstractGfxResource_OnCPUCreate					();
	void						ZLAbstractGfxResource_OnCPUDestroy					();
	void						ZLAbstractGfxResource_OnGPUBind						();
	bool						ZLAbstractGfxResource_OnGPUCreate					();
	void						ZLAbstractGfxResource_OnGPUDeleteOrDiscard			( bool shouldDelete );
	void						ZLAbstractGfxResource_OnGPUUnbind					();
	bool						ZLAbstractGfxResource_OnGPUUpdate					();
	MOAIShaderUniformHandle		ZLAbstractShaderUniformSchema_GetUniformHandle		( void* buffer, ZLIndex uniformID ) const;
	void						ZLGfxListener_OnUniformLocation						( u32 addr, void* userdata );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgramGL )

	//----------------------------------------------------------------//
	void				Clear						();
	void				DeleteShaders				();
	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	void				Load						( cc8* vshSource, cc8* fshSource );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveAttributes			( ZLSize nAttributes );
	void				ReserveGlobals				( ZLSize nGlobals );
	void				ReserveTextures				( ZLSize nTextures );
	void				ReserveUniforms				( ZLSize nUniforms );
	void				SetGlobal					( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureBaseGL* fallback );
	void				SetTexture					( ZLIndex idx, MOAITextureBaseGL* texture, ZLIndex unit );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
						MOAIShaderProgramGL			();
						~MOAIShaderProgramGL		();
};

#endif
