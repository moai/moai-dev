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
	
	USVec2D				mMove;					// current move remaining (original vector)
	USVec2D				mProjectedMove;			// current move projected onto floor
	float				mProjectedMoveDist;		// length of projected move along floor
	
	USVec2D				mLoc;
	USVec2D				mFoot;
	USVec2D				mUp;
	
	const MOAISurface2D*	mFloor;				// may be nil even if state is ON_FLOOR ('ghost' platform from snap)
	USVec2D					mFloorNorm;
	USVec2D					mFloorTangent;
	
	enum {
		STATE_UNKNOWN,
		STATE_ON_FLOOR,
		STATE_IN_AIR,
		STATE_DONE,
	};
	
	//----------------------------------------------------------------//
	void		ApplyMoveOnFloor				( const MOAISurface2D* hit, float time );
	void		ApplyWallShoveOnFloor			( bool wallToLeft, bool wallToRight, float leftWallDepth, float rightWallDepth );
	void		CalculateWallShoveOnFloor		();
	void		DoFloorStep						();
	void		Move							( MOAIPlatformerBody2D& body );
	void		ProjectMove						();
	void		SetFloor						( const MOAISurface2D& floor );
	void		SnapUp							();
};

#endif
