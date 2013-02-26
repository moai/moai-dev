// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticlePexPlugin.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleTimedEmitter.h>

//================================================================
// An implementation of a particle plugin that implements the
// behavior that this tool creates:
//   http://particledesigner.71squared.com/
// 
// It basically has a ton of tweakable variables and changes behavior
// based on those variables at runtime. This implementation currently
// tries to reduce the register count per particle by detecting when
// values are invariant over the lifetime of each particle and storing
// that on the plugin instance. This makes the implementations below
// a bit more complex (and can probably be optimized further), but it
// drastically reduces the footprint for most particle configurations
// which are only "using" a subset of the total variable set at a time.
// (Upwards of like 30+ registers per particle starts to eat into memory
// budgets.)
//================================================================


//================================================================//
// lua
//================================================================//

int	MOAIParticlePexPlugin::_getBlendMode( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	
	lua_pushnumber ( state, self->mBlendFuncSrc );
	lua_pushnumber ( state, self->mBlendFuncDst );
	return 2;
}

int MOAIParticlePexPlugin::_getDuration( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	
	lua_pushnumber ( state, self->mDuration );
	return 1;
}

int MOAIParticlePexPlugin::_getEmission( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	lua_pushnumber ( state, self->mEmissionCount );
	return 1;
}

int MOAIParticlePexPlugin::_getFrequency( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	lua_pushnumber ( state, self->mEmissionRate );
	return 1;
}

int MOAIParticlePexPlugin::_getLifespan		( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	lua_pushnumber ( state, self->mLifespanTerm[0] );
	lua_pushnumber ( state, self->mLifespanTerm[1] );
	return 2;
}

int MOAIParticlePexPlugin::_getMaxParticles	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	lua_pushnumber ( state, self->mNumParticles );
	return 1;
}

int MOAIParticlePexPlugin::_getRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )
	lua_pushnumber ( state, -self->mSourcePosVariance[0] );
	lua_pushnumber ( state, -self->mSourcePosVariance[1] );
	lua_pushnumber ( state, self->mSourcePosVariance[0] );
	lua_pushnumber ( state, self->mSourcePosVariance[1] );
	return 4;
}

//----------------------------------------------------------------//
/**	@name	getTextureName
	@text	Return the texture name associated with plugin.
	
	@in		MOAIParticlePlugin self
	@out	string textureName
*/
int MOAIParticlePexPlugin::_getTextureName( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )

		lua_pushstring( state, self->mTextureName);
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Create a particle plugin from an XML file
	
	@in		String file to load
	@out	MOAIParticlePexPlugin - The plugin object that has been initialized with XML's data
*/
int MOAIParticlePexPlugin::_load( lua_State* L ){

	MOAILuaState state ( L );										
	if ( !state.CheckParams ( 1, "S" )) {							
		MOAILog ( L, MOAILogMessages::MOAI_ParamTypeMismatch );		
		return 0;													
	}																
		
	cc8* xml = lua_tostring ( state, 1 );

	if ( MOAILogMessages::CheckFileExists ( xml, L )) {
		TiXmlDocument doc;
		doc.LoadFile ( xml );
		MOAIParticlePexPlugin *particle = new MOAIParticlePexPlugin();
		MOAIParticlePexPlugin::Parse ( xml, *particle, doc.RootElement ());
		particle->PushLuaUserdata( state );
		return 1;
	}
	
	return 0;
}
//================================================================//
// MOAIParticlePlugin
//================================================================//


