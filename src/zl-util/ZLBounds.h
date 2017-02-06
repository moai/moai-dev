// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOUNDS_H
#define	ZLBOUNDS_H

#include <zl-util/ZLAccessors.h>
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

	IS ( Empty, mStatus, ZL_BOUNDS_EMPTY )
	
	//----------------------------------------------------------------//
	void		Init					( const ZLBox& box );
	void		Init					( const ZLRect& rect );
	void		Init					( float left, float top, float right, float bottom, float back = 0.0f, float front = 0.0f );
	void		UpdateStatus			();
				ZLBounds				();
				ZLBounds				( u32 status );
				ZLBounds				( const ZLRect& rect );
};

#endif
