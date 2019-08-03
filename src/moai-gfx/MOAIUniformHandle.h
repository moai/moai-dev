// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNIFORMHANDLE_H
#define	MOAIUNIFORMHANDLE_H

#include <moai-gfx/MOAIUniformDescriptor.h>

//================================================================//
// MOAIUniformHandle
//================================================================//
class MOAIUniformHandle :
	public MOAIUniformDescriptor {
private:

	//----------------------------------------------------------------//
	void						SetRawValue						( const void* srcBuffer );
	void						SetValue						( const s32* srcBuffer );
	void						SetValue						( const float* srcBuffer );

public:

	ZLIndex						mIndex;
	void*						mBuffer;

	//----------------------------------------------------------------//
	MOAIUniformHandle& operator = ( const MOAIUniformDescriptor& assign ) {
		*( MOAIUniformDescriptor* )this = assign;
		return *this;
	}

	//----------------------------------------------------------------//
	void						AddValue						( const ZLAttribute& attr );
	void						Default							( u32 count = 1 );
	bool						IsValid							();
	void						Next							();
	MOAIUniformHandle			Offset							( ZLIndex index );
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
								MOAIUniformHandle				();
								~MOAIUniformHandle				();
};

#endif
