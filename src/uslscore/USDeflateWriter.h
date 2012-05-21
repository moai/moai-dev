// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDEFLATEWRITER_H
#define USDEFLATEWRITER_H

#include <zlib.h>
#include <uslscore/USStream.h>

#define US_DEFLATE_WRITER_CHUNK_SIZE	2048

//================================================================//
// USDeflateWriter
//================================================================//
class USDeflateWriter :
	public USStream {
private:

	USStream*			mOutputStream;			// compressed input stream
	size_t				mUncompressedCursor;	// cursor in the output stream
	z_stream			mZStream;				// underlying zip stream state
	
	//----------------------------------------------------------------//
	size_t				Deflate					( const void* src, size_t size );

public:

	static const u32 DEFAULT_LEVEL = 9;
	static const int DEFAULT_WBITS = -15;

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream& stream, int level = DEFAULT_LEVEL, int windowBits = DEFAULT_WBITS );
						USDeflateWriter			();
						~USDeflateWriter		();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
