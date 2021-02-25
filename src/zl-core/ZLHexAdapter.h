// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHEXADAPTER_H
#define ZLHEXADAPTER_H

#include <zl-core/ZLStreamAdapter.h>

//================================================================//
// ZLHexAdapter
//================================================================//
class ZLHexAdapter :
	public ZLStreamAdapter {
	
	//----------------------------------------------------------------//
	ZLSizeResult		ReadBytes				( void* buffer, size_t size );
	ZLSizeResult		WriteBytes				( const void* buffer, size_t size );
	
public:

	//----------------------------------------------------------------//
	static size_t		EstimateDecodedLength	( size_t encodedLength );
	static size_t		EstimateEncodedLength	( size_t plainLength );
	u32					GetCaps					();
	static u8			HexToByte				( u32 c );
						ZLHexAdapter			();
						~ZLHexAdapter			();
};

#endif
