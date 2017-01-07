// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOUNDS_H
#define	ZLBOUNDS_H

#include <zl-util/ZLBox.h>

//================================================================//
// ZLBounds
//================================================================//
class ZLBounds :
	public ZLBox {
public:

	static const ZLBounds EMPTY;
	static const ZLBounds GLOBAL;

	enum {
		ZL_BOUNDS_EMPTY,
		ZL_BOUNDS_GLOBAL,
		ZL_BOUNDS_OK,
	};

	u32			mStatus;
	
	//----------------------------------------------------------------//
				ZLBounds			();
				ZLBounds			( u32 status );
};

#endif
