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
	
	static const u32	MAX_STEPS = 4;
	
	MOAISurfaceBuffer2D mSurfaceBuffer;
	
	float				mFloorCos;
	float				mCeilCos;
	
	u32					mState;
	
	USVec2D				mMove;
	USVec2D				mLoc;
	USVec2D				mFoot;
	USVec2D				mUp;
	
	const MOAISurface2D* mFloor;
	
	enum {
		STATE_UNKNOWN,
		STATE_ON_FLOOR,
		STATE_IN_AIR,
		STATE_DONE,
	};
	
	//----------------------------------------------------------------//
	void		CalculateWallShove		();
	void		DoFloorStep				();
	void		Move					( MOAIPlatformerBody2D& body );
	void		SnapUp					();
};

#endif
