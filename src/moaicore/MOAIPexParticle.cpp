// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPexParticle.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleTimedEmitter.h>

 #define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
//================================================================//
// lua
//================================================================//



int	MOAIPexParticle::_getBlendMode( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	
	lua_pushnumber ( state, self->mBlendFuncSrc );
	lua_pushnumber ( state, self->mBlendFuncDst );
	return 2;
}
int MOAIPexParticle::_getDuration( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	
	lua_pushnumber ( state, self->mDuration );
	return 1;
}
int MOAIPexParticle::_getEmissionCount( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	lua_pushnumber ( state, self->mEmissionCount );
	return 1;
}
int MOAIPexParticle::_getEmissionRate( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	lua_pushnumber ( state, self->mEmissionRate );
	return 1;
}
int MOAIPexParticle::_getLifespan		( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	lua_pushnumber ( state, self->mLifespanTerm[0] );
	lua_pushnumber ( state, self->mLifespanTerm[1] );
	return 2;
}
int MOAIPexParticle::_getMaxParticles	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	lua_pushnumber ( state, self->mNumParticles );
	return 1;
}
int MOAIPexParticle::_getRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	lua_pushnumber ( state, -self->mSourcePosVariance[0] );
	lua_pushnumber ( state, -self->mSourcePosVariance[1] );
	lua_pushnumber ( state, self->mSourcePosVariance[0] );
	lua_pushnumber ( state, self->mSourcePosVariance[1] );
	return 4;
}
//----------------------------------------------------------------//
/**	@name	getSize
	@text	Return the particle size expected by the plugin.
	
	@in		MOAIParticlePlugin self
	@out	number size
*/
int MOAIPexParticle::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )
	
	lua_pushnumber ( state, self->mSize );
	
	return 1;
}

int MOAIPexParticle::_getTextureName( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIPexParticle, "U" )

		lua_pushstring( state, self->mTextureName);
	return 1;
}
/**	@name	loadExternal
	@text	Create a particle plugin from an XML file
	
	@in		String file to load
	@out	MOAIParticleSystem system
	@out	MOAITParticleTimedEmitter emitter
	@out	String	texture file associated with particle system
*/
int MOAIPexParticle::_loadExternal( lua_State* L ){

	//MOAI_LUA_SETUP ( MOAIParticlePlugin, "S" )
	MOAILuaState state ( L );										
	if ( !state.CheckParams ( 1, "S" )) {							
		MOAILog ( L, MOAILogMessages::MOAI_ParamTypeMismatch );		
		return 0;													
	}																
		
	cc8* xml = lua_tostring ( state, 1 );

	lua_getglobal ( L, "PexParticles" );
	
	if ( lua_isnil ( L, -1 )) {
		lua_newtable ( L );
		lua_setglobal ( L, "PexParticles" );
	}
	
	lua_getglobal ( L, "PexParticles" );

	assert ( lua_isnil ( L, -1 ) == false );
	MOAIPexParticle *particle = NULL;
	
	if ( lua_type (L, -2 ) == LUA_TUSERDATA) {
		particle = *( MOAIPexParticle** )lua_touserdata (L, -2 );
	}

	if(particle)
	{
		particle->PushLuaUserdata( state );
		return 1;
	}
	else if ( MOAILogMessages::CheckFileExists ( xml, L )) {
		TiXmlDocument doc;
		doc.LoadFile ( xml );
		particle = new MOAIPexParticle();
		MOAIPexParticle::Parse ( state, *particle, doc.RootElement ());
		particle->mParticlePath = xml;
		//PexInitFunc initFunc = &MOAIPexParticle::_initGravityScript;
		//particle->mInitFunc = &MOAIPexParticle::_initGravityScript;
		//particle->mRenderFunc = &MOAIPexParticle::_renderGravityScript;
		/*if( particle->mEmitterType == EMITTER_GRAVITY )
			particle->Init(_initGravityScript,_renderGravityScript);
		else
			particle->Init();*/
		//lua_setfield ( L, -2, xml );
		lua_pop ( L, 1 );
		particle->PushLuaUserdata( state );
		return 1;
	}
	
	return 0;
}
//================================================================//
// MOAIParticlePlugin
//================================================================//