void MOAIParticlePexPlugin::Parse( cc8* filename, MOAIParticlePexPlugin& plugin, TiXmlNode* node )
{
	if ( !node ) return;
	
	plugin.mParticlePath = filename;
	
	STLString absFilePath = USFileSys::GetAbsoluteFilePath ( filename );
	STLString absDirPath = USFileSys::TruncateFilename ( absFilePath );
	
	TiXmlElement* element = node->ToElement();

	if ( element && strcmp(element->Value (), "patricleEmitterConfig") ) {
		
		plugin.mSize = 0;
		// round up the children
		STLSet < string > children;
		TiXmlElement* childElement = node->FirstChildElement ();
		
		for ( ; childElement; childElement = childElement->NextSiblingElement ()) {
			STLString text = childElement->Value ();
			TiXmlAttribute* attribute = childElement->FirstAttribute ();
			int i = 0;
			if(!attribute || !text || text == "")
				continue;

			if(text == "angle")
				plugin.mAngle = (float)atof(attribute->Value());
			else if(text == "angleVariance")
			{
				plugin.mAngleVariance = (float)atof(attribute->Value());
				if(plugin.mAngleVariance != 0)
					plugin.mAngleRegister = plugin.mSize++;
			}
			else if(text == "blendFuncSource")
				plugin.mBlendFuncSrc = atoi(attribute->Value());
			else if(text == "blendFuncDestination")
				plugin.mBlendFuncDst = atoi(attribute->Value());
			else if(text == "duration")
				plugin.mDuration = (float)atof(attribute->Value());
			else if(text == "emitterType")
				plugin.mEmitterType = (EmitterType)atoi(attribute->Value());
			else if(text == "finishColor")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					plugin.mFinishColor[i] = (float)atof(attribute->Value());
				}
			else if(text == "finishColorVariance")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					plugin.mFinishColorVariance[i] = (float)atof(attribute->Value());
					if(plugin.mFinishColorVariance[i] != 0)
						plugin.mFinishColorRegister[i] = plugin.mSize++;
				}
			else if(text == "finishParticleSize")
				plugin.mFinishSize = (float)atof(attribute->Value());
			else if(text == "FinishParticleSizeVariance")
			{
				plugin.mFinishSizeVariance = (float)atof(attribute->Value());
				if(plugin.mFinishSizeVariance != 0)
						plugin.mFinishSizeRegister = plugin.mSize++;
			}
			else if(text == "gravity")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					plugin.mGravity[i] = (float)atof(attribute->Value());
				}
			else if(text == "maxParticles")
				plugin.mNumParticles = atoi(attribute->Value());
			else if(text == "maxRadius")
				plugin.mMaxRadius = (float)atof(attribute->Value());
			else if(text == "maxRadiusVariance")
			{
				plugin.mMaxRadiusVariance = (float)atof(attribute->Value());
				if(plugin.mMaxRadiusVariance != 0)
						plugin.mMaxRadiusRegister = plugin.mSize++;
			}
			else if(text == "minRadius")
				plugin.mMinRadius = (float)atof(attribute->Value());
			else if(text == "particleLifeSpan")
				plugin.mLifespan = (float)atof(attribute->Value());
			else if(text == "particleLifespanVariance")
			{
				plugin.mLifespanVariance = (float)atof(attribute->Value());
				if(plugin.mLifespanVariance != 0)
						plugin.mLifespanRegister = plugin.mSize++;
			}
			else if(text == "radialAcceleration")
				plugin.mRadialAcceleration = (float)atof(attribute->Value());
			else if(text == "radialAccelVariance")
			{
				plugin.mRadialAccelVariance = (float)atof(attribute->Value());
				if(plugin.mRadialAccelVariance != 0)
						plugin.mRadialAccelRegister = plugin.mSize++;
			}
			else if(text == "rotatePerSecond")
				plugin.mRotPerSecond = (float)atof(attribute->Value());
			else if(text == "rotatePerSecondVariance")
			{
				plugin.mRotPerSecondVariance = (float)atof(attribute->Value());
			}
			else if(text == "rotationEnd")
				plugin.mRotEnd = (float)atof(attribute->Value());
			else if(text == "rotationEndVariance")
			{
				plugin.mRotEndVariance = (float)atof(attribute->Value());
				if(plugin.mRotEndVariance != 0)
						plugin.mRotEndRegister = plugin.mSize++;
			}
			else if(text == "rotationStart")
				plugin.mRotStart = (float)atof(attribute->Value());
			else if(text == "rotationStartVariance")
			{
				plugin.mRotStartVariance = (float)atof(attribute->Value());
				if(plugin.mRotStartVariance != 0)
						plugin.mRotStartRegister = plugin.mSize++;
			}
			else if(text == "sourcePosition")
				for ( ; attribute; attribute = attribute->Next (), i++) {
						plugin.mSourcePos[i] = (float)atof(attribute->Value());
					}
			else if(text == "sourcePositionVariance")
				for ( ; attribute; attribute = attribute->Next (), i++) {
						plugin.mSourcePosVariance[i] = (float)atof(attribute->Value());
					}
			else if(text == "speed")
				plugin.mSpeed = (float)atof(attribute->Value());
			else if(text == "speedVariance")
			{
				plugin.mSpeedVariance = (float)atof(attribute->Value());
				if(plugin.mSpeedVariance != 0)
					plugin.mSpeedRegister = plugin.mSize++;
			}
			else if(text == "startColor")
					for ( ; attribute; attribute = attribute->Next (), i++) {
						plugin.mStartColor[i] = (float)atof(attribute->Value());
					}
			else if(text == "startColorVariance")
					for ( ; attribute; attribute = attribute->Next (), i++) {
						plugin.mStartColorVariance[i] = (float)atof(attribute->Value());
						if(plugin.mStartColorVariance[i] != 0)
							plugin.mStartColorRegister[i] = plugin.mSize++;
					}
			else if(text == "startParticleSize")
				plugin.mStartSize = (float)atof(attribute->Value());
			else if(text == "startParticleSizeVariance")
			{
				plugin.mStartSizeVariance = (float)atof(attribute->Value());
				if(plugin.mStartSizeVariance != 0)
					plugin.mStartSizeRegister = plugin.mSize++;
			}
			else if(text == "tangentialAcceleration")
				plugin.mTanAccel = (float)atof(attribute->Value());
			else if(text == "tangentialAccelVariance")
			{
				plugin.mTanAccelVariance = (float)atof(attribute->Value());
				if(plugin.mTanAccelVariance != 0)
					plugin.mTanAccelRegister = plugin.mSize++;
			}
			else if(text == "texture") {
				plugin.mTextureName = absDirPath;
				plugin.mTextureName.append ( attribute->Value ());
			}
			
		}
		
		plugin.mStartXRegister = plugin.mSize++;
		plugin.mStartYRegister = plugin.mSize++;

		if(plugin.mEmitterType == EMITTER_GRAVITY)
		{		
			plugin.mDirectionXRegister = plugin.mSize++;
			plugin.mDirectionYRegister = plugin.mSize++;
		}
		else
		{
			plugin.mRotPerSecondRegister = plugin.mSize++;
			plugin.mRadialRegister = plugin.mSize++;
		}

		// Set up emitter.
		plugin.mEmissionRate =  1.0f / (plugin.mNumParticles / plugin.mLifespan );
		plugin.mEmissionCount = 1;
		if( plugin.mEmissionRate <  0.05 )
		{
			plugin.mEmissionCount = (u32)ceil( 0.05 / plugin.mEmissionRate );
			plugin.mEmissionRate *= plugin.mEmissionCount;
		}

		plugin.mLifespanTerm[0] = plugin.mLifespan - plugin.mLifespanVariance < 0 ? 0 : plugin.mLifespan - plugin.mLifespanVariance;
		plugin.mLifespanTerm[1] = plugin.mLifespan + plugin.mLifespanVariance;
		
	}
}

