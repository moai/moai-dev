// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFRUSTUMFITTER_H
#define	ZLFRUSTUMFITTER_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLBox.h>
#include <zl-util/ZLPlane.h>
#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLFrustumFitter
//================================================================//
class ZLFrustumFitter {
private:

	enum {
		LEFT_PLANE = 0,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		TOTAL_PLANES,
	};

	ZLPlane3D	mPlanes [ TOTAL_PLANES ];
	ZLVec3D		mPosition;

	bool		mNeedsInit;
	bool		mIsDirty;

public:

	//----------------------------------------------------------------//
	void		FitBox			( const ZLBox& box, float pad );
	void		FitPoint		( const ZLVec3D& loc, float radius );
	ZLVec3D		GetPosition		();
	void		Init			( const ZLRect& viewRect, const ZLRect& fitRect, float hFov, const ZLAffine3D& orient );
	void		Reset			();
};

#endif
