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
	
	enum {
		PARTICLE_X,
		PARTICLE_Y,
		PARTICLE_DX,
		PARTICLE_DY,
		TOTAL_PARTICLE_REG,
	};
	
	// how much longer the particle will be active
	float mTimeToLive;
	
	// the dynamically allocated array storing four basic particle registers and registers for properties with variance
	float*			mData;
	
	
	MOAICCParticle *mNext;
	
	// The index from the deck this particle should use
	u32 mDeckIndex;
	
	
	// properties from Cocos2D's CCParticle struct.
	// TODO: decide wheter to keep the CCParticle properties
	// I could go with MOAI's way of using dynamically allocated registers for properties with variance.
	
	// position
	float mCurrentPosition[2];
	float mStartPosition[2];
	
	// color
	float mColor[4];
	float mDeltaColor[4];
	
	// particle size
	float mParticleSize;
	float mDeltaParticleSize;
	
	// particle rotation
	float mParticleRotation;
	float mParticleRotation;
	
	// gravity properties
	float mDirection[2];
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
