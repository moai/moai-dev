// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USZIP_H
#define USZIP_H

//================================================================//
// USZip
//================================================================//
/**	@brief USZip utility functions (wrapper for zlib).
*/
namespace USZip {

	static const u32 CHUNKSIZE = 16384;

	//----------------------------------------------------------------//
	int		Deflate		( USStream& source, USStream& dest, int level );
	cc8*	GetErrMsg	( int ret );
	int		Inflate		( USStream& source, USStream& dest );
}

#endif
