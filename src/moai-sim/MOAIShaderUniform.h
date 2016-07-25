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

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	u32		mType;			// type of element
	u32		mWidth;			// number of elements
	
	ZLLeanArray < u8 > mBuffer;

	union {
		float	mFloat;
		s32		mInt;
	};

	//----------------------------------------------------------------//
	void		Clear				();
	void		ClearValue			();
	u32			SetBuffer			( const s32* buffer, u32 width );
	u32			SetBuffer			( const float* buffer, u32 width );

public:

	enum {
		UNIFORM_TYPE_FLOAT,
		UNIFORM_TYPE_INT,
	};

	enum {
		UNIFORM_WIDTH_VEC_2			= 2,
		UNIFORM_WIDTH_VEC_3			= 3,
		UNIFORM_WIDTH_VEC_4			= 4,
		UNIFORM_WIDTH_MATRIX_3X3	= 9,
		UNIFORM_WIDTH_MATRIX_4X4	= 16,
	};

	GET ( u32, Type, mType )
	GET ( u32, Width, mWidth )

	//----------------------------------------------------------------//
	void		AddValue					( const MOAIAttribute& attr );
	void		Default						();
	bool		Init						( u32 type, u32 width = 1 );
				MOAIShaderUniformBuffer		();
				~MOAIShaderUniformBuffer	();
	u32			SetValue					( float value );
	u32			SetValue					( s32 value );
	u32			SetValue					( const MOAIAttribute& attr );
	u32			SetValue					( const MOAIShaderUniformBuffer& uniformBuffer );
	u32			SetValue					( const ZLColorVec& value );
	u32			SetValue					( const ZLIntVec4D& value );
	u32			SetValue					( const ZLAffine2D& value );
	u32			SetValue					( const ZLAffine3D& value );
	u32			SetValue					( const ZLMatrix3x3& value );
	u32			SetValue					( const ZLMatrix4x4& value );
	u32			SetValue					( const ZLVec4D& value );
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
