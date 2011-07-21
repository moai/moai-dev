// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERES2_H
#define	MOAISHADERES2_H

#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIShader.h>

class MOAITransformBase;

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform {
private:
	friend class MOAIGlslProgram;

	enum {
		UNIFORM_NONE,
		UNIFORM_INT,
		UNIFORM_FLOAT,
		UNIFORM_MODEL,
		UNIFORM_TRANSFORM,
		UNIFORM_VIEW_PROJ,
	};

	u32		mAddr;
	u32		mType;
	u32		mSize;
	u32		mSrc;
	
	STLString mName;
	USRef < MOAITransformBase >	mTransform;

	//----------------------------------------------------------------//
	void		Bind				( const float* attributes );
	void		BindAffine			( const USAffine2D& affine );

public:

	//----------------------------------------------------------------//
				MOAIShaderUniform	();

};

//================================================================//
// MOAIGlslProgram
//================================================================//
// TODO: doxygen
class MOAIGlslProgram :
	public virtual MOAIShader {
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
	void			Affirm					();
	GLuint			CompileShader			( GLuint type,  cc8* source );
	void			SetSource				( cc8* vshSource, cc8* fshSource );
	void			Validate				();

public:
	
	DECL_LUA_FACTORY ( MOAIGlslProgram )
	
	//----------------------------------------------------------------//
	void			Bind					();
	void			Clear					();
	void			ClearUniform			( u32 idx );
					MOAIGlslProgram			();
					~MOAIGlslProgram		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
