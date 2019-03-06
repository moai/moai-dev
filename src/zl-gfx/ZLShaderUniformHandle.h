// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSHADERUNIFORMHANDLE_H
#define	ZLSHADERUNIFORMHANDLE_H

#include <zl-gfx/ZLShaderUniform.h>

class ZLAttribute;

//================================================================//
// ZLShaderUniformHandle
//================================================================//
class ZLShaderUniformHandle :
	public ZLShaderUniformBase {
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
	ZLShaderUniformHandle		Offset							( ZLIndex index );
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
								ZLShaderUniformHandle			();
								~ZLShaderUniformHandle			();
};

#endif