void MOAIParticlePexPlugin::_initGravityScript( float* particle, float* registers)
{
	// Set colors.
	for(int i = 0; i < 4; i++)
	{

		if(mStartColorRegister[i] > -1 )
		{
			float minVal = mStartColor[i] - mStartColorVariance[i] < 0 ? 0 : mStartColor[i] - mStartColorVariance[i];	
			registers[mStartColorRegister[i]] = USFloat::Rand (minVal,  mStartColor[i] + mStartColorVariance[i] );
		}	

			
		if(this->mFinishColorRegister[i]  > -1 )
		{
			float minVal = mFinishColor[i] - mFinishColorVariance[i] < 0 ? 0 : mFinishColor[i] - mFinishColorVariance[i];	
			registers[mFinishColorRegister[i]] =USFloat::Rand (minVal,  mFinishColor[i] + mFinishColorVariance[i] );
		}
			
	}

	if(mStartSizeRegister > -1)
	{
		float minVal = mStartSize - mStartSizeVariance < 0 ? 0 :  mStartSize - mStartSizeVariance;
		registers[mStartSizeRegister] = USFloat::Rand (minVal,  mStartSize + mStartSizeVariance);
	}

			
	if(mFinishSizeRegister > -1)
	{
		float minVal = mFinishSize - mFinishSizeVariance < 0 ? 0 :  mFinishSize - mFinishSizeVariance;
		registers[mFinishSizeRegister] = USFloat::Rand (minVal,  mFinishSize + mFinishSizeVariance);
	}

	if(mRotStartRegister > -1 )
		registers[mRotStartRegister] = USFloat::Rand (mRotStart-mRotStartVariance, mRotStart+mRotStartVariance);
	

	if(mRotEndRegister > -1)
		registers[mRotStartRegister] = USFloat::Rand (mRotEnd-mRotEndVariance, mRotEnd+mRotEndVariance);


	float angleStartDeg;
	if(mAngleRegister > -1)
	{
		// S.S. updated angle to fix translation from degrees to radians
		float randAngle = USFloat::Rand (- mAngleVariance, + mAngleVariance);

		angleStartDeg = (float)(atan2 ( particle[MOAIParticle::PARTICLE_DY], particle[MOAIParticle::PARTICLE_DX] ) * R2D + randAngle);

		particle[MOAIParticle::PARTICLE_DX] = (float)Cos((mAngle + randAngle) * (float)D2R);
		particle[MOAIParticle::PARTICLE_DY] = (float)Sin((mAngle + randAngle) * (float)D2R);
	}
	else
	{
		angleStartDeg = (float)(atan2 ( particle[MOAIParticle::PARTICLE_DY], particle[MOAIParticle::PARTICLE_DX] ) * R2D) + mAngle;
		particle[MOAIParticle::PARTICLE_DX] = (float)Cos(angleStartDeg * (float)D2R);
		particle[MOAIParticle::PARTICLE_DY] = (float)Sin(angleStartDeg * (float)D2R);
	}

	
	// Set initial speed
	if(mSpeedRegister > -1)
	{
		registers[mSpeedRegister] = USFloat::Rand(mSpeed - mSpeedVariance, mSpeed + mSpeedVariance);
		registers[mDirectionXRegister] = particle[MOAIParticle::PARTICLE_DX] * registers[mSpeedRegister];
		registers[mDirectionYRegister] = particle[MOAIParticle::PARTICLE_DY] * registers[mSpeedRegister];
	}
	else
	{
		registers[mDirectionXRegister] = particle[MOAIParticle::PARTICLE_DX] * mSpeed;
		registers[mDirectionYRegister] = particle[MOAIParticle::PARTICLE_DY] * mSpeed;
	}

	registers[mStartXRegister] = particle[MOAIParticle::PARTICLE_X];
	registers[mStartYRegister] = particle[MOAIParticle::PARTICLE_Y];
	
	if(mRadialAccelRegister > -1)
		registers[mRadialAccelRegister] = USFloat::Rand(mRadialAcceleration - mRadialAccelVariance, mRadialAcceleration + mRadialAccelVariance);

	
	if(mTanAccelRegister > -1)
		registers[mTanAccelRegister] = USFloat::Rand(mTanAccel - mTanAccelVariance, mTanAccel + mTanAccelVariance);

}
		
