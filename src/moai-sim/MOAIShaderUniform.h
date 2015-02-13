// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>

class MOAIColor;
class MOAITransformBase;

#define		OPENGL_PREPROC		"#define LOWP\n #define MEDP\n"
#define		OPENGL_ES_PREPROC	"#define LOWP lowp\n #define MEDP mediump\n"

//================================================================//
// MOAIShaderUniformBuffer
//================================================================//
class MOAIShaderUniformBuffer {
protected:

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	u32		mType;
	
	ZLLeanArray < u8 > mBuffer;

	union {
		float	mFloat;
		int		mInt;
	};

	//----------------------------------------------------------------//
	void		Clear				();
	void		ClearValue			();
	bool		SetBuffer			( void* buffer, size_t size, bool check );

public:

	enum {
		UNIFORM_NONE,
		UNIFORM_FLOAT,
		UNIFORM_INDEX,
		UNIFORM_INT,
		UNIFORM_MATRIX_F3,
		UNIFORM_MATRIX_F4,
		UNIFORM_VECTOR_F4,
	};

	GET ( u32, Type, mType )

	//----------------------------------------------------------------//
	void		AddValue			( const MOAIAttrOp& attrOp );
	void		Default				();
	void		GetFlags			( MOAIAttrOp& attrOp );
	void        GetValue			( MOAIAttrOp& attrOp );
	void		SetType				( u32 type );
	bool		SetValue			( float value );
	bool		SetValue			( int value );
	bool		SetValue			( const MOAIAttrOp& attrOp, bool check );
	bool		SetValue			( const ZLColorVec& value, bool check );
	bool		SetValue			( const ZLAffine3D& value, bool check );
	bool		SetValue			( const ZLMatrix4x4& value, bool check );
	bool    	SetValue          	( const ZLMatrix3x3& value, bool check );
	bool		SetValue			( const MOAIShaderUniformBuffer& uniformBuffer, bool check );
};

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform :
	public MOAIShaderUniformBuffer {
private:

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	STLString	mName;
	u32			mAddr;			// this is resolved when linking the shader
	u32			mGlobal;		// this may be used to select a global rendering value such as the pen color or world transform

public:

	//----------------------------------------------------------------//
	void		Bind				();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return this->mAddr != ZGL_INVALID_UNIFORM_ADDR;
	}
};

#endif
