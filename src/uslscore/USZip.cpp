// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// based on zpipe.c Version 1.2 as provided to the public domain 9 November 2004 by Mark Adler

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

#include <uslscore/USByteStream.h>
#include <uslscore/USMemStream.h>
#include <uslscore/USStream.h>
#include <uslscore/USZip.h>

//----------------------------------------------------------------//
int USZip::Deflate ( const void* buffer, size_t size, USLeanArray < u8 >& result, int level ) {

	USMemStream outStream;
	
	USByteStream inStream;
	inStream.SetBuffer (( void* )buffer, size );
	inStream.SetLength ( size );
	
	int r = USZip::Deflate ( inStream, outStream, level );
	
	if ( r == Z_OK ) {
		result.Init ( outStream.GetLength ());
		outStream.Seek ( 0, SEEK_SET );
		outStream.ReadBytes ( result.Data (), result.Size () );
	}
	return r;
}

//----------------------------------------------------------------//
int USZip::Deflate ( USStream& source, USStream& dest, int level ) {

    int ret, flush;
    unsigned have;
    z_stream strm;
    char in [ CHUNKSIZE ];
    char out [ CHUNKSIZE ];

    /* allocate deflate state */
    //strm.zalloc = Z_NULL;
    //strm.zfree = Z_NULL;
    //strm.opaque = Z_NULL;
    
    memset ( &strm, 0, sizeof ( strm ));
	strm.next_in	= ( Bytef* )in;
	strm.avail_in	= CHUNKSIZE;
	strm.next_out	= ( Bytef* )out;
	strm.avail_out	= CHUNKSIZE;
    
    ret = deflateInit2 ( &strm, level, Z_DEFLATED, -MAX_WBITS, 7, Z_DEFAULT_STRATEGY );
	if ( ret != Z_OK ) return ret;

    // compress until end of file
    do {
		strm.avail_in = source.ReadBytes ( in, CHUNKSIZE );
		strm.next_in = ( Bytef* )in;

		flush = source.IsAtEnd () ? Z_FINISH : Z_NO_FLUSH;
		
        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = ( Bytef* )out;
			ret = deflate ( &strm, flush );    /* no bad return value */
			assert ( ret != Z_STREAM_ERROR );  /* state not clobbered */
			have = CHUNKSIZE - strm.avail_out;
			if ( dest.WriteBytes ( out, have ) != have ) {
				( void )deflateEnd ( &strm );
				return Z_ERRNO;
			}
		} while ( strm.avail_out == 0 );
		assert ( strm.avail_in == 0 );     /* all input will be used */

		/* done when last data in file processed */
	} while ( flush != Z_FINISH );
	assert ( ret == Z_STREAM_END );        /* stream will be complete */

	/* clean up and return */
	( void )deflateEnd ( &strm );
	return Z_OK;
}

//----------------------------------------------------------------//
cc8* USZip::GetErrMsg ( int code ) {

	fputs ( "zpipe: ", stderr );
	switch ( code ) {
		case Z_ERRNO:
			if ( ferror ( stdin ))
				return "zpipe: error reading stdin";
			if ( ferror ( stdout ))
				return "zpipe: error writing stdout";
			break;
		case Z_STREAM_ERROR:
			return "zpipe: invalid compression level";
			break;
		case Z_DATA_ERROR:
			return "zpipe: invalid or incomplete deflate data";
			break;
		case Z_MEM_ERROR:
			return "zpipe: out of memory";
			break;
		case Z_VERSION_ERROR:
			return "zpipe: zlib version mismatch!";
	}
	return "zpipe: ok";
}

//----------------------------------------------------------------//
int USZip::Inflate ( const void* buffer, size_t size, USLeanArray < u8 >& result ) {

	USMemStream outStream;
	
	USByteStream inStream;
	inStream.SetBuffer (( void* )buffer, size );
	inStream.SetLength ( size );
	
	int r = USZip::Inflate ( inStream, outStream );
	
	if ( r == Z_OK ) {
		result.Init ( outStream.GetLength ());
		outStream.Seek ( 0, SEEK_SET );
		outStream.ReadBytes ( result.Data (), result.Size () );
	}
	return r;
}

//----------------------------------------------------------------//
int USZip::Inflate ( USStream& source, USStream& dest ) {

	int ret;
	unsigned have;
	z_stream strm;
	char in [ CHUNKSIZE ];
	char out [ CHUNKSIZE ];

	memset ( &strm, 0, sizeof ( strm ));
	strm.next_in	= ( Bytef* )in;
	strm.avail_in	= CHUNKSIZE;
	strm.next_out	= ( Bytef* )out;
	strm.avail_out	= CHUNKSIZE;

	ret = inflateInit2 ( &strm, -MAX_WBITS );
	if ( ret != Z_OK ) return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = source.ReadBytes ( in, CHUNKSIZE );
        strm.next_in = ( Bytef* )in;
        
		if ( strm.avail_in == 0 ) break;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = ( Bytef* )out;
			ret = inflate ( &strm, Z_NO_FLUSH );
			assert( ret != Z_STREAM_ERROR );  /* state not clobbered */
			switch ( ret ) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;     /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					( void )inflateEnd ( &strm );
					return ret;
			}
			have = CHUNKSIZE - strm.avail_out;
			if ( dest.WriteBytes ( out, have ) != have ) {
				( void )inflateEnd ( &strm );
				return Z_ERRNO;
			}
		} while ( strm.avail_out == 0 );

		/* done when inflate() says it's done */
	} while ( ret != Z_STREAM_END );

	/* clean up and return */
	( void )inflateEnd ( &strm );
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
