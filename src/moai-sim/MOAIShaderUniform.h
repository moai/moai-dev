// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORM_H
#define	MOAISHADERUNIFORM_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAIShaderUniformFormatter.h>

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
class MOAIShaderUniform :
	public MOAIShaderUniformFormatter {
protected:

	friend class MOAIShader;
	friend class MOAIShaderProgram;

	STLString		mName;
	u32				mCount;				// (default) size of array
	u32				mGPUBase;			// this is resolved when linking the shader

public:

	//----------------------------------------------------------------//
	void			Bind					( MOAIShaderUniformInstance& instance );
	void			Default					( void* buffer, u32 count ) const;
	size_t			GetSize					( u32 count ) const;
	void			Init					( u32 type, u32 width = 1, u32 count = 1 );
					MOAIShaderUniform		();
					~MOAIShaderUniform		();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
