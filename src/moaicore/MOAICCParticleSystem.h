//
//  MOAICCParticleSystem.h
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#ifndef MOAICCPARTICLESYSTEM_H
#define MOAICCPARTICLESYSTEM_H

#include <tinyxml.h>

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAICCParticle.h>


class MOAICCParticleSystem : public MOAIProp, public MOAIAction {
private:
	
	enum EmitterType {
		EMITTER_GRAVITY,
		EMITTER_RADIAL,
	};
	
	enum ParticlePositionType {
		PARTICLE_POSITION_FREE = 0,
		PARTICLE_POSITION_RELATIVE,
		PARTICLE_POSITION_GROUPED,
	};
	
	
	// Array of particles.
	MOAICCParticle *mParticles;
	u32 mParticleCount;
	u32 mAllocatedParticles;
	
	// Maximum particles.
	u32 mTotalParticles;
	
	
	EmitterType mEmitterType;
	
	float mLifespan;
	float mLifespanVariance;
	float mLifespanTerm[2];
	
	float mAngle;
	float mAngleVariance;
	
	float mStartColor[4];
	float mStartColorVariance[4];
	
	float mFinishColor[4];
	float mFinishColorVariance[4];
	
	float mStartSize;
	float mStartSizeVariance;
	
	float mFinishSize;
	float mFinishSizeVariance;
	
	float mGravity[2];
	float mGravityVariance[2];
	
	float mMaxRadius;
	float mMaxRadiusVariance;
	
	float mMinRadius;
	float mMinRadiusVariance;
	
	float mRadialAcceleration;
	float mRadialAccelVariance;
	
	float mTangentialAcceleration;
	float mTangentialAccelVariance;
	
	float mRotStart;
	float mRotStartVariance;
	
	float mRotEnd;
	float mRotEndVariance;
	
	float mSpeed;
	float mSpeedVariance;
	
	float mRotPerSecond;
	float mRotPerSecondVariance;
	
	// Rotational acceleration
	float mRotationalAcceleration;
	float mRotationalAccelVariance;
	
	float mSourcePos[2];
	float mSourcePosVariance[2];
	
	float mDuration;
	
	u32 mBlendFuncSrc;
	u32	mBlendFuncDst;
	
	STLString mTextureName;
	STLString mParticlePath;
	
	// Emission information.
	float mEmitCounter;
	float mEmissionRate;
	
	// time since start of system in seconds.
	float mElapsed;
	
	// true if the particle system is active
	bool mActive;
	
	ParticlePositionType mParticlePositionType;
	
	enum IndexModeType{
		INDEX_MODE_SEQUENTIAL = 0,
		INDEX_MODE_RANDOM,
	};
	// Index Mode properties
	IndexModeType mIndexMode;
	u32			mStartIndex;
	u32			mEndIndex;
	
	u32			mDeckTilesX;
	u32			mDeckTilesY;
	float		mDeckCellWidth;
	float		mDeckCellHeight;
	float		mDeckOffsetX;
	float		mDeckOffsetY;
	float		mDeckTileWidth;
	float		mDeckTileHeight;
	
	
	static int		_getAngle							( lua_State* L );
	static int		_setAngle							( lua_State* L );
	static int		_getAngleVariance					( lua_State* L );
	static int		_setAngleVariance					( lua_State* L );
	static int		_getBlendMode						( lua_State* L );
	static int		_setBlendMode						( lua_State* L );
	static int		_getDuration						( lua_State* L );
	static int		_setDuration						( lua_State* L );
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
	static int		_getStartRadius						( lua_State* L );
	static int		_setStartRadius						( lua_State* L );
	static int		_getStartRadiusVariance				( lua_State* L );
	static int		_setStartRadiusVariance				( lua_State* L );
	static int		_getEndRadius						( lua_State* L );
	static int		_setEndRadius						( lua_State* L );
	static int		_getEndRadiusVariance				( lua_State* L );
	static int		_setEndRadiusVariance				( lua_State* L );
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
	
	static int		_getGravityVariance					( lua_State* L );
	static int		_setGravityVariance					( lua_State* L );
	static int		_getRotationalAcceleration			( lua_State* L );
	static int		_setRotationalAcceleration			( lua_State* L );
	static int		_getRotationalAccelerationVariance	( lua_State* L );
	static int		_setRotationalAccelerationVariance	( lua_State* L );
	
	static int		_getTextureName						( lua_State* L );
	
	static int		_getTotalParticles					( lua_State* L );
	static int		_setTotalParticles					( lua_State* L );
	static int		_getParticleCount					( lua_State* L );
	static int		_setEmissionRate					( lua_State* L );
	static int		_getParticlePositionType			( lua_State* L );
	static int		_setParticlePositionType			( lua_State* L );
	
	static int		_getIndexMode						( lua_State* L );
	static int		_setIndexMode						( lua_State* L );
	static int		_getStartIndex						( lua_State* L );
	static int		_setStartIndex						( lua_State* L );
	static int		_getEndIndex						( lua_State* L );
	static int		_setEndIndex						( lua_State* L );
	static int		_getDeckTileCount					( lua_State* L );
	static int		_getDeckTileDimensions				( lua_State* L );
	static int		_getDeckCellDimensions				( lua_State* L );
	static int		_getDeckTileOffset					( lua_State* L );
	
	static int		_flipY								( lua_State* L );
	static int		_initializeProperties				( lua_State* L );
	static int		_load								( lua_State* L );
	static int		_startSystem						( lua_State* L );
	static int		_stopSystem       					( lua_State* L );
	static int		_reset								( lua_State* L );
	
	
	bool			AddParticle							();
	void			InitParticle						( MOAICCParticle *particle );
	void			InitializeEmitter					();
	bool			IsFull								();
	void			OnUpdate							( float step );
	void			ParseXML							( cc8* filename, TiXmlNode* node );
	
public:
	DECL_LUA_FACTORY ( MOAICCParticleSystem )
	
	
	void			Draw					( int subPrimID );
	void			FlipY					();
	bool			IsDone					();
	
					MOAICCParticleSystem	();
					~MOAICCParticleSystem	();
	
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	
	void			SetTotalParticles		( u32 numberOfParticles );
	void			SetVisibility			( bool visible );
	void			ResetSystem				( bool activate );
	void			StartSystem				();
	void			StopSystem				();
	
};


#endif /* defined(MOAICCPARTICLESYSTEM_H) */
