// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USFRUSTFITTER_H
#define	USFRUSTFITTER_H

#include <uslscore/USVec3D.h>

class USFrustum;
class USRhombus;

//================================================================//
// USFrustFitter
//================================================================//
class USFrustFitter {
private:

	USFrustum*	mFrustum;
	u32			mTotalPoints;
	float		mBestDist [ 4 ];
	USVec3D		mBestLoc [ 4 ];
	
	//----------------------------------------------------------------//
	void	ShrinkPlane			( u32 id );
			USFrustFitter		();
	
public:

	//----------------------------------------------------------------//
	void	Evaluate			( USVec3D& vec );
	void	Evaluate			( USRhombus& rhombus );
	void	Reset				();
	void	ShrinkFrustum		();
			USFrustFitter		( USFrustum& frustum );
};

#endif
