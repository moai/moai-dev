// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBASE64READER_H
#define ZLBASE64READER_H

#include <zl-core/ZLBase64Encoder.h>
#include <zl-core/ZLStreamAdapter.h>

//================================================================//
// ZLBase64Reader
//================================================================//
class ZLBase64Reader :
	public ZLStreamAdapter {
private:

	ZLBase64Encoder		mEncoder;
	u8					mPlainBlock [ ZLBase64Encoder::PLAIN_BLOCK_SIZE ];
	size_t				mBlockID;
	u32					mBlockTop;

	//----------------------------------------------------------------//
	void				OnClose					();
	ZLSizeResult		ReadBytes				( void* buffer, size_t size );
	void				SyncBlock				();

public:

	//----------------------------------------------------------------//
	static size_t		EstimateDecodedLength	( size_t encodedLength );
	u32					GetCaps					();
	int					SetCursor				( size_t offset );
						ZLBase64Reader			();
						~ZLBase64Reader			();
};

#endif
