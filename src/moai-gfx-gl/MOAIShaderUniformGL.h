// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMGL_H
#define	MOAISHADERUNIFORMGL_H

//================================================================//
// MOAIShaderUniformGL
//================================================================//
class MOAIShaderUniformGL :
	public MOAIShaderUniform {
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
					MOAIShaderUniformGL			();
					~MOAIShaderUniformGL		();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
