// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDEFLATEWRITER_H
#define ZLDEFLATEWRITER_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLStreamAdapter.h>
#include <zlib.h>

#define ZL_DEFLATE_WRITER_CHUNK_SIZE	2048

//================================================================//
// ZLDeflateWriter
//================================================================//
class ZLDeflateWriter :
	public ZLStreamAdapter {
private:

	z_stream			mZStream;				// underlying zip stream state
	
	int					mCompressionLevel;
	int					mWindowBits;
	
	//----------------------------------------------------------------//
	ZLSizeResult		Deflate					( const void* src, size_t size );
	void				OnClose					();
	ZLResultCode		OnOpen					();
	ZLSizeResult		WriteBytes				( const void* buffer, size_t size );

public:

	static const u32 DEFAULT_LEVEL = 9;
	static const int DEFAULT_WBITS = -15;

	GET_SET ( int, CompressionLevel, mCompressionLevel )
	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
	u32					GetCaps					();
						ZLDeflateWriter			();
						~ZLDeflateWriter		();
};

#endif
