// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMSAMPLE_H
#define	MOAIANIMSAMPLE_H

#include <moaicore/MOAIAttrOp.h>

//================================================================//
// MOAIAnimSample
//================================================================//
class MOAIAnimSample {
private:

	static const size_t BUFFER_SIZE = 64;

	u32 mType;
	u8 mBuffer [ BUFFER_SIZE ];

public:

	enum {
		TYPE_BOOL,
		TYPE_FLOAT,
		TYPE_INDEX,
		TYPE_INT,
		TYPE_QUATERNION,
		TYPE_TRANSFORM_2D,
		TYPE_TRANSFORM_3D,
		TYPE_VEC2,
		TYPE_VEC3,
	};
	
	//----------------------------------------------------------------//
	void			Add				( const void* s0, const void* s1 );
	void			GetAttrOp		( MOAIAttrOp& attrOp );
	static size_t	GetSize			( u32 type );
	void			Interpolate		( const void* s0, const void* s1, u32 mode, float t, float w );
	void			Set				( void* buffer, u32 type );
	void			Sub				( const void* s0, const void* s1 );
};

#endif
