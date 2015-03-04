//
//  MOAICCParticle.cpp
//  libmoai
//
//  Created by Isaac Barrett on 3/4/15.
//
//

#include <stdio.h>
#include <moaicore/MOAICCParticle.h>
#include <moaicore/MOAICCParticleSystem.h> 

MOAICCParticle::MOAICCParticle(MOAICCParticleSystem *particleSystem) {
	// timeToLive
	this->mTimeToLive = USFloat::Rand(particleSystem->mMinLifespan, particleSystem->mMaxLifespan);
 
	// position
	this->mStartPosition.mX = particleSystem->mSourcePos[0] + particleSystem->mSourcePosVariance[0] * USFloat::Rand(-1.0f, 1.0f);
	this->mStartPosition.mY = particleSystem->mSourcePos[1] + particleSystem->mSourcePosVariance[1] * USFloat::Rand(-1.0f, 1.0f);
	
	// color
	float start;
	float end;
	
	for (int i = 0; i < 4; ++i) {
		start = USFloat::Clamp(particleSystem->mStartColor[i] + particleSystem->mStartColorVariance[i] * USFloat::Rand(-1.0f, 1.0f), 0.0f, 1.0f);
		
		end = USFloat::Clamp(particleSystem->mFinishColor[i] + particleSystem->mFinishColorVariance[i] * USFloat::Rand(-1.0f, 1.0f), 0.0f, 1.0f);
		
		this->mColor[i] = start;
		
		this->mDeltaColor[i] = (end - start) / this->mTimeToLive;
	}
	
	// partice size
	start = particleSystem->mStartSize + particleSystem->mStartSizeVariance * USFloat::Rand(-1.0f, 1.0f);
	start = MAX(0, start);
	
	this->mParticleSize = start;
	
	end = particleSystem->mFinishParticleSize + particleSystem->mFinishParticleSizeVariance * USFloat::Rand(-1.0f, 1.0f);
	this->mDeltaParticleSize = (end - start) / this->mTimeToLive;
	
	
	// particle rotation
	start = particleSystem->mRotStart + particleSystem->mRotStartVariance * USFloat::Rand(-1.0f, 1.0f);
	end = particleSystem->mRotEnd + particleSystem->mRotEndVariance * USFloat::Rand(-1.0f, 1.0f);
	
	this->mParticleRotation = start;
	this->mDeltaParticleRotation = (end - start) / this->mTimeToLive;
	
	// position
	
	// relative particle position
	USVec3D loc;
	if (particleSystem->mParticlePositionType == MOAICCParticleSystem::PARTICLE_POSITION_RELATIVE) {
		loc = particleSystem->GetLoc();
		this->mStartPosition.Init(loc.mX, loc.mY);
	}
	// free particle position
	else if (particleSystem->mParticlePositionType == MOAICCParticleSystem::PARTICLE_POSITION_FREE) {
		
		loc.Init(0.0f, 0.0f, 0.0f);
		USAffine3D modelToWorld = particleSystem->GetLocalToWorldMtx ();
		modelToWorld.Transform(loc);
		this->mStartPosition.Init(loc.mX, loc.mY);
	}
	
	// set current position
	this->mCurrentPosition.Init(this->mStartPosition);
	
	// direction
	float a = D2R * (particleSystem->mAngle + particleSystem->mAngleVariance * USFloat::Rand(-1.0f, 1.0f));
	
	
	// gravity mode
	if ( particleSystem->mEmitterType == MOAICCParticleSystem::EMITTER_GRAVITY ) {
		USVec2D v;
		v.Init(Cos(a), Sin(a));
		
		float s = particleSystem->mSpeed + particleSystem->mSpeedVariance * USFloat::Rand(-1.0f, 1.0f);
		v.Scale( s );
		
		// direction
		this->mDirection.Init(v);
		
		// gravity variance
		this->mGravity.mX = particleSystem->mGravity[0] + particleSystem->mGravityVariance[0] * USFloat::Rand(-1.0f, 1.0f);
		this->mGravity.mY = particleSystem->mGravity[1] + particleSystem->mGravityVariance[1] * USFloat::Rand(-1.0f, 1.0f);
		
		// radial accel
		this->mRadialAcceleration = particleSystem->mRadialAcceleration + particleSystem->mRadialAccelVariance * USFloat::Rand(-1.0f, 1.0f);
		
		// tangential accel
		this->mTangentialAcceleration = particleSystem->mTangentialAcceleration + particleSystem->mTangentialAccelVariance * USFloat::Rand(-1.0f, 1.0f);
		
	}
	// radial mode
	else {
		float startRadius = particleSystem->mEndRadius + particleSystem->mEndRadiusVariance *  USFloat::Rand(-1.0f, 1.0f);
		float endRadius = particleSystem->mStartRadius + particleSystem->mStartRadiusVariance *  USFloat::Rand(-1.0f, 1.0f);
		
		this->mRadius = startRadius;
		
		this->mDeltaRadius = (endRadius - startRadius) / this->mTimeToLive;
		
		this->mAngle = a;
		this->mDegreesPerSecond = D2R * (particleSystem->mRotatePerSecond + particleSystem->mRotatePerSecondVariance * USFloat::Rand(-1.0f, 1.0f));
		
		// rotational acceleration
		this->mRotationalAcceleration = D2R * (particleSystem->mRotationalAcceleration + particleSystem->mRotationalAccelVariance * USFloat::Rand(-1.0f, 1.0f));
		
	}
	
	// deck index
	if (particleSystem->mIndexMode == MOAICCParticleSystem::INDEX_MODE_RANDOM) { // random
		u32 endIdx;
		u32 startIdx;
		if (particleSystem->mEndIndex > particleSystem->mStartIndex) {
			endIdx = particleSystem->mEndIndex;
			startIdx = particleSystem->mStartIndex;
		} else {
			endIdx = particleSystem->mStartIndex;
			startIdx = particleSystem->mEndIndex;
		}
		
		this->mDeckIndex = (u32) Rand(startIdx, endIdx);
	} else { // sequential
		u32 span = (particleSystem->mEndIndex - particleSystem->mStartIndex) + 1;
		if (particleSystem->mEndIndex > particleSystem->mStartIndex) {
			this->mDeckIndex = particleSystem->mStartIndex + (particleSystem->mSeqIndex++ % span);
		} else {
			this->mDeckIndex = particleSystem->mStartIndex;
		}
	}
	
}

