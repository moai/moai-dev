//
//  MOAICCParticleSystem.cpp
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGfxDevice.h>


#include <moaicore/MOAICCParticleSystem.h>


int MOAICCParticleSystem::_getAngle( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mAngle );
	
	return 1;
}

int MOAICCParticleSystem::_setAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mAngle = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getAngleVariance( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mAngleVariance );
	return 1;
}

int MOAICCParticleSystem::_setAngleVariance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mAngleVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int	MOAICCParticleSystem::_getBlendMode( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mBlendFuncSrc );
	lua_pushnumber ( state, self->mBlendFuncDst );
	return 2;
}

int MOAICCParticleSystem::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNN" )
	self->mBlendFuncSrc = state.GetValue < u32 >( 2, 0 );
	self->mBlendFuncDst = state.GetValue < u32 >( 3, 0 );
	return 0;
}

int MOAICCParticleSystem::_getDuration( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mDuration );
	return 1;
}

int MOAICCParticleSystem::_setDuration ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mDuration = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getEmitterType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mEmitterType );
	return 1;
}

int MOAICCParticleSystem::_setEmitterType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mEmitterType = (EmitterType)state.GetValue < u32 >( 2, 0 );
	return 0;
}

int	MOAICCParticleSystem::_getFinishColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mFinishColor[0] );
	lua_pushnumber ( state, self->mFinishColor[1] );
	lua_pushnumber ( state, self->mFinishColor[2] );
	lua_pushnumber ( state, self->mFinishColor[3] );
	return 4;
}

int MOAICCParticleSystem::_setFinishColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNNN" )
	self->mFinishColor[0] = state.GetValue < float >( 2, 0 );
	self->mFinishColor[1] = state.GetValue < float >( 3, 0 );
	self->mFinishColor[2] = state.GetValue < float >( 4, 0 );
	self->mFinishColor[3] = state.GetValue < float >( 5, 1 );
	return 0;
}

int	MOAICCParticleSystem::_getFinishColorVariance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mFinishColorVariance[0] );
	lua_pushnumber ( state, self->mFinishColorVariance[1] );
	lua_pushnumber ( state, self->mFinishColorVariance[2] );
	lua_pushnumber ( state, self->mFinishColorVariance[3] );
	return 4;
}

int MOAICCParticleSystem::_setFinishColorVariance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNNN" )
	self->mFinishColorVariance[0] = state.GetValue < float >( 2, 0 );
	self->mFinishColorVariance[1] = state.GetValue < float >( 3, 0 );
	self->mFinishColorVariance[2] = state.GetValue < float >( 4, 0 );
	self->mFinishColorVariance[3] = state.GetValue < float >( 5, 0 );
	return 0;
}

int MOAICCParticleSystem::_getFinishParticleSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mFinishSize );
	return 1;
}

int MOAICCParticleSystem::_setFinishParticleSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mFinishSize = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getFinishParticleSizeVariance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mFinishSizeVariance );
	return 1;
}

int MOAICCParticleSystem::_setFinishParticleSizeVariance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mFinishSizeVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mEmissionRate );
	return 1;
}

int MOAICCParticleSystem::_getGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mGravity[0] );
	lua_pushnumber ( state, self->mGravity[1] );
	return 2;
}

int MOAICCParticleSystem::_setGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNN" )
	self->mGravity[0] = state.GetValue < float >( 2, 0 );
	self->mGravity[1] = state.GetValue < float >( 3, 0 );
	return 0;
}

int MOAICCParticleSystem::_getLifespan ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mLifespanTerm[0] );
	lua_pushnumber ( state, self->mLifespanTerm[1] );
	return 2;
}

int MOAICCParticleSystem::_getStartRadius ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mMaxRadius );
	return 1;
}

int MOAICCParticleSystem::_setStartRadius ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mMaxRadius = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getStartRadiusVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mMaxRadiusVariance );
	return 1;
}

int MOAICCParticleSystem::_setStartRadiusVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mMaxRadiusVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getEndRadius ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mMinRadius );
	return 1;
}

int MOAICCParticleSystem::_setEndRadius ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mMinRadius = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getEndRadiusVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mMinRadiusVariance );
	return 1;
}

int MOAICCParticleSystem::_setEndRadiusVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mMinRadiusVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getParticleLifespan ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mLifespan );
	return 1;
}

int MOAICCParticleSystem::_setParticleLifespan ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mLifespan = state.GetValue < float >( 2, 0 );
	
	self->InitializeEmitter();
	
	return 0;
}

int MOAICCParticleSystem::_getParticleLifespanVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mLifespanVariance );
	return 1;
}

int MOAICCParticleSystem::_setParticleLifespanVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mLifespanVariance = state.GetValue < float >( 2, 0 );
	
	self->InitializeEmitter();
	
	return 0;
}

int MOAICCParticleSystem::_getRadialAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRadialAcceleration );
	return 1;
}

int MOAICCParticleSystem::_setRadialAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRadialAcceleration = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRadialAccelVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRadialAccelVariance );
	return 1;
}

int MOAICCParticleSystem::_setRadialAccelVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRadialAccelVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRect ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, -self->mSourcePosVariance[0] );
	lua_pushnumber ( state, -self->mSourcePosVariance[1] );
	lua_pushnumber ( state, self->mSourcePosVariance[0] );
	lua_pushnumber ( state, self->mSourcePosVariance[1] );
	return 4;
}

int MOAICCParticleSystem::_getRotatePerSecond ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotPerSecond );
	return 1;
}

int MOAICCParticleSystem::_setRotatePerSecond ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotPerSecond = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotatePerSecondVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotPerSecondVariance );
	return 1;
}

int MOAICCParticleSystem::_setRotatePerSecondVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotPerSecondVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationEnd ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotEnd );
	return 1;
}

