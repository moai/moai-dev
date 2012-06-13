// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USZIP_H
#define USZIP_H

class USStream;

#include <uslscore/USLeanArray.h>

//================================================================//
// USZip
//================================================================//
namespace USZip {

	static const u32 CHUNKSIZE = 16384;
	static const u32 DEFAULT_LEVEL = 9;

	//----------------------------------------------------------------//
	int		Deflate		( const void* buffer, size_t size, USLeanArray < u8 >& result, int level = DEFAULT_LEVEL );
	int		Deflate		( USStream& source, USStream& dest, int level = DEFAULT_LEVEL );
	cc8*	GetErrMsg	( int ret );
	int		Inflate		( const void* buffer, size_t size, USLeanArray < u8 >& result );
	int		Inflate		( USStream& source, USStream& dest );
}

#endif
