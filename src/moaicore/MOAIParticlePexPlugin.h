// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEPEXPLUGIN_H
#define	MOAIPARTICLEPEXPLUGIN_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleScript.h>
#include <aku/AKU-particles.h>
#include <tinyxml.h>
#include <uslscore/STLString.h>

//================================================================//
// MOAIParticlePexPlugin
//================================================================//
/**	@name	MOAIParticlePexPlugin
	@text	Allows custom particle processing  derived from .pex file via C language callback.
*/
class MOAIParticlePexPlugin :
	public virtual MOAIParticlePlugin {
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
	static int		_getAngle							( lua_State* L );
	static int		_setAngle							( lua_State* L );
	static int		_getAngleVariance					( lua_State* L );
	static int		_setAngleVariance					( lua_State* L );
	static int		_getBlendMode						( lua_State* L );
	static int		_setBlendMode						( lua_State* L );
	static int		_getDuration						( lua_State* L );
	static int		_setDuration						( lua_State* L );
	static int		_getEmission						( lua_State* L );
	static int		_getEmitterType						( lua_State* L );
	static int		_setEmitterType						( lua_State* L );
	static int		_getFinishColor						( lua_State* L );
	static int		_setFinishColor						( lua_State* L );
	static int		_getFinishColorVariance				( lua_State* L );
	static int		_setFinishColorVariance				( lua_State* L );
	static int		_getFinishParticleSize				( lua_State* L );
	static int		_setFinishParticleSize				( lua_State* L );
	static int		_getFinishParticleSizeVariance		( lua_State* L );
	static int		_setFinishParticleSizeVariance		( lua_State* L );
	static int		_getFrequency						( lua_State* L );
	static int		_getGravity							( lua_State* L );
	static int		_setGravity							( lua_State* L );
	static int		_getLifespan						( lua_State* L );
	static int		_getMaxParticles					( lua_State* L );
	static int		_setMaxParticles					( lua_State* L );
	static int		_getMaxRadius						( lua_State* L );
	static int		_setMaxRadius						( lua_State* L );
	static int		_getMaxRadiusVariance				( lua_State* L );
	static int		_setMaxRadiusVariance				( lua_State* L );
	static int		_getMinRadius						( lua_State* L );
	static int		_setMinRadius						( lua_State* L );
	static int		_getMinRadiusVariance				( lua_State* L );
	static int		_setMinRadiusVariance				( lua_State* L );
	static int		_getParticleLifespan				( lua_State* L );
	static int		_setParticleLifespan				( lua_State* L );
	static int		_getParticleLifespanVariance		( lua_State* L );
	static int		_setParticleLifespanVariance		( lua_State* L );
	static int		_getRadialAcceleration				( lua_State* L );
	static int		_setRadialAcceleration				( lua_State* L );
	static int		_getRadialAccelVariance				( lua_State* L );
	static int		_setRadialAccelVariance				( lua_State* L );
	static int		_getRect							( lua_State* L );
	static int		_getRotatePerSecond					( lua_State* L );
	static int		_setRotatePerSecond					( lua_State* L );
	static int		_getRotatePerSecondVariance			( lua_State* L );
	static int		_setRotatePerSecondVariance			( lua_State* L );
	static int		_getRotationEnd						( lua_State* L );
	static int		_setRotationEnd						( lua_State* L );
	static int		_getRotationEndVariance				( lua_State* L );
	static int		_setRotationEndVariance				( lua_State* L );
	static int		_getRotationStart					( lua_State* L );
	static int		_setRotationStart					( lua_State* L );
	static int		_getRotationStartVariance			( lua_State* L );
	static int		_setRotationStartVariance			( lua_State* L );
	static int		_getSourcePosition					( lua_State* L );
	static int		_setSourcePosition					( lua_State* L );
	static int		_getSourcePositionVariance			( lua_State* L );
	static int		_setSourcePositionVariance			( lua_State* L );
	static int		_getSpeed							( lua_State* L );
	static int		_setSpeed							( lua_State* L );
	static int		_getSpeedVariance					( lua_State* L );
	static int		_setSpeedVariance					( lua_State* L );
	static int		_getStartColor						( lua_State* L );
	static int		_setStartColor						( lua_State* L );
	static int		_getStartColorVariance				( lua_State* L );
	static int		_setStartColorVariance				( lua_State* L );
	static int		_getStartParticleSize				( lua_State* L );
	static int		_setStartParticleSize				( lua_State* L );
	static int		_getStartParticleSizeVariance		( lua_State* L );
	static int		_setStartParticleSizeVariance		( lua_State* L );
	static int		_getTangentalAcceleration			( lua_State* L );
	static int		_setTangentalAcceleration			( lua_State* L );
	static int		_getTangentalAccelerationVariance	( lua_State* L );
	static int		_setTangentalAccelerationVariance	( lua_State* L );

	static int		_getTextureName						( lua_State* L );
	static int		_initializeProperties				( lua_State* L );
	static int		_load								( lua_State* L );

	static void     InitializeBasicRegisters    ( MOAIParticlePexPlugin& plugin );
	static void		InitializeEmitter			( MOAIParticlePexPlugin& plugin );
	void			InitializeProperties		();
	static void		Parse						( cc8* filename, MOAIParticlePexPlugin& plugin, TiXmlNode* node );
	
	void			_initGravityScript			( float* particle, float* registers );
	void			_initRadialScript			( float* particle, float* registers );
	void			_renderGravityScript		( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1,  float term );
	void			_renderRadialScript		    ( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1,  float term );

public:
	
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticlePexPlugin )
	
	//----------------------------------------------------------------//
					MOAIParticlePexPlugin		();
					~MOAIParticlePexPlugin		();
	void			OnInit						( float* particle, float* registers );
	void			OnRender					( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};
	
#endif