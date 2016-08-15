// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>

class MOAIColor;
class MOAITransformBase;

//================================================================//
// MOAIShaderUniformInstance
//================================================================//
class MOAIShaderUniformInstance {
protected:

	friend class MOAIShader;
	friend class MOAIShaderUniform;

	void*			mBuffer;			// offset in CPU buffer
	u32				mCount;				// actual size of array (may differ from default count in uniform)
};

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
	u32				mCount;				// (default) size of array
	u32				mGPUBase;			// this is resolved when linking the shader

	//----------------------------------------------------------------//
	void*			GetUniformBuffer	( MOAIShaderUniformInstance& instance, u32 index ) const;
	const void*		GetUniformBuffer	( const MOAIShaderUniformInstance& instance, u32 index ) const;
	void			SetRawValue			( MOAIShaderUniformInstance& instance, u32 index, const void* srcBuffer ) const;
	void			SetValue			( MOAIShaderUniformInstance& instance, u32 index, const s32* srcBuffer ) const;
	void			SetValue			( MOAIShaderUniformInstance& instance, u32 index, const float* srcBuffer ) const;

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
	void		AddValue				( MOAIShaderUniformInstance& instance, u32 index, const MOAIAttribute& attr );
	void		Bind					( const MOAIShaderUniformInstance& instance, u32 index );
	void		Default					( MOAIShaderUniformInstance& instance ) const;
	bool		Init					( u32 type, u32 width = 1, u32 count = 1 );
				MOAIShaderUniform		();
				~MOAIShaderUniform		();
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, float value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, s32 value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const MOAIAttribute& attr ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLColorVec& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLIntVec4D& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLAffine2D& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLAffine3D& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLMatrix3x3& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLMatrix4x4& value ) const;
	void		SetValue				( MOAIShaderUniformInstance& instance, u32 index, const ZLVec4D& value ) const;

	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
