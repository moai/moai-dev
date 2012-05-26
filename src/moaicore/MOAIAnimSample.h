// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMSAMPLE_H
#define	MOAIANIMSAMPLE_H

#include <moaicore/MOAIAttrOp.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIAnimSample
//================================================================//
class MOAIAnimSample {
private:

	static const size_t BUFFER_SIZE = 64;

	u32			mType;
	size_t		mSize;
	u8			mBuffer [ BUFFER_SIZE ];

public:

	enum {
		TYPE_UNKNOWN,
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
	void			Add				( const MOAIAnimSample& s0, const MOAIAnimSample& s1 );
	void			Clear			();
	bool			Equals			( const MOAIAnimSample& s0 );
	void			GetAttrOp		( MOAIAttrOp& attrOp );
	void			GetBuffer		( void* buffer ) const;
	static size_t	GetSize			( u32 type );
	void			HandleAttrOp	( MOAIAttrOp& attrOp, u32 op );
	void			Interpolate		( const MOAIAnimSample& s0, const MOAIAnimSample& s1, u32 mode, float t, float w );
	u32				Push			( MOAILuaState& state );
	void			Scale			( float s );
	void			Set				( u32 type = TYPE_UNKNOWN );
	void			Set				( float value );
	void			Set				( const void* buffer, u32 type );
	void			Set				( MOAILuaState& state, int idx, u32 type );
	void			Sub				( const MOAIAnimSample& s0, const MOAIAnimSample& s1 );
};

#endif