void MOAIParticlePexPlugin::_initRadialScript( float* particle, float* registers)
{

	// Set colors.
	for(int i = 0; i < 4; i++)
	{

		if(mStartColorRegister[i] > -1 )
		{
			float minVal = mStartColor[i] - mStartColorVariance[i] < 0 ? 0 : mStartColor[i] - mStartColorVariance[i];	
			registers[mStartColorRegister[i]] = USFloat::Rand (minVal,  mStartColor[i] + mStartColorVariance[i] );
		}	

			
		if(this->mFinishColorRegister[i]  > -1 )
		{
			float minVal = mFinishColor[i] - mFinishColorVariance[i] < 0 ? 0 : mFinishColor[i] - mFinishColorVariance[i];	
			registers[mFinishColorRegister[i]] =USFloat::Rand (minVal,  mFinishColor[i] + mFinishColorVariance[i] );
		}
			
	}

	if(mStartSizeRegister > -1)
	{
		float minVal = mStartSize - mStartSizeVariance < 0 ? 0 :  mStartSize - mStartSizeVariance;
		registers[mStartSizeRegister] = USFloat::Rand (minVal,  mStartSize + mStartSizeVariance);
	}

			
	if(mFinishSizeRegister > -1)
	{
		float minVal = mFinishSize - mFinishSizeVariance < 0 ? 0 :  mFinishSize - mFinishSizeVariance;
		registers[mFinishSizeRegister] = USFloat::Rand (minVal,  mFinishSize + mFinishSizeVariance);
	}

	if(mRotStartRegister > -1 )
		registers[mRotStartRegister] = USFloat::Rand (mRotStart-mRotStartVariance, mRotStart+mRotStartVariance);
	

	if(mRotEndRegister > -1)
		registers[mRotStartRegister] = USFloat::Rand (mRotEnd-mRotEndVariance, mRotEnd+mRotEndVariance);


	float angleStartDeg;
	if(mAngleRegister > -1)
	{
		float randAngle = USFloat::Rand (mAngle - mAngleVariance, mAngle + mAngleVariance);
		particle[MOAIParticle::PARTICLE_DX] = Cos(randAngle * (float)D2R);
		particle[MOAIParticle::PARTICLE_DY] = Sin(randAngle * (float)D2R);
		angleStartDeg = randAngle;

	}
	else
	{
		particle[MOAIParticle::PARTICLE_DX] = Cos(mAngle * (float)D2R);
		particle[MOAIParticle::PARTICLE_DY] = Sin(mAngle * (float)D2R);
		angleStartDeg = mAngle;
	}
	
	if( mRotPerSecondVariance != 0)
	{
		float randVal =  USFloat::Rand (mRotPerSecond - mRotPerSecondVariance, mRotPerSecond + mRotPerSecondVariance);
		registers[mRotPerSecondRegister] = randVal;		
	}
	else
		registers[mRotPerSecondRegister] = mRotPerSecond;


	registers[mStartXRegister] = particle[MOAIParticle::PARTICLE_X];
	registers[mStartYRegister] = particle[MOAIParticle::PARTICLE_Y];

	if( mMaxRadiusRegister > -1 )
	{
		registers[mMaxRadiusRegister] = USFloat::Rand (mMaxRadius - mMaxRadiusVariance, mMaxRadius + mMaxRadiusVariance);
		particle[MOAIParticle::PARTICLE_X] += Cos(angleStartDeg * (float)D2R) * registers[mMaxRadiusRegister];
		particle[MOAIParticle::PARTICLE_Y] += Sin(angleStartDeg * (float)D2R) * registers[mMaxRadiusRegister];
	}
	else
	{
		particle[MOAIParticle::PARTICLE_X] += Cos(angleStartDeg * (float)D2R) * mMaxRadius;
		particle[MOAIParticle::PARTICLE_Y] += Sin(angleStartDeg * (float)D2R) * mMaxRadius;
	}

	registers[mRadialRegister] = angleStartDeg;	
}

