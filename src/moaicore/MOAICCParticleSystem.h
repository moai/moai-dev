//
//  MOAICCParticleSystem.h
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#ifndef MOAICCPARTICLESYSTEM_H
#define MOAICCPARTICLESYSTEM_H

#include <moaicore/MOAIProp.h>

class MOAICCParticleSystem : public virtual MOAIProp {
private:
	
	enum EmitterType{
		EMITTER_GRAVITY,
		EMITTER_RADIAL,
	};
	
	u32 mNumParticles;
	EmitterType mEmitterType;
	
	float mLifespan;
	float mLifespanVariance;
	float mLifespanTerm[2];
	
	s32  mLifespanRegister;
	
	float mAngle;
	float mAngleVariance;
	s32 mAngleRegister;
	
	float mStartColor[4];
	float mStartColorVariance[4];
	int mStartColorRegister[4];
	
	float mFinishColor[4];
	float mFinishColorVariance[4];
	s32 mFinishColorRegister[4];
	
	float mStartSize;
	float mStartSizeVariance;
	s32	  mStartSizeRegister;
	
	float mFinishSize;
	float mFinishSizeVariance;
	s32	  mFinishSizeRegister;
	
	float mGravity[2];
	
	float mMaxRadius;
	float mMaxRadiusVariance;
	s32	  mMaxRadiusRegister;
	
	float mMinRadius;
	float mMinRadiusVariance;
	s32   mMinRadiusRegister;
	
	float mRadialAcceleration;
	float mRadialAccelVariance;
	s32	  mRadialAccelRegister;
	
	float mTanAccel;
	float mTanAccelVariance;
	s32   mTanAccelRegister;
	
	float mRotStart;
	float mRotStartVariance;
	s32	  mRotStartRegister;
	
	float mRotEnd;
	float mRotEndVariance;
	s32	  mRotEndRegister;
	
	float mSpeed;
	float mSpeedVariance;
	s32	  mSpeedRegister;
	
	float mRotPerSecond;
	float mRotPerSecondVariance;
	
	// Rotational acceleration
	float mRotationalAccel;
	s32 mRotationalAccelRegister;
	
	float mSourcePos[2];
	float mSourcePosVariance[2];
	
	float mDuration;
	
	u32 mBlendFuncSrc;
	u32	mBlendFuncDst;
	
	STLString mTextureName;
	STLString mParticlePath;
	
	// Gravity Registers
	s32 mStartXRegister;
	s32 mStartYRegister;
	s32 mDirectionXRegister;
	s32 mDirectionYRegister;
	
	// Registers for dealing with gravity variance
	s32 mGravityXRegister;
	s32 mGravityYRegister;
	
	// Radial Registers
	s32 mRotPerSecondRegister;
	s32 mRadialRegister;
	
	// Emission information.
	u32 mEmissionCount;
	float mEmissionRate;
	
	// true if the particle system is active
	bool mActive;
	
	// TODO: add methods for accessing properties
	
	static int		_initializeProperties				( lua_State* L );
	static int		_load								( lua_State* L );
	static int		_start								( lua_State* L );
	static int		_stop       						( lua_State* L );
	static int		_reset								( lua_State* L );
	
public:
	DECL_LUA_FACTORY ( MOAICCParticleSystem )
	
					MOAICCParticleSystem	();
					~MOAICCParticleSystem	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};


#endif /* defined(MOAICCPARTICLESYSTEM_H) */
