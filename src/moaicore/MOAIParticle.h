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

	enum {
		PARTICLE_X,
		PARTICLE_Y,
		PARTICLE_DX,
		PARTICLE_DY,
		TOTAL_PARTICLE_REG,
	};

	float					mAge;
	float					mTerm;
	float					mMass;
	MOAIParticleState*		mState;
	float*					mData;

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
