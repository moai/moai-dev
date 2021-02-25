// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOUNDS_H
#define	ZLBOUNDS_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLBox.h>
#include <zl-core/ZLPrism.h>

//================================================================//
// ZLBounds
//================================================================//
class ZLBounds {
private:

	enum {
		ZL_BOUNDS_EMPTY,
		ZL_BOUNDS_FITTED,
		ZL_BOUNDS_GLOBAL,
	};

	u32			mStatus;

	//----------------------------------------------------------------//
	void		UpdateStatus			();
				ZLBounds				( u32 status );

public:

	static const ZLBounds EMPTY;
	static const ZLBounds GLOBAL;

	ZLBox		mAABB;
	ZLPrism		mOBB;

	HAS ( Geometry, mStatus, ZL_BOUNDS_FITTED )
	IS ( Empty, mStatus, ZL_BOUNDS_EMPTY )
	IS ( Global, mStatus, ZL_BOUNDS_GLOBAL )
	
	//----------------------------------------------------------------//
	void		Init					( const ZLBox& box );
	void		Init					( const ZLPrism& prism );
	void		Init					( const ZLRect& rect );
	void		Init					( float left, float top, float right, float bottom, float back = 0.0f, float front = 0.0f );
				ZLBounds				();
				ZLBounds				( const ZLBox& box );
				ZLBounds				( const ZLPrism& prism );
				ZLBounds				( const ZLRect& rect );
				ZLBounds				( float left, float top, float right, float bottom, float back = 0.0f, float front = 0.0f );
};

#endif
