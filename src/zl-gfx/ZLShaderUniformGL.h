// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADERUNIFORMGL_H
#define	ZLSHADERUNIFORMGL_H

#include <zl-gfx/ZLGfxEnum.h>

class ZLGfx;

//================================================================//
// ZLShaderUniformBaseGL
//================================================================//
class ZLShaderUniformBaseGL {
protected:

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix
	static const size_t ELEMENT_SIZE = 4;

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

	u32				mType;				// type of buffer
	u32				mWidth;				// number of elements (ELEMENT_SIZE bytes each)

	//----------------------------------------------------------------//
	size_t			GetSize						() const;
	bool			Init						( u32 type, u32 width = 1 );
					ZLShaderUniformBaseGL		();
					~ZLShaderUniformBaseGL		();
};

//================================================================//
// ZLShaderUniformGL
//================================================================//
class ZLShaderUniformGL :
	public ZLShaderUniformBaseGL {
protected:

	friend class MOAIShaderGL;
	friend class MOAIShaderProgramGL;
	friend class ZLShaderGL;
	friend class ZLShaderProgramGL;

	STLString		mName;
	u32				mCount;				// (default) size of array
	u32				mGPUBase;			// this is resolved when linking the shader
	size_t			mCPUOffset;			// offset in CPU buffer

public:

	//----------------------------------------------------------------//
	void			Bind						( ZLGfx&gfx, const void* buffer ) const;
	size_t			GetSize						() const;
	void			Init						( u32 type, u32 width = 1, u32 count = 1 );
					ZLShaderUniformGL			();
					~ZLShaderUniformGL			();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
