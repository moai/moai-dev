/* zpipe.c: example of proper use of zlib's inflate() and deflate()
   Not copyrighted -- provided to the public domain
   Version 1.2  9 November 2004  Mark Adler */

/* Version history:
   1.0  30 Oct 2004  First version
   1.1   8 Nov 2004  Add void casting for unused return values
                     Use switch statement for inflate() return values
   1.2   9 Nov 2004  Add assertions to document zlib guarantees
   1.3   6 Apr 2005  Remove incorrect assertion in inf()
*/

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

#include <uslsext/USZip.h>

//----------------------------------------------------------------//
/**	@brief Compresses contents of source stream and stores in dest
	stream.  Continues until EOF is encountered.
	
	@param source The input stream.
	@param dest The output stream.
	@param level Compression level.  0 - archive only.  9 - best.
	@return Z_OK on success.
*/
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

		flush = source.Done () ? Z_FINISH : Z_NO_FLUSH;
		
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
/**	@brief Reports a zlib error.
	
	@param code The return code to report.
	@return Message string corresponding to the code.
*/
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
/**	@brief Decompresses contents of source stream and stores in dest
	stream.  Continues until EOF is encountered.
	
	@param source The input stream.
	@param dest The output stream.
	@return Z_OK on success.
*/
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
