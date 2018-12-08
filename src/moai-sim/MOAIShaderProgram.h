// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAM_H
#define	MOAISHADERPROGRAM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAIShaderUniform.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n #define HIGHP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n #define HIGHP highp\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

class MOAIShader;
class MOAITextureBase;

//================================================================//
// MOAIShaderProgramGlobal
//================================================================//
class MOAIShaderProgramGlobal {
private:

	friend class MOAIShader;
	friend class MOAIShaderGlobals;
	friend class MOAIShaderProgram;

	u32			mGlobalID;
	ZLIndex		mUniformID;
	ZLIndex		mIndex;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramGlobal		();
};

//================================================================//
// MOAIShaderProgramTexture
//================================================================//
class MOAIShaderProgramTexture {
private:

	friend class MOAIShaderProgram;

	u32				mName;
	ZLIndex			mUnit;
	ZLStrongPtr < MOAITextureBase > mTexture;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramTexture	();
};


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
	public MOAIGfxResource,
	public MOAIShaderUniformSchema {
protected:

	friend class MOAIShader;
	friend class MOAIGfxMgr;

	enum {
		GFX_EVENT_UPDATED_UNIFORMS = GFX_EVENT_TOTAL,
	};

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle		mProgram;
	ZLGfxHandle		mVertexShader;
	ZLGfxHandle		mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniform >			mUniforms;
	ZLLeanArray < MOAIShaderProgramGlobal >		mGlobals;
	ZLLeanArray < MOAIShaderProgramTexture >	mTextures;

	size_t										mMaxCount;
	size_t										mUniformBufferSize;

	ZLLeanArray < u8 >							mUniformBuffer;

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
	void				AffirmUniforms				();
	void				ApplyUniforms				( ZLLeanArray < u8 >& buffer );
	void				BindUniforms				();
	ZLGfxHandle			CompileShader				( u32 type, cc8* source );
	MOAIShaderUniform*	GetUniform					( ZLIndex uniformID );
	void				InitUniformBuffer			( ZLLeanArray < u8 >& buffer );
	bool				OnCPUCreate					();
	void				OnCPUDestroy				();
	void				OnGPUBind					();
	bool				OnGPUCreate					();
	void				OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				OnGPUUnbind					();
	bool				OnGPUUpdate					();
	void				OnUniformLocation			( u32 addr, void* userdata );
	int					ReserveGlobals				( lua_State* L, int idx );
	void				ScheduleTextures			();
	int					SetGlobal					( lua_State* L, int idx );
	void				UpdateUniforms				( ZLLeanArray < u8 >& buffer );
	
	//----------------------------------------------------------------//
	MOAIShaderUniformHandle				MOAIShaderUniformSchema_GetUniformHandle	( void* buffer, ZLIndex uniformID ) const;
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgram )

	//----------------------------------------------------------------//
	void				Clear						();
	void				DeleteShaders				();
	void				DeclareUniform				( ZLIndex idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	void				Load						( cc8* vshSource, cc8* fshSource );
						MOAIShaderProgram			();
						~MOAIShaderProgram			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveAttributes			( ZLSize nAttributes );
	void				ReserveGlobals				( ZLSize nGlobals );
	void				ReserveTextures				( ZLSize nTextures );
	void				ReserveUniforms				( ZLSize nUniforms );
	void				SetGlobal					( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
	void				SetTexture					( ZLIndex idx, u32 name, ZLIndex unit, MOAITextureBase* fallback );
	void				SetTexture					( ZLIndex idx, MOAITextureBase* texture, ZLIndex unit );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
};

#endif