int MOAICCParticleSystem::_setRotationEnd ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotEnd = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationEndVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotEndVariance );
	return 1;
}

int MOAICCParticleSystem::_setRotationEndVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotEndVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationStart ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotStart );
	return 1;
}

int MOAICCParticleSystem::_setRotationStart ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotStart = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationStartVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotStartVariance );
	return 1;
}

int MOAICCParticleSystem::_setRotationStartVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotStartVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getSourcePosition ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mSourcePos[0] );
	lua_pushnumber ( state, self->mSourcePos[1] );
	return 2;
}

int MOAICCParticleSystem::_setSourcePosition ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNN" )
	self->mSourcePos[0] = state.GetValue < float >( 2, 0 );
	self->mSourcePos[1] = state.GetValue < float >( 3, 0 );
	return 0;
}

int MOAICCParticleSystem::_getSourcePositionVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mSourcePosVariance[0] );
	lua_pushnumber ( state, self->mSourcePosVariance[1] );
	return 2;
}

int MOAICCParticleSystem::_setSourcePositionVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNN" )
	self->mSourcePosVariance[0] = state.GetValue < float >( 2, 0 );
	self->mSourcePosVariance[1] = state.GetValue < float >( 3, 0 );
	return 0;
}


int MOAICCParticleSystem::_getSpeed ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mSpeed );
	return 1;
}

int MOAICCParticleSystem::_setSpeed ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mSpeed = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getSpeedVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mSpeedVariance );
	return 1;
}

int MOAICCParticleSystem::_setSpeedVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mSpeedVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getStartColor ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mStartColor[0] );
	lua_pushnumber ( state, self->mStartColor[1] );
	lua_pushnumber ( state, self->mStartColor[2] );
	lua_pushnumber ( state, self->mStartColor[3] );
	return 4;
}

int MOAICCParticleSystem::_setStartColor ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNNNN" )
	self->mStartColor[0] = state.GetValue < float >( 2, 0 );
	self->mStartColor[1] = state.GetValue < float >( 3, 0 );
	self->mStartColor[2] = state.GetValue < float >( 4, 0 );
	self->mStartColor[3] = state.GetValue < float >( 5, 0 );
	return 0;
}

int MOAICCParticleSystem::_getStartColorVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mStartColorVariance[0] );
	lua_pushnumber ( state, self->mStartColorVariance[1] );
	lua_pushnumber ( state, self->mStartColorVariance[2] );
	lua_pushnumber ( state, self->mStartColorVariance[3] );
	return 4;
}

int MOAICCParticleSystem::_setStartColorVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNNNN" )
	self->mStartColorVariance[0] = state.GetValue < float >( 2, 0 );
	self->mStartColorVariance[1] = state.GetValue < float >( 3, 0 );
	self->mStartColorVariance[2] = state.GetValue < float >( 4, 0 );
	self->mStartColorVariance[3] = state.GetValue < float >( 5, 0 );
	return 0;
}

int MOAICCParticleSystem::_getStartParticleSize ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mStartSize );
	return 1;
}

int MOAICCParticleSystem::_setStartParticleSize ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mStartSize = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getStartParticleSizeVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mStartSizeVariance );
	return 1;
}

int MOAICCParticleSystem::_setStartParticleSizeVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mStartSizeVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getTangentalAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mTangentialAcceleration );
	return 1;
}

int MOAICCParticleSystem::_setTangentalAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mTangentialAcceleration = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getTangentalAccelerationVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mTangentialAccelVariance );
	return 1;
}

int MOAICCParticleSystem::_setTangentalAccelerationVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mTangentialAccelVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getGravityVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mGravityVariance[0] );
	lua_pushnumber ( state, self->mGravityVariance[1] );
	return 2;
}

int MOAICCParticleSystem::_setGravityVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UNN" )
	self->mGravityVariance[0] = state.GetValue < float >( 2, 0 );
	self->mGravityVariance[1] = state.GetValue < float >( 3, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationalAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotationalAcceleration );
	return 1;
}

int MOAICCParticleSystem::_setRotationalAcceleration ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotationalAcceleration = state.GetValue < float >( 2, 0 );
	return 0;
}

int MOAICCParticleSystem::_getRotationalAccelerationVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber ( state, self->mRotationalAccelVariance );
	return 1;
}

int MOAICCParticleSystem::_setRotationalAccelerationVariance ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	self->mRotationalAccelVariance = state.GetValue < float >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTextureName
	@text	Return the texture name associated with plugin.
 
	@in		MOAIParticlePlugin self
	@out	string textureName
 */

int MOAICCParticleSystem::_getTextureName ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	
	lua_pushstring( state, self->mTextureName);
	
	return 1;
}



//----------------------------------------------------------------//
int MOAICCParticleSystem::_getTotalParticles( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mTotalParticles );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAICCParticleSystem::_setTotalParticles( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	u32 total = state.GetValue < u32 >( 2, 1 );
	
	self->SetTotalParticles( total );
	
	
	return 0;
}

//----------------------------------------------------------------//
int MOAICCParticleSystem::_getParticleCount( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mParticleCount );
	return 1;
}


int MOAICCParticleSystem::_setEmissionRate( lua_State* L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	self->mEmissionRate = state.GetValue < float >(2, 1.0);
	
	return 0;
}


int MOAICCParticleSystem::_getParticlePositionType( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mParticlePositionType );
	return 1;
}

int MOAICCParticleSystem::_setParticlePositionType( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	ParticlePositionType type = ( ParticlePositionType )state.GetValue < u32 >( 2, PARTICLE_POSITION_RELATIVE );
	self->mParticlePositionType = type;
	return 0;
}

