// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLRINGADAPTER_H
#define ZLRINGADAPTER_H

#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLRingAdapter
//================================================================//
class ZLRingAdapter :
	public ZLStreamAdapter {
protected:

	//----------------------------------------------------------------//
	size_t				Process					( void* readBuffer, const void* writeBuffer, size_t size );
	
public:

	//----------------------------------------------------------------//
	size_t				GetCursor				();
	bool				IsAtEnd					();
	size_t				ReadBytes				( void* buffer, size_t size );
	int					SetCursor				( long offset );
	size_t				SetLength				( size_t length );
	size_t				WriteBytes				( const void* buffer, size_t size );
						ZLRingAdapter			();
						~ZLRingAdapter			();
};

#endif
