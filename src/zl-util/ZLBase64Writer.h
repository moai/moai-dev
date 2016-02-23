// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBASE64WRITER_H
#define ZLBASE64WRITER_H

#include <zl-util/ZLBase64Encoder.h>
#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLBase64Writer
//================================================================//
class ZLBase64Writer :
	public ZLStreamAdapter {
private:
	
	ZLBase64Encoder		mEncoder;
	u8					mPlainBlock [ ZLBase64Encoder::PLAIN_BLOCK_SIZE ];

	//----------------------------------------------------------------//
	void				OnClose					();
	ZLResultCode		OnOpen					();
	ZLSizeResult		WriteBytes				( const void* buffer, size_t size );

public:

	//----------------------------------------------------------------//
	static size_t		EstimateEncodedLength	( size_t plainLength );
	u32					GetCaps					();
						ZLBase64Writer			();
						~ZLBase64Writer			();
};

#endif