//----------------------------------------------------------------//
int MOAICCParticleSystem::_getIndexMode( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mIndexMode );
	return 1;
}

int MOAICCParticleSystem::_setIndexMode ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "UN" )
	IndexModeType mode = ( IndexModeType ) state.GetValue < u32 > ( 2, INDEX_MODE_SEQUENTIAL );
	self->mIndexMode = mode;
	return 0;
}

//----------------------------------------------------------------//
int MOAICCParticleSystem::_getStartIndex( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mStartIndex );
	return 1;
}

int MOAICCParticleSystem::_setStartIndex( lua_State* L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	self->mStartIndex = state.GetValue < u32 >(2, 1.0);
	return 0;
}

//----------------------------------------------------------------//
int MOAICCParticleSystem::_getEndIndex( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mEndIndex );
	return 1;
}

int MOAICCParticleSystem::_setEndIndex( lua_State* L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	self->mEndIndex = state.GetValue < u32 >(2, 1.0);
	return 0;
}
//----------------------------------------------------------------//
/** @name	getDeckTileCount
	@text
 
	@in     MOAICCParticleSystem	self
	@out	number					x
	@out    number					y
 
 */
int MOAICCParticleSystem::_getDeckTileCount( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mDeckTilesX);
	lua_pushnumber( state, self->mDeckTilesY);
	return 2;
}
//----------------------------------------------------------------//
/** @name	getDeckTileDimensions
	@text
 
	@in     MOAICCParticleSystem	self
	@out	number					width
	@out    number					height
 
 */
int MOAICCParticleSystem::_getDeckTileDimensions( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mDeckTileWidth);
	lua_pushnumber( state, self->mDeckTileHeight);
	return 2;
}

//----------------------------------------------------------------//
/** @name	getDeckCellDimensions
	@text
 
	@in     MOAICCParticleSystem	self
	@out	number					width
	@out    number					height
 
 */
int MOAICCParticleSystem::_getDeckCellDimensions( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mDeckCellWidth);
	lua_pushnumber( state, self->mDeckCellHeight);
	return 2;
}

//----------------------------------------------------------------//
/** @name	getDeckTileOffset
	@text
 
	@in     MOAICCParticleSystem	self
	@out	number					x
	@out    number					y
 
 */
int MOAICCParticleSystem::_getDeckTileOffset ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	lua_pushnumber( state, self->mDeckOffsetX);
	lua_pushnumber( state, self->mDeckOffsetY);
	return 2;
}

//----------------------------------------------------------------//
/**	@name	flipY
	@text	Adjust the properties of the system so they mirror around the Y axis.
 
	@in		MOAICCParticleSystem	self
	@out	nil
 */
int MOAICCParticleSystem::_flipY( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAICCParticleSystem, "U" )
	
	self->FlipY();
	
	return 0;
}

//----------------------------------------------------------------//

int MOAICCParticleSystem::_initializeProperties( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	
	self->InitializeEmitter();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Initialize particle system properties with an XML file
 
	@in		MOAICCParticleSystem	self
	@in		String					file to load
	@out	nil
 */

int MOAICCParticleSystem::_load( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "US" )
	cc8* xml = lua_tostring( state, 2);
	
	if ( MOAILogMessages::CheckFileExists ( xml, L )) {
		TiXmlDocument doc;
		doc.LoadFile ( xml );
		self->ParseXML ( xml, doc.RootElement ());
		
	}
	
	return 0;
}

int MOAICCParticleSystem::_startSystem ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->StartSystem();
	return 0;
}

int MOAICCParticleSystem::_stopSystem ( lua_State *L ) {
	
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->StopSystem();
	
	return 0;
}

int MOAICCParticleSystem::_reset ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	bool activate = state.GetValue < bool > (2, true);
	self->ResetSystem( activate );
	return 0;
}

//----------------------------------------------------------------//
//----------------------------------------------------------------//

bool MOAICCParticleSystem::AddParticle () {
	if ( this->IsFull() ) {
		return false;
	}
	
	MOAICCParticle * particle = &(this->mParticles[this->mParticleCount]);
	this->InitParticle( particle );
	this->mParticleCount++;
	
	return true;
}

void MOAICCParticleSystem::Draw	( int subPrimID ) {
	UNUSED(subPrimID);
	
	if ( !this->mDeck ) {
		return;
	}
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( this->mUVTransform ) {
		USAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		gfxDevice.SetUVTransform ( uvMtx );
	}
	else {
		gfxDevice.SetUVTransform ();
	}
	
	this->LoadGfxState();
	
	USAffine3D drawingMtx;
	USAffine3D spriteMtx;
	
	u32 maxParticles = this->mTotalParticles;
	u32 total = this->mParticleCount;
	if (total > maxParticles) {
		total = maxParticles;
	}
	
	
	for ( u32 i = 0 ; i < total; ++i ) {
		u32 idx = i;
		MOAICCParticle *particle = &(this->mParticles[ idx ]);
		
		// set pen color
		gfxDevice.SetPenColor(particle->mColor[0], particle->mColor[1], particle->mColor[2], particle->mColor[3]);
		
		// set transforms
		spriteMtx.ScRoTr(particle->mParticleSize, particle->mParticleSize, 1.0f,
						 0.0f, 0.0f, particle->mParticleRotation,
						 particle->mCurrentPosition.mX, particle->mCurrentPosition.mY, 0.0f);
		
		drawingMtx = this->GetLocalToWorldMtx();
		drawingMtx.Prepend( spriteMtx );
		
		gfxDevice.SetVertexTransform( MOAIGfxDevice::VTX_WORLD_TRANSFORM, drawingMtx );
		
		this->mDeck->Draw ( this->mIndex + (u32) particle->mDeckIndex, this->mRemapper);
		
	}
}

