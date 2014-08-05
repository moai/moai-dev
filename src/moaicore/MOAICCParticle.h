//
//  MOAICCParticle.h
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#ifndef MOAICCPARTICLE_H
#define MOAICCPARTICLE_H

#include "pch.h"

class MOAICCParticle {
public:
	
	// how much longer the particle will be active
	float mTimeToLive;
	
	MOAICCParticle *mNext;
	
	// The index from the deck this particle should use
	u32 mDeckIndex;
	
	
	// properties from Cocos2D's CCParticle struct.
	
	// position
	USVec2D mCurrentPosition;
	USVec2D mStartPosition;
	
	// color
	float mColor[4];
	float mDeltaColor[4];
	
	// particle size
	float mParticleSize;
	float mDeltaParticleSize;
	
	// particle rotation
	float mParticleRotation;
	float mDeltaParticleRotation;
	
	// gravity properties
	USVec2D mDirection;
	USVec2D mGravity;
	float mRadialAcceleration;
	float mTangentialAcceleration;
	
	// radial properties
	float mAngle;
	float mDegreesPerSecond;
	float mRadius;
	float mDeltaRadius;
	float mRotationalAcceleration;
	
};

#endif
