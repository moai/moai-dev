// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>

class MOAIColor;
class MOAITransformBase;

//================================================================//
// MOAIShaderUniform
//================================================================//
class MOAIShaderUniform {
protected:

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix
	static const size_t ELEMENT_SIZE = 4;

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	STLString		mName;
	u32				mType;				// type of element
	u32				mWidth;				// number of elements (ELEMENT_SIZE bytes each)
	u32				mCount;				// size of array
	size_t			mSize;				// size in bytes
	size_t			mCPUBase;			// offset in CPU buffer
	u32				mGPUBase;			// this is resolved when linking the shader
	u32				mFlagsBase;			// index of the first flag

	//----------------------------------------------------------------//
	void*			GetUniformBuffer	( void* buffer, u32 index );
	const void*		GetUniformBuffer	( const void* buffer, u32 index );
	u32				SetRawValue			( void* buffer, u32 index, const void* srcBuffer );
	u32				SetValue			( void* buffer, u32 index, const s32* srcBuffer );
	u32				SetValue			( void* buffer, u32 index, const float* srcBuffer );

public:

	enum {
		UNIFORM_FLAG_DIRTY		= 0x01,
		UNIFORM_FLAG_GLOBAL		= 0x02,
	};

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
	void		AddValue				( void* buffer, u32 index, const MOAIAttribute& attr );
	void		Bind					( const void* buffer, u32 index );
	u32			CopyValue				( void* buffer, const void* srcBuffer, u32 index );
	void		Default					( void* buffer );
	bool		Init					( u32 type, u32 width = 1, u32 count = 1 );
				MOAIShaderUniform		();
				~MOAIShaderUniform		();
	u32			SetValue				( void* buffer, u32 index, float value );
	u32			SetValue				( void* buffer, u32 index, s32 value );
	u32			SetValue				( void* buffer, u32 index, const MOAIAttribute& attr );
	u32			SetValue				( void* buffer, u32 index, const ZLColorVec& value );
	u32			SetValue				( void* buffer, u32 index, const ZLIntVec4D& value );
	u32			SetValue				( void* buffer, u32 index, const ZLAffine2D& value );
	u32			SetValue				( void* buffer, u32 index, const ZLAffine3D& value );
	u32			SetValue				( void* buffer, u32 index, const ZLMatrix3x3& value );
	u32			SetValue				( void* buffer, u32 index, const ZLMatrix4x4& value );
	u32			SetValue				( void* buffer, u32 index, const ZLVec4D& value );

	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