void MOAICCParticleSystem::FlipY ( ) {
	this->mAngle = 360.0f - this->mAngle;
	
	// invert y component of gravity
	this->mGravity[1] = -this->mGravity[1];
	
	// invert angular velocity
	this->mRotPerSecond = -this->mRotPerSecond;
	
	// invert angular acceleration
	this->mRotationalAcceleration = -this->mRotationalAcceleration;
	
	// invert tangential acceleration
	this->mTangentialAcceleration = -this->mTangentialAcceleration;
}

void MOAICCParticleSystem::InitParticle ( MOAICCParticle *particle ) {
	
	// timeToLive
	particle->mTimeToLive = USFloat::Rand(this->mLifespanTerm[0], this->mLifespanTerm[1]);
	
	
	// position
	particle->mStartPosition.mX = this->mSourcePos[0] + this->mSourcePosVariance[0] * USFloat::Rand(-1.0f, 1.0f);
	particle->mStartPosition.mY = this->mSourcePos[1] + this->mSourcePosVariance[1] * USFloat::Rand(-1.0f, 1.0f);
	
	// color
	float start;
	float end;
	
	for (int i = 0; i < 4; ++i) {
		start = USFloat::Clamp(this->mStartColor[i] + this->mStartColorVariance[i] * USFloat::Rand(-1.0f, 1.0f), 0.0f, 1.0f);
		
		end = USFloat::Clamp(this->mFinishColor[i] + this->mFinishColorVariance[i] * USFloat::Rand(-1.0f, 1.0f), 0.0f, 1.0f);
		
		particle->mColor[i] = start;
		
		particle->mDeltaColor[i] = (end - start) / particle->mTimeToLive;
	}
	
	// partice size
	start = this->mStartSize + this->mStartSizeVariance * USFloat::Rand(-1.0f, 1.0f);
	start = MAX(0, start);
	
	particle->mParticleSize = start;
	
	end = this->mFinishSize + this->mFinishSizeVariance * USFloat::Rand(-1.0f, 1.0f);
	particle->mDeltaParticleSize = (end - start) / particle->mTimeToLive;
	
	
	// particle rotation
	start = this->mRotStart + this->mRotStartVariance * USFloat::Rand(-1.0f, 1.0f);
	end = this->mRotEnd + this->mRotEndVariance * USFloat::Rand(-1.0f, 1.0f);
	
	particle->mParticleRotation = start;
	particle->mDeltaParticleRotation = (end - start) / particle->mTimeToLive;
	
	// position
	
	// relative particle position
	USVec3D loc;
	if (this->mParticlePositionType == PARTICLE_POSITION_RELATIVE) {
		loc = this->GetLoc();
		particle->mStartPosition.Init(loc.mX, loc.mY);
	}
	// free particle position
	else if (this->mParticlePositionType == PARTICLE_POSITION_FREE) {
		
		loc.Init(0.0f, 0.0f, 0.0f);
		USAffine3D modelToWorld = this->GetLocalToWorldMtx ();
		modelToWorld.Transform(loc);
		particle->mStartPosition.Init(loc.mX, loc.mY);
	}
	
	// set current position
	particle->mCurrentPosition.Init(particle->mStartPosition);
	
	// direction
	float a = D2R * (this->mAngle + this->mAngleVariance * USFloat::Rand(-1.0f, 1.0f));
	
	
	// gravity mode
	if ( this->mEmitterType == EMITTER_GRAVITY ) {
		USVec2D v;
		v.Init(Cos(a), Sin(a));
		
		float s = this->mSpeed + this->mSpeedVariance * USFloat::Rand(-1.0f, 1.0f);
		v.Scale( s );
		
		// direction
		particle->mDirection.Init(v);
		
		// gravity variance
		particle->mGravity.mX = this->mGravity[0] + this->mGravityVariance[0] * USFloat::Rand(-1.0f, 1.0f);
		particle->mGravity.mY = this->mGravity[1] + this->mGravityVariance[1] * USFloat::Rand(-1.0f, 1.0f);
		
		// radial accel
		particle->mRadialAcceleration = this->mRadialAcceleration + this->mRadialAccelVariance * USFloat::Rand(-1.0f, 1.0f);
		
		// tangential accel
		particle->mTangentialAcceleration = this->mTangentialAcceleration + this->mTangentialAccelVariance * USFloat::Rand(-1.0f, 1.0f);
		
	}
	// radial mode
	else {
		float startRadius = this->mMinRadius + this->mMinRadiusVariance *  USFloat::Rand(-1.0f, 1.0f);
		float endRadius = this->mMaxRadius + this->mMaxRadiusVariance *  USFloat::Rand(-1.0f, 1.0f);
		
		particle->mRadius = startRadius;
		
		particle->mDeltaRadius = (endRadius - startRadius) / particle->mTimeToLive;
		
		particle->mAngle = a;
		particle->mDegreesPerSecond = D2R * (this->mRotPerSecond + this->mRotPerSecondVariance * USFloat::Rand(-1.0f, 1.0f));
		
		// rotational acceleration
		particle->mRotationalAcceleration = D2R * (this->mRotationalAcceleration + this->mRotationalAccelVariance * USFloat::Rand(-1.0f, 1.0f));
		
	}
	
	// deck index
	if ( this->mIndexMode == INDEX_MODE_RANDOM) { // random
		u32 endIdx;
		u32 startIdx;
		if (this->mEndIndex > this->mStartIndex) {
			endIdx = this->mEndIndex;
			startIdx = this->mStartIndex;
		}
		else{
			endIdx = this->mStartIndex;
			startIdx = this->mEndIndex;
		}
		
		particle->mDeckIndex = (u32) Rand(startIdx, endIdx);
	}
	else { // sequential
		u32 span = (this->mEndIndex - this->mStartIndex) + 1;
		if (this->mEndIndex > this->mStartIndex) {
			particle->mDeckIndex = this->mStartIndex + (this->mSeqIndex++ % span);
		}
		else{
			particle->mDeckIndex = this->mStartIndex;
		}
	}
	
	
}

