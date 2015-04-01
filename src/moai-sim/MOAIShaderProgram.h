// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERPROGRAM_H
#define	MOAISHADERPROGRAM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAIShaderUniform.h>

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n"
#define		WEBGL_PREPROC		"precision mediump int;\n precision mediump float;\n"

//================================================================//
// MOAIShaderProgramGlobal
//================================================================//
class MOAIShaderProgramGlobal {
private:

	friend class MOAIShaderProgram;

	u32		mUniformID;
	u32		mGlobalID;
};

//================================================================//
// MOAIShaderProgram
//================================================================//
/**	@lua	MOAIShaderProgram
	@text	Programmable shader class.

	@const	UNIFORM_COLOR
	@const	UNIFORM_FLOAT
	@const	UNIFORM_INT
	@const  UNIFORM_NORMAL
	@const	UNIFORM_PEN_COLOR
	@const	UNIFORM_SAMPLER
	@const	UNIFORM_TRANSFORM
	@const	UNIFORM_VIEW_PROJ
	@const	UNIFORM_VIEW_WIDTH
	@const	UNIFORM_VIEW_HEIGHT
	@const	UNIFORM_VIEW_HALF_WIDTH
	@const	UNIFORM_VIEW_HALF_HEIGHT
	@const	UNIFORM_WORLD
	@const	UNIFORM_WORLD_VIEW
	@const	UNIFORM_WORLD_VIEW_PROJ
*/
class MOAIShaderProgram :
	public MOAIGfxResource {
protected:

	friend class MOAIShader;
	// friend class MOAIShaderProgram;

	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;

	u32				mProgram;
	u32				mVertexShader;
	u32				mFragmentShader;

	typedef STLMap < u32, STLString >::iterator AttributeMapIt;
	STLMap < u32, STLString > mAttributeMap;

	ZLLeanArray < MOAIShaderUniform >			mUniforms;
	ZLLeanArray < MOAIShaderUniformBuffer >		mUniformDefaults;
	ZLLeanArray < MOAIShaderProgramGlobal >		mGlobals;

	//----------------------------------------------------------------//
	static int		_clearUniform				( lua_State* L );
	static int		_declareUniform				( lua_State* L );
	static int		_declareUniformFloat		( lua_State* L );
	static int		_declareUniformInt			( lua_State* L );
	static int		_declareUniformSampler		( lua_State* L );
	static int		_load						( lua_State* L );
	static int		_reserveGlobals				( lua_State* L );
	static int		_reserveUniforms			( lua_State* L );
	static int		_setGlobal					( lua_State* L );
	static int		_setVertexAttribute			( lua_State* L );

	//----------------------------------------------------------------//
	u32				CompileShader				( u32 type, cc8* source );
	u32				GetLoadingPolicy			();
	void			PrintProgramLog				( u32 program );
	void			PrintShaderLog				( u32 shader );
	bool			OnCPUCreate					();
	void			OnCPUDestroy				();
	void			OnGPUBind					();
	bool			OnGPUCreate					();
	void			OnGPUDestroy				();
	void			OnGPULost					();
	void			OnGPUUnbind					();
	bool			Validate					();

public:

	DECL_LUA_FACTORY ( MOAIShaderProgram )

	enum {
		GLOBAL_NONE,
		GLOBAL_PEN_COLOR,
		GLOBAL_VIEW_PROJ,
		GLOBAL_VIEW_WIDTH,
		GLOBAL_VIEW_HEIGHT,
		GLOBAL_VIEW_HALF_WIDTH,
		GLOBAL_VIEW_HALF_HEIGHT,
		GLOBAL_WORLD,
		GLOBAL_WORLD_VIEW,
		GLOBAL_WORLD_VIEW_PROJ,
		GLOBAL_WORLD_VIEW_PROJ_NORM,
	};

	//----------------------------------------------------------------//
	void			Clear						();
	void			ClearUniform				( u32 idx );
	void			ClearUniforms				();
	void			DeleteShaders				();
	void			DeclareUniform				( u32 idx, cc8* name, u32 type );
	void			DeclareUniform				( u32 idx, cc8* name, u32 type, float value );
	void			DeclareUniform				( u32 idx, cc8* name, u32 type, int value );
	bool			IsValid						();
	bool			LoadGfxState				();
					MOAIShaderProgram			();
					~MOAIShaderProgram			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			ReserveAttributes			( u32 nAttributes );
	void			ReserveGlobals				( u32 nGlobals );
	void			ReserveUniforms				( u32 nUniforms );
	void			SetGlobal					( u32 idx, u32 uniformID, u32 globalID );
	void			SetSource					( cc8* vshSource, cc8* fshSource );
	void			SetVertexAttribute			( u32 idx, cc8* attribute );
	void			UpdateGlobals				();
};

#endif
