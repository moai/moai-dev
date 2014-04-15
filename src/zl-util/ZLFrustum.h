// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFRUSTUM_H
#define	ZLFRUSTUM_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLBox.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLPlane.h>
#include <zl-util/ZLVec3D.h>

class ZLPrism;
class ZLRhombus;

//================================================================//
// ZLFrustum
//================================================================//
class ZLFrustum {
public:

	enum {
		LEFT_PLANE = 0,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		NEAR_PLANE,
		FAR_PLANE,
		TOTAL_PLANES,
	};
	
	enum {
		NEAR_LT_POINT = 0,
		NEAR_RT_POINT,
		NEAR_RB_POINT,
		NEAR_LB_POINT,
		FAR_LT_POINT,
		FAR_RT_POINT,
		FAR_RB_POINT,
		FAR_LB_POINT,
		TOTAL_POINTS,
	};

	ZLBox		mAABB;
	ZLVec3D		mPoints [ TOTAL_POINTS ];
	ZLPlane3D	mPlanes [ TOTAL_PLANES ];
	bool		mUsePlanes;

	//----------------------------------------------------------------//
	bool	Cull			( const ZLVec3D& vec ) const;
	bool	Cull			( const ZLBox& box ) const;
	bool	Cull			( const ZLPrism& prism ) const;
	bool	Cull			( const ZLRhombus& rhombus ) const;
	bool	GetXYSectRect	( const ZLAffine3D& mtx, ZLRect& rect ) const;
	void	Init			( const ZLMatrix4x4& invViewProjMtx );
	//void	Init			( ZLVec3D& loc, ZLRhombus rhombus, ZLPlane3D& near, ZLPlane3D& far );
	//void	Init			( ZLVec3D& loc, ZLVec3D& viewAxis, ZLVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV );
			ZLFrustum		();
};

#endif
