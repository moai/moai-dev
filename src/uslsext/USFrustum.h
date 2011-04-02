// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef FRUSTUM_H
#define	FRUSTUM_H

#include <uslsext/USPlane.h>
#include <uslsext/USBox.h>

//================================================================//
// USFrustum
//================================================================//
class USFrustum {
private:
	
	//----------------------------------------------------------------//
	void	UpdateHull		();
	
public:

	friend class USFrustShrinker;

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

	USVec3D		mLoc;
	USBox		mAABB;
	USVec3D		mPoints [ TOTAL_POINTS ];

	USPlane3D	mPlanes [ TOTAL_PLANES ];

	//----------------------------------------------------------------//
	bool	Cull		( USVec3D& vec ); // Culls by all planes
	bool	Cull		( USBox& box ); // Culls by all planes
	bool	Cull		( USPrism& prism ); // Culls by all planes
	bool	Cull		( USRhombus& rhombus ); // Culls by all planes
	void	Init		( USVec3D& loc, USRhombus& rhombus, USPlane3D& near, USPlane3D& far );
	void	Init		( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float nearD, float farD, float hFOV, float vFOV );
			USFrustum	();
};

//================================================================//
// USFrustShrinker
//================================================================//
class USFrustShrinker {
private:

	USFrustum*	mFrustum;
	u32			mTotalPoints;
	float		mBestDist [ 4 ];
	USVec3D		mBestLoc [ 4 ];
	
	//----------------------------------------------------------------//
	void	ShrinkPlane			( u32 id );
			USFrustShrinker		();
	
public:

	//----------------------------------------------------------------//
	void	Evaluate			( USVec3D& vec );
	void	Evaluate			( USRhombus& rhombus );
	void	Reset				();
	void	ShrinkFrustum		();
			USFrustShrinker		( USFrustum& frustum );
};

//================================================================//
// USFrustCone
//================================================================//
class USFrustCone {
public:

	enum {
		LEFT_PLANE,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		TOTAL_PLANES,
	};

	USVec3D	mLoc;
	USPlane3D	mPlanes		[ TOTAL_PLANES ];
	
	//----------------------------------------------------------------//
	bool	Cull		( USBox& box ); // Culls by all planes
	bool	Cull		( USPrism& prism ); // Culls by all planes
	bool	Cull		( USRhombus& rhombus ); // Culls by all planes
	bool	CullConvex	( USVec3D* convex, u32 total  ); // Culls by all planes
	void	Init		( USFrustum& frustum );
	void	Init		( USVec3D& loc, USRhombus& rhombus );
	void	Init		( USVec3D& loc, USVec3D& viewAxis, USVec3D& worldUp, float hFOV, float vFOV );
};

//================================================================//
// USFrustboxum
//================================================================//
class USFrustboxum {
public:

	USFrustCone	mFrustCone;
	USBox			mAABB;
	
	//----------------------------------------------------------------//
	bool	Cull		( USBox& box );
};

#endif
