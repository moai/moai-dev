// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPEXPARTICLE_H
#define	MOAIPEXPARTICLE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleScript.h>
#include <aku/AKU-particles.h>
#include <tinyxml.h>
#include <uslscore/STLString.h>

//================================================================//
// MOAIParticlePlugin
//================================================================//
/**	@name	MOAIParticlePlugin
	@text	Allows custom particle processing via C language callbacks.
*/

class MOAIPexParticle :
	public virtual MOAILuaObject {
private:
	enum EmitterType{
		EMITTER_GRAVITY,
		EMITTER_RADIAL,
	};

	int	 mSize;

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
	
	float mRadialAcceleration;
	float mRadialAccelVariance;
	u32	  mRadialAccelRegister;
	
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

	// Radial Registers
	s32 mRotPerSecondRegister;
	s32 mRadialRegister;

	// Emission information.
	u32 mEmissionCount;
	float mEmissionRate;
	
	//----------------------------------------------------------------//
	static int		_getBlendMode		( lua_State* L );
	static int		_getDuration		( lua_State* L );
	static int		_getEmission		( lua_State* L );
	static int		_getFrequency		( lua_State* L );
	static int		_getLifespan		( lua_State* L );
	static int		_getMaxParticles	( lua_State* L );
	static int		_getRect			( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_getTextureName		( lua_State* L );
	static int		_load				( lua_State* L );

	static void		Parse						( MOAILuaState& state, MOAIPexParticle& plugin, TiXmlNode* node );
	void			InitFunc					( float* particle, float* registers );
	void			RenderFunc					( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 );
	void			_initGravityScript			( float* particle, float* registers );
	void			_initRadialScript			( float* particle, float* registers );
	void			_renderGravityScript		( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 );
	void			_renderRadialScript		    ( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 );

public:
	
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIPexParticle )
	
	//----------------------------------------------------------------//
					MOAIPexParticle			();
					~MOAIPexParticle			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};
	
#endif
