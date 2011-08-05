// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moaicore/MOAINode.h>

class MOAITransformBase;
	
#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n"

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform {
private:

	friend class MOAIShader;

	enum {
		UNIFORM_NONE,
		UNIFORM_INT,
		UNIFORM_FLOAT,
		UNIFORM_TRANSFORM,
		UNIFORM_VIEW_PROJ,
		UNIFORM_WORLD,
		UNIFORM_WORLD_VIEW_PROJ,
	};

	u32		mAddr;
	u32		mType;
	u32		mSize;
	u32		mSrc;
	
	STLString mName;
	USRef < MOAITransformBase >	mTransform;

	//----------------------------------------------------------------//
	void		BindAttributes				( const float* attributes );
	void		BindMatrix					( const USMatrix3D& matrix );
	void		BindPipelineTransforms		( const USMatrix3D& world, const USMatrix3D& view, const USMatrix3D& proj );

public:

	//----------------------------------------------------------------//
				MOAIShaderUniform	();

};

//================================================================//
// MOAIShader
//================================================================//
// TODO: doxygen
class MOAIShader :
	public virtual MOAINode {
protected:
	
	enum {
		STATE_UNINITIALIZED,
		STATE_PENDING_LOAD,
		STATE_ERROR,
		STATE_VALID,
	};
	
	u32				mState;
	
	STLString		mVertexShaderSource;
	STLString		mFragmentShaderSource;
	
	GLuint			mProgram;
	GLuint			mVertexShader;
	GLuint			mFragmentShader;
	
	typedef STLMap < GLuint, STLString >::iterator AttributeMapIt;
	STLMap < GLuint, STLString > mAttributeMap;
	
	USLeanArray < MOAIShaderUniform > mUniforms;
	USLeanArray < float > mAttributes;
	
	//----------------------------------------------------------------//
	static int		_clearUniform			( lua_State* L );
	static int		_declareUniform			( lua_State* L );
	static int		_load					( lua_State* L );
	static int		_reserveAttributes		( lua_State* L );
	static int		_reserveUniforms		( lua_State* L );
	static int		_setUniform				( lua_State* L );
	static int		_setVertexAttribute		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Affirm						();
	void			Bind						();
	GLuint			CompileShader				( GLuint type,  cc8* source );
	void			UpdatePipelineTransforms	( const USMatrix3D& world, const USMatrix3D& view, const USMatrix3D& proj );
	void			Validate					();

public:
	
	DECL_LUA_FACTORY ( MOAIShader )
	
	friend class MOAIGfxDevice;
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			ClearUniform			( u32 idx );
	void			DeclareUniform			( u32 idx, cc8* name, u32 type );
					MOAIShader				();
					~MOAIShader				();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReserveAttributes		( u32 nAttributes );
	void			ReserveUniforms			( u32 nUniforms );
	void			SetSource				( cc8* vshSource, cc8* fshSource );
	void			SetUniform				( u32 idx, u32 src, u32 size );
	void			SetUniform				( u32 idx, MOAITransformBase* transform );
	void			SetVertexAttribute		( u32 idx, cc8* attribute );
	STLString		ToString				();
};

#endif