// move constructor
MOAICCParticle::MOAICCParticle(MOAICCParticle&& other)
	:mTimeToLive(other.mTimeToLive),
	mDeckIndex(other.mDeckIndex),
	mCurrentPosition(other.mCurrentPosition),
	mStartPosition(other.mStartPosition),
	mParticleSize(other.mParticleSize),
	mDeltaParticleSize(other.mDeltaParticleSize),
	mParticleRotation(other.mParticleRotation),
	mDeltaParticleRotation(other.mDeltaParticleRotation),
	mDirection(other.mDirection),
	mGravity(other.mGravity),
	mRadialAcceleration(other.mRadialAcceleration),
	mTangentialAcceleration(other.mTangentialAcceleration),
	mAngle(other.mAngle),
	mDegreesPerSecond(other.mDegreesPerSecond),
	mRadius(other.mRadius),
	mDeltaRadius(other.mDeltaRadius),
	mRotationalAcceleration(other.mRotationalAcceleration)
{
	for (int index = 0; index < 4; ++index) {
		mColor[index] = other.mColor[index];
		mDeltaColor[index] = other.mDeltaColor[index];
	}
	
	other.mTimeToLive = 0.0f;
	other.mDeckIndex = 0;
	other.mCurrentPosition.Init(0.0f, 0.0f);
	other.mStartPosition.Init(0.0f, 0.0f);
	other.mParticleSize = 0.0f;
	other.mDeltaParticleSize = 0.0f;
	other.mParticleRotation = 0.0f;
	other.mDeltaParticleRotation = 0.0f;
	other.mDirection.Init(0.0f, 0.0f);
	other.mGravity.Init(0.0f, 0.0f);
	other.mRadialAcceleration = 0.0f;
	other.mTangentialAcceleration = 0.0f;
	other.mAngle = 0.0f;
	other.mDegreesPerSecond = 0.0f;
	other.mRadius = 0.0f;
	other.mDeltaRadius = 0.0f;
	other.mRotationalAcceleration = 0.0f;
	
	for (int index = 0; index < 4; ++index) {
		other.mColor[index] = 0.0f;
		other.mDeltaColor[index] = 0.0f;
	}
}

// copy constructor
MOAICCParticle::MOAICCParticle(const MOAICCParticle& other)
	:mTimeToLive(other.mTimeToLive),
	mDeckIndex(other.mDeckIndex),
	mCurrentPosition(other.mCurrentPosition),
	mStartPosition(other.mStartPosition),
	mParticleSize(other.mParticleSize),
	mDeltaParticleSize(other.mDeltaParticleSize),
	mParticleRotation(other.mParticleRotation),
	mDeltaParticleRotation(other.mDeltaParticleRotation),
	mDirection(other.mDirection),
	mGravity(other.mGravity),
	mRadialAcceleration(other.mRadialAcceleration),
	mTangentialAcceleration(other.mTangentialAcceleration),
	mAngle(other.mAngle),
	mDegreesPerSecond(other.mDegreesPerSecond),
	mRadius(other.mRadius),
	mDeltaRadius(other.mDeltaRadius),
	mRotationalAcceleration(other.mRotationalAcceleration)
{
	for (int index = 0; index < 4; ++index) {
		mColor[index] = other.mColor[index];
		mDeltaColor[index] = other.mDeltaColor[index];
	}
}


