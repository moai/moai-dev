// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

//================================================================//
// MOAIShaderProgramGlobal
//================================================================//
class MOAIShaderProgramGlobal {
private:

	friend class MOAIShader;
	friend class MOAIShaderGlobals;
	friend class MOAIShaderProgram;

	u32		mGlobalID;
	u32		mUniformID;
	u32		mIndex;

public:

	//----------------------------------------------------------------//
		MOAIShaderProgramGlobal		();
};

//================================================================//
// MOAIShaderGlobals
//================================================================//
class MOAIShaderGlobals {
protected:

	ZLLeanArray < MOAIShaderProgramGlobal >		mGlobals;

	//----------------------------------------------------------------//
	int		ReserveGlobals			( lua_State* L, int idx );
	int		SetGlobal				( lua_State* L, int idx );

public:

	//----------------------------------------------------------------//
	void	CopyGlobals				( const MOAIShaderGlobals& globals );
	void	ReserveGlobals			( u32 nGlobals );
	void	SetGlobal				( u32 idx, u32 globalID, u32 uniformID, u32 index );
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

	@const	UNIFORM_FLOAT
	@const	UNIFORM_INDEX
	@const	UNIFORM_INT
	@const	UNIFORM_MATRIX_F3
	@const	UNIFORM_MATRIX_F4
	@const	UNIFORM_VECTOR_F4
	
	@const	GLOBAL_VIEW_PROJ
	@const	GLOBAL_VIEW_PROJ
	@const	GLOBAL_VIEW_WIDTH
	@const	GLOBAL_VIEW_HEIGHT
	@const	GLOBAL_WORLD
	@const	GLOBAL_WORLD_INVERSE
	@const	GLOBAL_WORLD_VIEW
	@const	GLOBAL_WORLD_VIEW_INVERSE
	@const	GLOBAL_WORLD_VIEW_PROJ
*/
class MOAIShaderProgram :
	public MOAIGfxResource,
	public MOAIShaderGlobals {
protected:

	friend class MOAIShader;
	friend class MOAIGfxMgr;

	enum {
		GFX_EVENT_UPDATED_UNIFORMS = GFX_EVENT_TOTAL,
	};

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	ZLGfxHandle*	mProgram;
	ZLGfxHandle*	mVertexShader;
	ZLGfxHandle*	mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniform >			mUniforms;
	
	u32											mGlobalsMask;

	//----------------------------------------------------------------//
	static int			_declareUniform				( lua_State* L );
	static int			_load						( lua_State* L );
	static int			_reserveGlobals				( lua_State* L );
	static int			_reserveUniforms			( lua_State* L );
	static int			_setVertexAttribute			( lua_State* L );
	static int			_setGlobal					( lua_State* L );

	//----------------------------------------------------------------//
	ZLGfxHandle*		CompileShader				( u32 type, cc8* source );
	MOAIShaderUniform*	GetUniform					( u32 uniformID );
	bool				OnCPUCreate					();
	void				OnCPUDestroy				();
	void				OnGPUBind					();
	bool				OnGPUCreate					();
	void				OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				OnGPUUnbind					();
	bool				OnGPUUpdate					();
	void				OnUniformLocation			( u32 addr, void* userdata );
	
public:

	DECL_LUA_FACTORY ( MOAIShaderProgram )

	//----------------------------------------------------------------//
	void				Clear						();
	//void				ClearUniforms				();
	void				DeleteShaders				();
	void				DeclareUniform				( u32 idx, cc8* name, u32 type, u32 width = 1, u32 count = 1 );
	u32					GetGlobalsMask				();
	void				Load						( cc8* vshSource, cc8* fshSource );
						MOAIShaderProgram			();
						~MOAIShaderProgram			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveAttributes			( u32 nAttributes );
	void				ReserveUniforms				( u32 nUniforms );
	void				SetVertexAttribute			( u32 idx, cc8* attribute );
};

#endif
