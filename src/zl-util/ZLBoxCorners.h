// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOXCORNERS_H
#define	ZLBOXCORNERS_H

#include <zl-util/ZLVec3D.h>
#include <zl-util/ZLMatrix4x4.h>

class ZLBox;

//================================================================//
// ZLBoxCorners
//================================================================//
class ZLBoxCorners {
public:

	enum {
		MIN_LT_POINT = 0,
		MIN_RT_POINT,
		MIN_RB_POINT,
		MIN_LB_POINT,
		MAX_LT_POINT,
		MAX_RT_POINT,
		MAX_RB_POINT,
		MAX_LB_POINT,
		TOTAL_POINTS,
	};

	ZLVec3D		mPoints [ TOTAL_POINTS ];

	//----------------------------------------------------------------//
	void			Project				( const ZLMatrix4x4& mtx );
	void			Transform			( const ZLMatrix4x4& mtx );
					ZLBoxCorners		();
					ZLBoxCorners		( const ZLBox& box );
};

#endif
