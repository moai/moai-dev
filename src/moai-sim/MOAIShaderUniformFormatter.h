// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMFORMATTER_H
#define	MOAISHADERUNIFORMFORMATTER_H

class MOAIAttribute;

//================================================================//
// MOAIShaderUniformFormatter
//================================================================//
class MOAIShaderUniformFormatter {
private:

	static const size_t MAX_BUFFER_WIDTH = 16; // 4x4 matrix
	static const size_t ELEMENT_SIZE = 4;

	//----------------------------------------------------------------//
	void			SetRawValue			( void* buffer, const void* srcBuffer ) const;
	void			SetValue			( void* buffer, const s32* srcBuffer ) const;
	void			SetValue			( void* buffer, const float* srcBuffer ) const;

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
	void			AddValue						( void* buffer, const MOAIAttribute& attr ) const;
	void			Default							( void* buffer, u32 count = 1 ) const;
	void*			GetArrayItem					( void* buffer, u32 index ) const;
	const void*		GetArrayItem					( const void* buffer, u32 index ) const;
	size_t			GetSize							() const;
	bool			Init							( u32 type, u32 width = 1 );
					MOAIShaderUniformFormatter		();
					~MOAIShaderUniformFormatter		();
	void			SetValue						( void* buffer, float value ) const;
	void			SetValue						( void* buffer, s32 value ) const;
	void			SetValue						( void* buffer, const MOAIAttribute& attr ) const;
	void			SetValue						( void* buffer, const ZLColorVec& value ) const;
	void			SetValue						( void* buffer, const ZLIntVec4D& value ) const;
	void			SetValue						( void* buffer, const ZLAffine2D& value ) const;
	void			SetValue						( void* buffer, const ZLAffine3D& value ) const;
	void			SetValue						( void* buffer, const ZLMatrix3x3& value ) const;
	void			SetValue						( void* buffer, const ZLMatrix4x4& value ) const;
	void			SetValue						( void* buffer, const ZLVec4D& value ) const;
};

#endif
