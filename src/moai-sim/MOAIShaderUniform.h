// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>

class MOAIColor;
class MOAITransformBase;

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
	u32			SetBuffer			( void* buffer, size_t size );

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
	u32			SetValue			( float value );
	u32			SetValue			( int value );
	u32			SetValue			( const MOAIAttrOp& attrOp );
	u32			SetValue			( const ZLColorVec& value );
	u32			SetValue			( const ZLAffine3D& value );
	u32			SetValue			( const ZLMatrix4x4& value );
	u32			SetValue          	( const ZLMatrix3x3& value );
	u32			SetValue			( const MOAIShaderUniformBuffer& uniformBuffer );
};

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform :
	public MOAIShaderUniformBuffer {
private:

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	STLString		mName;
	u32				mAddr;			// this is resolved when linking the shader
	u32				mFlags;			// used by MOAIShaderProgram

public:

	enum {
		UNIFORM_FLAG_DIRTY		= 0x01,
		UNIFORM_FLAG_GLOBAL		= 0x02,
	};

	//----------------------------------------------------------------//
	void		Bind				();
				MOAIShaderUniform	();
				~MOAIShaderUniform	();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return this->mAddr != ZGL_INVALID_UNIFORM_ADDR;
	}
};

#endif