void MOAICCParticleSystem::InitializeEmitter () {
	this->mEmissionRate = 1.0f / (this->mTotalParticles / this->mLifespan);
	
	float minLifespan = this->mLifespan - this->mLifespanVariance;
	if ( minLifespan < 0.0f ) {
		minLifespan = 0.0f;
	}
	
	this->mLifespanTerm[0] = minLifespan;
	
	float maxLifespan = this->mLifespan + this->mLifespanVariance;
	this->mLifespanTerm[1] = maxLifespan;
}

bool MOAICCParticleSystem::IsDone () {
	return false;
}


bool MOAICCParticleSystem::IsFull () {
	return (this->mParticleCount == this->mTotalParticles);
}


MOAICCParticleSystem::MOAICCParticleSystem() :
	mParticles( NULL ),
	mParticleCount( 0 ),
	mAllocatedParticles( 0 ),
	mTotalParticles( 0 ),
	mEmitterType( EMITTER_GRAVITY ),
	mLifespan( 0.0f ),
	mLifespanVariance( 0.0f ),
	mAngle( 0.0f ),
	mAngleVariance( 0.0f ),
	mStartSize( 16.0f ),
	mStartSizeVariance( 0.0f ),
	mFinishSize( 0.0f ),
	mFinishSizeVariance( 0.0f ),
	mMaxRadius( 0.0f ),
	mMaxRadiusVariance( 0.0f ),
	mMinRadius( 0.0f ),
	mMinRadiusVariance( 0.0f ),
	mRadialAcceleration( 0.0f ),
	mRadialAccelVariance( 0.0f ),
	mTangentialAcceleration( 0.0f ),
	mTangentialAccelVariance( 0.0f ),
	mRotStart( 0.0f ),
	mRotStartVariance( 0.0f ),
	mRotEnd( 0.0f ),
	mRotEndVariance( 0.0f ),
	mSpeed( 0.0f ),
	mSpeedVariance( 0.0f ),
	mRotPerSecond( 0.0f ),
	mRotPerSecondVariance( 0.0f ),
	mRotationalAcceleration( 0.0f ),
	mRotationalAccelVariance( 0.0f ),
	mDuration( -1.0f ),
	mBlendFuncSrc( GL_ONE ),
	mBlendFuncDst( GL_ONE_MINUS_SRC_ALPHA ),
	mEmitCounter( 0.0f ),
	mEmissionRate( 0.0f ),
	mElapsed( 0.0f ),
	mActive( false ),
	mParticlePositionType( PARTICLE_POSITION_GROUPED ),
	mIndexMode( INDEX_MODE_SEQUENTIAL ),
	mStartIndex( 0 ),
	mEndIndex( 0 ),
	mDeckTilesX( 1 ),
	mDeckTilesY( 1 ),
	mDeckCellWidth( 1.0 ),
	mDeckCellHeight( 1.0 ),
	mDeckOffsetX( 0.0 ),
	mDeckOffsetY( 0.0 ),
	mDeckTileWidth( 1.0 ),
	mDeckTileHeight( 1.0 ),
	mSeqIndex( 0 )
{
	int i = 0;
	for ( ; i < 2;  ++i) {
		this->mLifespanTerm[i] = 0.0f;
		this->mGravity[i] = 0.0f;
		this->mGravityVariance[i] = 0.0f;
		this->mSourcePos[i] = 0.0f;
		this->mSourcePosVariance[i] = 0.0f;
	}
	
	for (i = 0; i < 4; ++i) {
		this->mStartColor[i] = 1.0f;
		this->mStartColorVariance[i] = 0.0f;
		this->mFinishColor[i] = 0.0f;
		this->mFinishColorVariance[i] = 0.0f;
	}
	
	RTTI_BEGIN
		RTTI_EXTEND( MOAIProp )
		RTTI_EXTEND( MOAIAction )
	RTTI_END
}

MOAICCParticleSystem::~MOAICCParticleSystem () {
	// clean up the allocated array
	if (this->mParticles) {
		free (this->mParticles);
		this->mParticles = NULL;
	}
}

