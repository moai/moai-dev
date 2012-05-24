// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimSample.h>

//================================================================//
// MOAIAnimSample
//================================================================//

//----------------------------------------------------------------//
void Add ( const void* s0, const void* s1 ) {
	UNUSED ( s0 );
	UNUSED ( s1 );
}

//----------------------------------------------------------------//
void GetAttrOp ( MOAIAttrOp& attrOp ) {
	UNUSED ( attrOp );
}

//----------------------------------------------------------------//
size_t GetSize ( u32 type ) {
	UNUSED ( type );
	return 0;
}

//----------------------------------------------------------------//
void Interpolate ( const void* s0, const void* s1, u32 mode, float t, float w ) {
	UNUSED ( s0 );
	UNUSED ( s1 );
	UNUSED ( mode );
	UNUSED ( t );
	UNUSED ( w );
}

//----------------------------------------------------------------//
void Set ( void* buffer, u32 type ) {
	UNUSED ( buffer );
	UNUSED ( type );
}

//----------------------------------------------------------------//
void Sub ( const void* s0, const void* s1 ) {
	UNUSED ( s0 );
	UNUSED ( s1 );
}
