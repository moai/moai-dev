// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLE_H
#define	MOAIPARTICLE_H

class MOAIParticleState;

//================================================================//
// MOAIParticle
//================================================================//
class MOAIParticle {
public:

	float					mAge;
	float					mTerm;
	float					mMass;
	MOAIParticleState*		mState;
	float*					mData;
	
	USVec2D					mLoc;
	USVec2D					mVelocity;
	USVec2D					mOffset;

	MOAIParticle*			mNext;
};

//================================================================//
// MOAIParticleSprite
//================================================================//
class MOAIParticleSprite {
public:
	
	USVec2D		mLoc;
	float		mRot;
	USVec2D		mScl;
	USColorVec	mColor;
	u32			mGfxID;
};

#endif