void MOAICCParticleSystem::OnUpdate ( float step ) {
	
	if (this->mActive && this->mEmissionRate) {
		float rate = this->mEmissionRate;
		
		if (this->mParticleCount < this->mTotalParticles) {
			this->mEmitCounter += step;
		}
		while (this->mParticleCount < this->mTotalParticles && this->mEmitCounter > rate){
			this->AddParticle();
			this->mEmitCounter -= rate;
		}
		
		this->mElapsed += step;
		if (this->mDuration != -1 && this->mDuration < this->mElapsed) {
			this->StopSystem();
		}
		
		
		
	}
	
	if (this->mFlags & FLAGS_VISIBLE) {
		for (int i = 0; i < (int) this->mParticleCount; ) {
			MOAICCParticle *p = &(this->mParticles[i]);
			
			// life
			p->mTimeToLive -= step;
			if ( p->mTimeToLive > 0) {
				// Gravity mode
				if (this->mEmitterType == EMITTER_GRAVITY) {
					USVec2D radialVector(0.0f, 0.0f);
					// radial acceleration
					
					if (p->mCurrentPosition.mX || p->mCurrentPosition.mY) {
						// normalize the vector
						radialVector.mX = p->mCurrentPosition.mX;
						radialVector.mY = p->mCurrentPosition.mY;
						
						radialVector.Norm();
					}
					USVec2D tangentialVector;
					tangentialVector.Init(radialVector);
					
					radialVector.Scale(p->mRadialAcceleration);
					
					// tangential acceleration
					float newY = tangentialVector.mX;
					tangentialVector.mX = -tangentialVector.mY;
					tangentialVector.mY = newY;
					tangentialVector.Scale(p->mTangentialAcceleration);
					
					// (gravity + radial + tangential) * dt
					USVec2D tmp;
					tmp.Init(p->mGravity);
					tmp.Add(radialVector);
					tmp.Add(tangentialVector);
					tmp.Scale(step);
					
					p->mDirection.Add(tmp);
					tmp.Init(p->mDirection);
					tmp.Scale(step);
					p->mCurrentPosition.Add(tmp);
					
				}
				// Radial mode
				else {
					p->mDegreesPerSecond += p->mRotationalAcceleration * step;
					p->mAngle += p->mDegreesPerSecond * step;
					p->mRadius += p->mDeltaRadius * step;
					
					p->mCurrentPosition.mX = - Cos(p->mAngle) * p->mRadius;
					p->mCurrentPosition.mY = - Sin(p->mAngle) * p->mRadius;
				}
				// color
				for (int j = 0; j < 4; ++j) {
					p->mColor[j] += (p->mDeltaColor[j] * step);
				}
				
				// particle size
				p->mParticleSize += (p->mDeltaParticleSize * step);
				p->mParticleSize = MAX( 0, p->mParticleSize );
				
				// particle rotation
				p->mParticleRotation += (p->mDeltaParticleRotation * step);
				
				++i;
				
			}
			else{
				// life <= 0
				if ( i != (int)this->mParticleCount - 1) {
					this->mParticles[i] = this->mParticles[this->mParticleCount - 1];
				}
				this->mParticleCount--;
				
				if ( this->mParticleCount == 0 ){
					
				}
				
			}
		}
	}
	
}

void MOAICCParticleSystem::ParseXML( cc8 *filename, TiXmlNode *node ) {
	if (!node) {
		return;
	}
	
	this->mParticlePath = filename;
	
	STLString absFilePath = USFileSys::GetAbsoluteFilePath ( filename );
	STLString absDirPath = USFileSys::TruncateFilename ( absFilePath );
	
	TiXmlElement* element = node->ToElement();
	if (element && (strcmp(element->Value(), "particleEmitterConfig") == 0)) {
		
		TiXmlElement* childElement = node->FirstChildElement ();
		
		for ( ; childElement; childElement = childElement->NextSiblingElement() ) {
			STLString text = childElement->Value ();
			TiXmlAttribute* attribute = childElement->FirstAttribute ();
			int i = 0;
			if (!attribute || !text || text == "") {
				continue;
			}
			
			if (text == "angle") {
				this->mAngle = (float)atof(attribute->Value());
			}
			else if (text == "angleVariance") {
				this->mAngleVariance = (float)atof(attribute->Value());
			}
			else if (text == "blendFuncSource") {
				this->mBlendFuncSrc = atoi(attribute->Value());
			}
			else if (text == "blendFuncDestination") {
				this->mBlendFuncDst = atoi(attribute->Value());
			}
			else if (text == "deckCellDimensions") {
				this->mDeckCellHeight = (float) atof(attribute->Value());
				attribute = attribute->Next ();
				if (attribute) {
					this->mDeckCellWidth = (float) atof(attribute->Value());
				}
			}
			else if (text == "deckOffset") {
				this->mDeckOffsetX = (float) atof(attribute->Value());
				attribute = attribute->Next ();
				if (attribute) {
					this->mDeckOffsetY = (float) atof(attribute->Value());
				}
			}
			else if (text == "deckTileCount") {
				this->mDeckTilesX = (u32) atoi(attribute->Value());
				attribute = attribute->Next ();
				if (attribute) {
					this->mDeckTilesY = (u32) atoi(attribute->Value());
				}
			}
			else if (text == "deckTileDimensions") {
				this->mDeckTileHeight = (float) atof(attribute->Value());
				attribute = attribute->Next ();
				if (attribute) {
					this->mDeckTileWidth = (float) atof(attribute->Value());
				}
			}
			else if (text == "duration") {
				this->mDuration = (float)atof(attribute->Value());
			}
			else if (text == "emitterType") {
				this->mEmitterType = (EmitterType)atoi(attribute->Value());
			}
			else if (text == "endIndex") {
				this->mEndIndex = (u32) atoi(attribute->Value());
			}
			else if (text == "finishColor") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mFinishColor[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "finishColorVariance") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mFinishColorVariance[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "finishParticleSize") {
				this->mFinishSize = (float)atof(attribute->Value());
			}
			else if (text == "finishParticleSizeVariance") {
				this->mFinishSizeVariance = (float)atof(attribute->Value());
			}
			else if (text == "gravity") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mGravity[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "gravityVariance") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mGravityVariance[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "indexMode") {
				this->mIndexMode = (IndexModeType) atoi(attribute->Value());
			}
			else if (text == "maxParticles") {
				u32 numParticles = atoi(attribute->Value());
				this->SetTotalParticles(numParticles);
			}
			else if (text == "maxRadius") {
				this->mMaxRadius = (float)atof(attribute->Value());
			}
			else if( text == "maxRadiusVariance") {
				this->mMaxRadiusVariance = (float)atof(attribute->Value());
			}
			else if (text == "minRadius") {
				this->mMinRadius = (float)atof(attribute->Value());
			}
			else if (text == "minRadiusVariance") {
				this->mMinRadiusVariance = (float)atof(attribute->Value());
			}
			else if (text == "particleLifeSpan") {
				this->mLifespan = (float)atof(attribute->Value());
			}
			else if (text == "particleLifespanVariance") {
				this->mLifespanVariance = (float)atof(attribute->Value());
			}
			else if (text == "radialAcceleration") {
				this->mRadialAcceleration = (float)atof(attribute->Value());
			}
			else if (text == "radialAccelVariance") {
				this->mRadialAccelVariance = (float)atof(attribute->Value());
			}
			else if (text == "rotatePerSecond") {
				this->mRotPerSecond = (float)atof(attribute->Value());
			}
			else if (text == "rotatePerSecondVariance") {
				this->mRotPerSecondVariance = (float)atof(attribute->Value());
			}
			else if (text == "rotationEnd") {
				this->mRotEnd = (float)atof(attribute->Value());
			}
			else if (text == "rotationEndVariance") {
				this->mRotEndVariance = (float)atof(attribute->Value());
			}
			else if (text == "rotationStart") {
				this->mRotStart = (float)atof(attribute->Value());
			}
			else if (text == "rotationStartVariance") {
				this->mRotStartVariance = (float)atof(attribute->Value());
			}
			else if (text == "rotationalAcceleration") {
				this->mRotationalAcceleration = (float)atof(attribute->Value());
			}
			else if (text == "rotationalAccelVariance") {
				this->mRotationalAccelVariance = (float)atof(attribute->Value());
			}
			else if (text == "sourcePosition") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mSourcePos[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "sourcePositionVariance") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mSourcePosVariance[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "speed") {
				this->mSpeed = (float)atof(attribute->Value());
			}
			else if (text == "speedVariance") {
				this->mSpeedVariance = (float)atof(attribute->Value());
			}
			else if (text == "startColor") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mStartColor[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "startColorVariance") {
				for ( ; attribute; attribute = attribute->Next (), i++) {
					this->mStartColorVariance[i] = (float)atof(attribute->Value());
				}
			}
			else if (text == "startIndex") {
				this->mStartIndex = (u32) atoi(attribute->Value());
			}
			else if (text == "startParticleSize") {
				this->mStartSize = (float)atof(attribute->Value());
			}
			else if (text == "startParticleSizeVariance") {
				this->mStartSizeVariance = (float)atof(attribute->Value());
			}
			else if (text == "tangentialAcceleration") {
				this->mTangentialAcceleration = (float)atof(attribute->Value());
			}
			else if (text == "tangentialAccelVariance") {
				this->mTangentialAccelVariance = (float)atof(attribute->Value());
			}
			else if (text == "texture") {
				this->mTextureName = absDirPath;
				this->mTextureName.append ( attribute->Value ());
			}
			
		}
		
		this->InitializeEmitter();
		
	}
	
}