void MOAIParticlePexPlugin::_renderGravityScript		( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term )
{
	// Set colors.
	float sVal, eVal;
	for(int i = 0; i < 4; i++)
	{
			
		if(mStartColorRegister[i] > -1 )
			sVal = registers[mStartColorRegister[i]];
		else
			sVal = mStartColor[i];

		if(mFinishColorRegister[i] > -1 )
			eVal = registers[mFinishColorRegister[i]];
		else
			eVal = mFinishColor[i];

		switch(i)
		{
		case 0:
			sprite->mRed = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 1:
			sprite->mGreen = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 2:
			sprite->mBlue = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 3:
			sprite->mAlpha = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		}
	}

	if(mStartSizeRegister > -1 )
		sVal = registers[mStartSizeRegister];
	else
		sVal = mStartSize;

	if(mFinishSizeRegister > -1 )
		eVal = registers[mFinishSizeRegister];
	else
		eVal = mFinishSize;

	if(sVal != eVal)
	{
		sprite->mXScl = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
		sprite->mYScl = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
	}
	else
	{
		sprite->mXScl = sVal;
		sprite->mYScl = sVal;
	}


	if(mRotStartRegister > -1 )
		sVal = registers[mRotStartRegister];
	else
		sVal = mRotStart;

	if(mRotEndRegister > -1 )
		eVal = registers[mRotEndRegister];
	else
		eVal = mRotEnd;

	if( sVal != eVal)
		sprite->mZRot = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
	else
		sprite->mZRot = sVal;

	float moveX = 0;
	float moveY = 0;

	particle[MOAIParticle::PARTICLE_X] = particle[MOAIParticle::PARTICLE_X] - registers[mStartXRegister];
	particle[MOAIParticle::PARTICLE_Y] = particle[MOAIParticle::PARTICLE_Y] - registers[mStartYRegister];

	if(mRadialAcceleration != 0 || mRadialAccelVariance != 0 || 
		mTanAccel != 0 || mTanAccelVariance != 0)
	{
		float xVal, yVal;
		float denom = Sqrt( (particle[MOAIParticle::PARTICLE_X] * particle[MOAIParticle::PARTICLE_X]) +
								(particle[MOAIParticle::PARTICLE_Y] * particle[MOAIParticle::PARTICLE_Y]) );

		// Set radial acceleration
		if( mRadialAcceleration != 0 || mRadialAccelVariance != 0)
		{
			if(denom)
			{
				xVal = (float)( particle[MOAIParticle::PARTICLE_X] / denom );
				yVal = (float)( particle[MOAIParticle::PARTICLE_Y] / denom );
			}
			else
			{
				xVal = 0;
				yVal = 0;
			}
	
			if( mRadialAccelRegister > -1)
			{
				xVal *= registers[mRadialAccelRegister];
				yVal *= registers[mRadialAccelRegister];
			}
			else
			{
				xVal *= mRadialAcceleration;
				yVal *= mRadialAcceleration;
			}
			moveX += xVal;
			moveY += yVal;
		}

		// Set tangential acceleration
		if(mTanAccel != 0 || mTanAccelVariance != 0)
		{
			if(denom)
			{
				xVal = (float)( particle[MOAIParticle::PARTICLE_X] / denom );
				yVal = (float)( particle[MOAIParticle::PARTICLE_Y] / denom );
			}
			else
			{
				xVal = 0;
				yVal = 0;
			}

			float yNewVal = xVal;
			if( mTanAccelRegister > -1)
			{
				xVal = -yVal * registers[mTanAccelRegister];
				yVal = yNewVal * registers[mTanAccelRegister];
			}
			else
			{
				xVal = -yVal * mTanAccel;
				yVal = yNewVal * mTanAccel;
			}

			moveX += xVal;
			moveY += yVal;
		}
	}
	
	
	moveX += mGravity[0];
	moveY += mGravity[1];
	
	float delta = t1 - t0;
	delta *= term;
	
	moveX *= delta;
	moveY *= delta;

	registers[ mDirectionXRegister ] += moveX;
	registers[ mDirectionYRegister ] += moveY;

	moveX = registers[ mDirectionXRegister ] * delta;
	moveY = registers[ mDirectionYRegister ] * delta;

	particle[ MOAIParticle::PARTICLE_X ] += moveX + registers[mStartXRegister];
	particle[ MOAIParticle::PARTICLE_Y ] += moveY + registers[mStartYRegister];

	sprite->mXLoc = particle[ MOAIParticle::PARTICLE_X ];
	sprite->mYLoc = particle[ MOAIParticle::PARTICLE_Y ];

	sprite->mGfxID = 1;
	
}
void MOAIParticlePexPlugin::_renderRadialScript( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term)
{
	// Set colors.
	float sVal, eVal;
	for(int i = 0; i < 4; i++)
	{
			
		if(mStartColorRegister[i] > -1 )
			sVal = registers[mStartColorRegister[i]];
		else
			sVal = mStartColor[i];

		if(mFinishColorRegister[i] > -1 )
			eVal = registers[mFinishColorRegister[i]];
		else
			eVal = mFinishColor[i];

		switch(i)
		{
		case 0:
			sprite->mRed = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 1:
			sprite->mGreen = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 2:
			sprite->mBlue = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 3:
			sprite->mAlpha = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
			break;
		}
	}

	if(mStartSizeRegister > -1 )
		sVal = registers[mStartSizeRegister];
	else
		sVal = mStartSize;

	if(mFinishSizeRegister > -1 )
		eVal = registers[mFinishSizeRegister];
	else
		eVal = mFinishSize;

	if(sVal != eVal)
	{
		sprite->mXScl = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
		sprite->mYScl = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
	}
	else
	{
		sprite->mXScl = sVal;
		sprite->mYScl = sVal;
	}


	if(mRotStartRegister > -1 )
		sVal = registers[mRotStartRegister];
	else
		sVal = mRotStart;

	if(mRotEndRegister > -1 )
		eVal = registers[mRotEndRegister];
	else
		eVal = mRotEnd;


	if( sVal != eVal)
		sprite->mZRot = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, eVal, t1 );
	else
		sprite->mZRot = sVal;

	
	registers[mRadialRegister] += registers[mRotPerSecondRegister] * ((t1 - t0)*term);

	if(mMaxRadiusRegister > -1)
		sVal = registers[mMaxRadiusRegister];
	else
		sVal = mMaxRadius;
	float magVal = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, mMinRadius, t1 );
	
	particle[MOAIParticle::PARTICLE_X] = registers[mStartXRegister] - Cos(registers[mRadialRegister] * (float)D2R) * magVal;
    particle[MOAIParticle::PARTICLE_Y] = registers[mStartYRegister] - Sin(registers[mRadialRegister] * (float)D2R) * magVal;

	sprite->mXLoc = particle[ MOAIParticle::PARTICLE_X ];
	sprite->mYLoc = particle[ MOAIParticle::PARTICLE_Y ];

}

