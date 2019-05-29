// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERUNIFORMHANDLE_H
#define	MOAISHADERUNIFORMHANDLE_H

#include <moai-gfx/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniformHandle
//================================================================//
class MOAIShaderUniformHandle :
	public MOAIShaderUniform {
private:

	//----------------------------------------------------------------//
	void						SetRawValue						( const void* srcBuffer );
	void						SetValue						( const s32* srcBuffer );
	void						SetValue						( const float* srcBuffer );

public:

	void*						mBuffer;

	//----------------------------------------------------------------//
	void						AddValue						( const ZLAttribute& attr );
	void						Default							( u32 count = 1 );
	bool						IsValid							();
	void						Next							();
	MOAIShaderUniformHandle		Offset							( ZLIndex index );
	void						SetValue						( float value );
	void						SetValue						( s32 value );
	void						SetValue						( const ZLAttribute& attr );
	void						SetValue						( const ZLColorVec& value );
	void						SetValue						( const ZLIntVec4D& value );
	void						SetValue						( const ZLAffine2D& value );
	void						SetValue						( const ZLAffine3D& value );
	void						SetValue						( const ZLMatrix3x3& value );
	void						SetValue						( const ZLMatrix4x4& value );
	void						SetValue						( const ZLVec4D& value );
								MOAIShaderUniformHandle			();
								~MOAIShaderUniformHandle		();
};

#endif
