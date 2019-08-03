// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMBINDINGGL_H
#define	MOAISHADERUNIFORMBINDINGGL_H

//================================================================//
// MOAIShaderUniformBindingGL
//================================================================//
class MOAIShaderUniformBindingGL {
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
	void			Bind							( ZLGfx&gfx, const MOAIShaderUniformDescriptor& descriptor, const void* buffer ) const;
	size_t			GetSize							( const MOAIShaderUniformDescriptor& descriptor ) const;
	void			Init							( u32 count = 1 );
					MOAIShaderUniformBindingGL		();
					~MOAIShaderUniformBindingGL		();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
