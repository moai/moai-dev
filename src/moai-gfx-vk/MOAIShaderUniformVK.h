// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMVK_H
#define	MOAISHADERUNIFORMVK_H

//================================================================//
// MOAIShaderUniformVK
//================================================================//
class MOAIShaderUniformVK :
	public MOAIShaderUniform {
protected:

//	friend class MOAIShaderVK;
//	friend class MOAIShaderProgramVK;
//	friend class ZLShaderVK;
//	friend class ZLShaderProgramVK;
//
//	STLString		mName;
//	u32				mCount;				// (default) size of array
//	u32				mGPUBase;			// this is resolved when linking the shader
//	size_t			mCPUOffset;			// offset in CPU buffer

public:

	//----------------------------------------------------------------//
//	void			Bind						( ZLGfx&gfx, const void* buffer ) const;
//	size_t			GetSize						() const;
//	void			Init						( u32 type, u32 width = 1, u32 count = 1 );
					MOAIShaderUniformVK			();
					~MOAIShaderUniformVK		();
	
//	//----------------------------------------------------------------//
//	inline bool IsValid () {
//		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
//	}
};

#endif