void MOAIPexParticle::Parse( MOAILuaState& state, MOAIPexParticle& plugin, TiXmlNode* node )
{
	if ( !node ) return;
	
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
				if(plugin.mMaxRadiusVariance != 0)
						plugin.mMaxRadiusRegister = plugin.mSize++;
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
				if(plugin.mRotPerSecondVariance != 0)
						plugin.mRotPerSecondRegister = plugin.mSize++;
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
			else if(text == "texture")
				plugin.mTextureName = attribute->Value();
			
		}
		

		if(plugin.mEmitterType == EMITTER_GRAVITY)
		{
			plugin.mStartXRegister = plugin.mSize++;
			plugin.mStartYRegister = plugin.mSize++;
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

void MOAIPexParticle::_initGravityScript( float* particle, float* registers )
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
		
void MOAIPexParticle::_initRadialScript( float* particle, float* registers )
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
		registers[mRotPerSecondRegister] = randVal  + angleStartDeg;		
	}
	else
		registers[mRotPerSecondRegister] = mRotPerSecond + angleStartDeg;


	if( mMaxRadiusRegister > -1 )
	{
		registers[mMaxRadiusRegister] = USFloat::Rand (mMaxRadius - mMaxRadiusVariance, mMaxRadius + mMaxRadiusVariance);
		particle[MOAIParticle::PARTICLE_X] = Cos(angleStartDeg * (float)D2R) * registers[mMaxRadiusRegister];
		particle[MOAIParticle::PARTICLE_Y] = Sin(angleStartDeg * (float)D2R) * registers[mMaxRadiusRegister];
	}
	else
	{
		particle[MOAIParticle::PARTICLE_X] = Cos(angleStartDeg * (float)D2R) * mMaxRadius;
		particle[MOAIParticle::PARTICLE_Y] = Sin(angleStartDeg * (float)D2R) * mMaxRadius;
	}

	registers[mRadialRegister] = angleStartDeg;	
}

void MOAIPexParticle::_renderGravityScript		( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 )
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

			if( mTanAccelRegister > -1)
			{
				xVal *= -registers[mTanAccelRegister];
				yVal *= registers[mTanAccelRegister];
			}
			else
			{
				xVal *= -mTanAccel;
				yVal *= mTanAccel;
			}

			moveX += xVal;
			moveY += yVal;
		}
	}
	
	
	moveX += mGravity[0];
	moveY += mGravity[1];
	
	float delta = t1 - t0;
	
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
	//printf( "Arrr: %f, %f\n", particle[ MOAIParticle::PARTICLE_X ], particle[ MOAIParticle::PARTICLE_Y ]);

}
void MOAIPexParticle::_renderRadialScript( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 )
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

	
	if( mRotPerSecondRegister > - 1)
		eVal = registers[mRotPerSecondRegister];
	else
		eVal = mRotPerSecond;

	float v0 =  USInterpolate::Interpolate ( USInterpolate::kLinear, registers[mRadialRegister], eVal, t0 );
	float v1 =  USInterpolate::Interpolate ( USInterpolate::kLinear, registers[mRadialRegister], eVal, t1 );
	registers[mRadialRegister] += (v0 - v1);
	

	if(mMaxRadiusRegister > -1)
		sVal = registers[mMaxRadiusRegister];
	else
		sVal = mMaxRadius;
	float magVal = USInterpolate::Interpolate ( USInterpolate::kLinear, sVal, mMinRadius, t1 );
	
	particle[MOAIParticle::PARTICLE_X] = Cos(registers[mRadialRegister] * (float)D2R) * magVal;
    particle[MOAIParticle::PARTICLE_Y] = Sin(registers[mRadialRegister] * (float)D2R) * magVal;

	sprite->mXLoc = particle[ MOAIParticle::PARTICLE_X ];
	sprite->mYLoc = particle[ MOAIParticle::PARTICLE_Y ];

}

void  MOAIPexParticle::InitFunc( float* particle, float* registers )
{
	if(mEmitterType == EMITTER_GRAVITY)
		_initGravityScript( particle, registers);
	else
		_initRadialScript( particle, registers);
}
void  MOAIPexParticle::RenderFunc( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1 )
{
	if(mEmitterType == EMITTER_GRAVITY)
		_renderGravityScript( particle, registers, sprite, t0, t1);
	else
		_renderRadialScript( particle, registers, sprite, t0, t1);
}

//----------------------------------------------------------------//
MOAIPexParticle::MOAIPexParticle () :
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

	memset(mStartColorRegister, -1, 4);
	memset(mFinishColorRegister, -1, 4);


	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIPexParticle::~MOAIPexParticle () {
}

//----------------------------------------------------------------//
void MOAIPexParticle::RegisterLuaClass ( MOAILuaState& state ) {
	//UNUSED ( state );
	luaL_Reg regTable [] = {
		{ "loadExternal", _loadExternal },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPexParticle::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "getBlendMode",		_getBlendMode },
		{ "getDuration",		_getDuration },
		{ "getEmissionCount",	_getEmissionCount },
		{ "getEmissionRate",	_getEmissionRate },
		{ "getLifespan",		_getLifespan },
		{ "getMaxParticles",	_getMaxParticles },
		{ "getRect",			_getRect },
		{ "getSize",			_getSize },
		{ "getTextureName",		_getTextureName },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