void  MOAIParticlePexPlugin::OnInit ( float* particle, float* registers)
{
	if(mEmitterType == EMITTER_GRAVITY)
		_initGravityScript( particle, registers);
	else
		_initRadialScript( particle, registers);
}
void  MOAIParticlePexPlugin::OnRender ( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1,  float term)
{
	if(mEmitterType == EMITTER_GRAVITY)
		_renderGravityScript( particle, registers, sprite, t0, t1, term);
	else
		_renderRadialScript( particle, registers, sprite, t0, t1, term);
}

//----------------------------------------------------------------//
MOAIParticlePexPlugin::MOAIParticlePexPlugin () :
	mLifespanRegister(-1),
	mAngleRegister(-1),
	mStartSizeRegister(-1),
	mFinishSizeRegister(-1),
	mMaxRadiusRegister(-1),
	mRadialAccelRegister(-1),
	mTanAccelRegister(-1),
	mRotStartRegister(-1),
	mRotEndRegister(-1),
	mSpeedRegister(-1){

	memset(mStartColorRegister, -1, sizeof(mStartColorRegister));
	memset(mFinishColorRegister, -1,sizeof(mFinishColorRegister));


	RTTI_SINGLE ( MOAIParticlePlugin )
}

//----------------------------------------------------------------//
MOAIParticlePexPlugin::~MOAIParticlePexPlugin () {
}

//----------------------------------------------------------------//
void MOAIParticlePexPlugin::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIParticlePlugin::RegisterLuaClass ( state );

	//UNUSED ( state );
	luaL_Reg regTable [] = {
		{ "load", _load },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticlePexPlugin::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIParticlePlugin::RegisterLuaFuncs ( state );

	luaL_Reg regTable[] = {
		{ "getBlendMode",		_getBlendMode },
		{ "getDuration",		_getDuration },
		{ "getEmission",		_getEmission },
		{ "getFrequency",		_getFrequency },
		{ "getLifespan",		_getLifespan },
		{ "getMaxParticles",	_getMaxParticles },
		{ "getRect",			_getRect },
		{ "getTextureName",		_getTextureName },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
