// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERFSM2D_H
#define	MOAIPLATFORMERFSM2D_H

#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// MOAIPlatformerFsm2D
//================================================================//
class MOAIPlatformerFsm2D {
private:
	
	friend class MOAIPlatformerBody2D;
	
	static const u32	MAX_STEPS = 8;
	
	MOAISurfaceBuffer2D mSurfaceBuffer;
	
	float				mFloorCos;
	float				mCeilCos;
	
	u32					mState;
	
	USVec2D				mMove;					// current move (original vector)
	
	bool				mWallToLeft;
	bool				mWallToRight;
	
	float				mLeftWallDepth;
	float				mRightWallDepth;
	
	float				mMoveDistOnFloor;		// distance to move along floor (scalar on floor tangent)
	float				mShoveDistOnFloor;		// distance to shove along floor (scalar on floor tangent)
	float				mShoveDistInAir;
	
	USVec2D				mLoc;
	USVec2D				mFoot;
	USVec2D				mUp;
	
	const MOAISurface2D*	mFloor;				// may be nil even if state is ON_FLOOR ('ghost' platform from snap)
	USVec2D					mFloorNorm;
	USVec2D					mFloorTangent;
	
	enum {
		STATE_DONE,
		STATE_IN_AIR,
		STATE_ON_FLOOR,
	};
	
	//----------------------------------------------------------------//
	void		CalculateWallDepthInAir			();
	void		CalculateWallDepthOnFloor		();
	void		CalculateWallShoveInAir			();
	void		CalculateWallShoveOnFloor		();
	void		DoMoveInAir						();
	void		DoMoveOnFloor					();
	void		DoWallSnapInAir					();
	void		Move							( MOAIPlatformerBody2D& body );
	void		SetFloor						( const MOAISurface2D& floor );
	void		SnapUp							();
};

#endif
