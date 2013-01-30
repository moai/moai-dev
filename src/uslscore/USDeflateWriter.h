// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDEFLATEWRITER_H
#define USDEFLATEWRITER_H

#include <uslscore/USAccessors.h>
#include <uslscore/USStreamWriter.h>
#include <zlib.h>

#define US_DEFLATE_WRITER_CHUNK_SIZE	2048

//================================================================//
// USDeflateWriter
//================================================================//
class USDeflateWriter :
	public USStreamWriter {
private:

	USStream*			mOutputStream;			// compressed output stream
	size_t				mUncompressedCursor;	// cursor in the input stream
	z_stream			mZStream;				// underlying zip stream state
	
	int					mCompressionLevel;
	int					mWindowBits;
	
	//----------------------------------------------------------------//
	size_t				Deflate					( const void* src, size_t size );

public:

	static const u32 DEFAULT_LEVEL = 9;
	static const int DEFAULT_WBITS = -15;

	GET_SET ( int, CompressionLevel, mCompressionLevel )
	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream* stream );
						USDeflateWriter			();
						~USDeflateWriter		();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
