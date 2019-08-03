// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNIFORMDESCRIPTOR_H
#define	MOAIUNIFORMDESCRIPTOR_H

//================================================================//
// MOAIUniformDescriptor
//================================================================//
class MOAIUniformDescriptor {
protected:

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix
	static const size_t COMPONENT_SIZE = 4;

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
	u32				mCount;				// (default) size of array
	uintptr			mCPUOffset;			// offset in CPU buffer

	//----------------------------------------------------------------//
	ZLSize			GetBufferSize					() const;
	ZLSize			GetComponentSize				() const;
	ZLSize			GetUniformSize					() const;
	bool			Init							( u32 type, u32 width = 1, u32 count = 1 );
					MOAIUniformDescriptor			();
					~MOAIUniformDescriptor			();
};

#endif
