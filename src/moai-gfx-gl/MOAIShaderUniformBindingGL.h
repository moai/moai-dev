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
	u32				mGPUBase;			// this is resolved when linking the shader
	
public:

	//----------------------------------------------------------------//
	void			Bind							( ZLGfx&gfx, const MOAIUniformDescriptor& descriptor, const void* buffer ) const;
	size_t			GetSize							( const MOAIUniformDescriptor& descriptor ) const;
					MOAIShaderUniformBindingGL		();
					~MOAIShaderUniformBindingGL		();
	
	//----------------------------------------------------------------//
	inline bool IsValid () {
		return ( this->mGPUBase != ZGL_INVALID_UNIFORM_ADDR );
	}
};

#endif