void MOAICCParticleSystem::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass( state );
	
	state.SetField(-1, "EMITTER_GRAVITY", (u32) EMITTER_GRAVITY);
	state.SetField(-1, "EMITTER_RADIAL", (u32) EMITTER_RADIAL);
	
	state.SetField(-1, "PARTICLE_POSITION_RELATIVE", (u32) PARTICLE_POSITION_RELATIVE );
	state.SetField(-1, "PARTICLE_POSITION_FREE", (u32) PARTICLE_POSITION_FREE );
	state.SetField(-1, "PARTICLE_POSITION_GROUPED", (u32) PARTICLE_POSITION_GROUPED );
	
	state.SetField(-1, "INDEX_MODE_SEQUENTIAL", (u32) INDEX_MODE_SEQUENTIAL );
	state.SetField(-1, "INDEX_MODE_RANDOM", (u32) INDEX_MODE_RANDOM );
}

void MOAICCParticleSystem::RegisterLuaFuncs( MOAILuaState &state ) {
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getAngle",								_getAngle },
		{ "setAngle",								_setAngle },
		{ "getAngleVariance",						_getAngleVariance },
		{ "setAngleVariance",						_setAngleVariance },
		{ "getBlendMode",							_getBlendMode },
		{ "setBlendModeParams",						_setBlendMode }, // changed name to avoid conflict with MOAIProp Lua method
		{ "getDuration",							_getDuration },
		{ "setDuration",							_setDuration },
		{ "getEmitterType",							_getEmitterType },
		{ "setEmitterType",							_setEmitterType },
		{ "getFinishColor",							_getFinishColor },
		{ "setFinishColor",							_setFinishColor },
		{ "getFinishColorVariance",					_getFinishColorVariance },
		{ "setFinishColorVariance",					_setFinishColorVariance },
		{ "getFinishParticleSize",					_getFinishParticleSize },
		{ "setFinishParticleSize",					_setFinishParticleSize },
		{ "getFinishParticleSizeVariance",			_getFinishParticleSizeVariance },
		{ "setFinishParticleSizeVariance",			_setFinishParticleSizeVariance },
		{ "getFrequency",							_getFrequency },
		{ "getGravity",								_getGravity },
		{ "setGravity",								_setGravity },
		{ "getLifespan",							_getLifespan },
		{ "getStartRadius",							_getStartRadius },
		{ "setStartRadius",							_setStartRadius },
		{ "getStartRadiusVariance",					_getStartRadiusVariance },
		{ "setStartRadiusVariance",					_setStartRadiusVariance },
		{ "getEndRadius",							_getEndRadius },
		{ "setEndRadius",							_setEndRadius },
		{ "getEndRadiusVariance",					_getEndRadiusVariance },
		{ "setEndRadiusVariance",					_setEndRadiusVariance },
		{ "getParticleLifespan",					_getParticleLifespan },
		{ "setParticleLifespan",					_setParticleLifespan },
		{ "getParticleLifespanVariance",			_getParticleLifespanVariance },
		{ "setParticleLifespanVariance",			_setParticleLifespanVariance },
		{ "getRadialAcceleration",					_getRadialAcceleration },
		{ "setRadialAcceleration",					_setRadialAcceleration },
		{ "getRadialAccelVariance",					_getRadialAccelVariance },
		{ "setRadialAccelVariance",					_setRadialAccelVariance },
		{ "getRadialAccelerationVariance",			_getRadialAccelVariance }, // alias without abbreviation
		{ "setRadialAccelerationVariance",			_setRadialAccelVariance }, // alias without abbreviation
		{ "getRect",								_getRect },
		{ "getRotatePerSecond",						_getRotatePerSecond },
		{ "setRotatePerSecond",						_setRotatePerSecond },
		{ "getRotatePerSecondVariance",				_getRotatePerSecondVariance },
		{ "setRotatePerSecondVariance",				_setRotatePerSecondVariance },
		{ "getRotationEnd",							_getRotationEnd },
		{ "setRotationEnd",							_setRotationEnd },
		{ "getRotationEndVariance",					_getRotationEndVariance },
		{ "setRotationEndVariance",					_setRotationEndVariance },
		{ "getRotationStart",						_getRotationStart },
		{ "setRotationStart",						_setRotationStart },
		{ "getRotationStartVariance",				_getRotationStartVariance },
		{ "setRotationStartVariance",				_setRotationStartVariance },
		{ "getSourcePosition",						_getSourcePosition },
		{ "setSourcePosition",						_setSourcePosition },
		{ "getSourcePositionVariance",				_getSourcePositionVariance },
		{ "setSourcePositionVariance",				_setSourcePositionVariance },
		{ "getSpeed",								_getSpeed },
		{ "setSpeed",								_setSpeed },
		{ "getSpeedVariance",						_getSpeedVariance },
		{ "setSpeedVariance",						_setSpeedVariance },
		{ "getStartColor",							_getStartColor },
		{ "setStartColor",							_setStartColor },
		{ "getStartColorVariance",					_getStartColorVariance },
		{ "setStartColorVariance",					_setStartColorVariance },
		{ "getStartParticleSize",					_getStartParticleSize },
		{ "setStartParticleSize",					_setStartParticleSize },
		{ "getStartParticleSizeVariance",			_getStartParticleSizeVariance },
		{ "setStartParticleSizeVariance",			_setStartParticleSizeVariance },
		{ "getTangentalAcceleration",				_getTangentalAcceleration },
		{ "setTangentalAcceleration",				_setTangentalAcceleration },
		{ "getTangentalAccelerationVariance",		_getTangentalAccelerationVariance },
		{ "setTangentalAccelerationVariance",		_setTangentalAccelerationVariance },
		{ "getGravityVariance",						_getGravityVariance },
		{ "setGravityVariance",						_setGravityVariance },
		{ "getRotationalAcceleration",				_getRotationalAcceleration },
		{ "setRotationalAcceleration",				_setRotationalAcceleration },
		{ "getRotationalAccelerationVariance",		_getRotationalAccelerationVariance },
		{ "setRotationalAccelerationVariance",		_setRotationalAccelerationVariance },
		
		{ "getEmissionRate",						_getFrequency },
		{ "setEmissionRate",						_setEmissionRate },
		{ "getParticlePositionType",				_getParticlePositionType },
		{ "setParticlePositionType",				_setParticlePositionType },
		
		{ "getIndexMode",							_getIndexMode },
		{ "setIndexMode",							_setIndexMode },
		{ "getStartIndex",							_getStartIndex },
		{ "setStartIndex",							_setStartIndex },
		{ "getEndIndex",							_getEndIndex },
		{ "setEndIndex",							_setEndIndex },
		{ "getDeckCellDimensions",					_getDeckCellDimensions },
		{ "getDeckTileCount",						_getDeckTileCount },
		{ "getDeckTileDimensions",					_getDeckTileDimensions },
		{ "getDeckTileOffset",						_getDeckTileOffset },
		
		{ "getParticleCount",						_getParticleCount },
		{ "getTextureName",							_getTextureName },
		{ "flipY",									_flipY },
		{ "initializeProperties",					_initializeProperties },
		{ "load",									_load },
		{ "reset",									_reset },
		
		{ "getTotalParticles",						_getTotalParticles },
		{ "setTotalParticles",						_setTotalParticles },
		{ "startSystem",							_startSystem },
		{ "stopSystem",								_stopSystem },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAICCParticleSystem::ResetSystem ( bool activate ) {
	if (activate){
		this->mActive = true;
	}
	this->mElapsed = 0.0f;
	this->mSeqIndex = 0;
	for (int i = 0; i < (int)this->mParticleCount; ++i) {
		MOAICCParticle *p = &(this->mParticles[i]);
		p->mTimeToLive = 0;
	}
}

void MOAICCParticleSystem::SetTotalParticles ( u32 numberOfParticles ){
	this->mTotalParticles = numberOfParticles;
	
	if (!this->mParticles ||  numberOfParticles > this->mAllocatedParticles) {
		// allocate new memory
		size_t particlesize = numberOfParticles * sizeof( MOAICCParticle );
		
		this->mParticles = (MOAICCParticle *)realloc(this->mParticles, particlesize);
		bzero(this->mParticles, particlesize);
		
		this->mAllocatedParticles = numberOfParticles;
	}
	
	this->ResetSystem(false);
	
}


void MOAICCParticleSystem::SetVisibility ( bool visible ) {
	MOAIProp::SetVisible(visible);
}

void MOAICCParticleSystem::StartSystem () {
	this->mActive = true;
	
	//MOAIAction::Start();
	
}


void MOAICCParticleSystem::StopSystem () {
	this->mActive = false;
	this->mElapsed = this->mDuration;
	this->mEmitCounter = 0.0f;
}
