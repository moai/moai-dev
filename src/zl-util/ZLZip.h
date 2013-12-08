// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLZIP_H
#define ZLZIP_H

class ZLStream;

#include <zl-util/ZLLeanArray.h>

//================================================================//
// ZLZip
//================================================================//
namespace ZLZip {

	static const u32 CHUNKSIZE = 16384;
	static const u32 DEFAULT_LEVEL = 9;

	//----------------------------------------------------------------//
	int		Deflate		( const void* buffer, size_t size, ZLLeanArray < u8 >& result, int level = DEFAULT_LEVEL );
	int		Deflate		( ZLStream& source, ZLStream& dest, int level = DEFAULT_LEVEL );
	cc8*	GetErrMsg	( int ret );
	int		Inflate		( const void* buffer, size_t size, ZLLeanArray < u8 >& result );
	int		Inflate		( ZLStream& source, ZLStream& dest );
}

#endif
