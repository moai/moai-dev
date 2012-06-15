// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USFRUSTUM_H
#define	USFRUSTUM_H

#include <uslscore/USAffine3D.h>
#include <uslscore/USBox.h>
#include <uslscore/USMatrix4x4.h>
#include <uslscore/USPlane.h>
#include <uslscore/USVec3D.h>

class USPrism;
class USRhombus;

//================================================================//
// USFrustum
//================================================================//
class USFrustum {
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

	USBox		mAABB;
	USVec3D		mPoints [ TOTAL_POINTS ];
	USPlane3D	mPlanes [ TOTAL_PLANES ];
	bool		mUsePlanes;

	//----------------------------------------------------------------//
	bool	Cull			( const USVec3D& vec ) const;
	bool	Cull			( const USBox& box ) const;
	bool	Cull			( const USPrism& prism ) const;
	bool	Cull			( const USRhombus& rhombus ) const;
	bool	GetXYSectRect	( const USAffine3D& mtx, USRect& rect ) const;
	void	Init			( const USMatrix4x4& mtx );
	//void	Init			( USVec3D& loc, USRhombus rhombus, USPlane3D& near, USPlane3D& far );
	//void	Init			( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV );
			USFrustum		();
};

#endif
