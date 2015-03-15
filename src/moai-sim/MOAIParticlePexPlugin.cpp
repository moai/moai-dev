// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#if MOAI_WITH_TINYXML
	#include <tinyxml.h>
#endif

#include <moai-sim/MOAIParticlePexPlugin.h>
#include <moai-sim/MOAIParticleSystem.h>
#include <moai-sim/MOAIParticleState.h>
#include <moai-sim/MOAIParticleScript.h>
#include <moai-sim/MOAIParticleTimedEmitter.h>

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
/**	@lua	getTextureName
	@text	Return the texture name associated with plugin.
	
	@in		MOAIParticlePexPlugin self
	@out	string textureName
*/
int MOAIParticlePexPlugin::_getTextureName( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIParticlePexPlugin, "U" )

		lua_pushstring( state, self->mTextureName);
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Create a particle plugin from an XML file
	
	@in		string fileName					file to load
	@out	MOAIParticlePexPlugin plugin	The plugin object that has been initialized with XML's data
*/
int MOAIParticlePexPlugin::_load( lua_State* L ){
	UNUSED ( L );

	#if MOAI_WITH_TINYXML
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
			particle->PushLuaUserdata ( state );
			return 1;
		}
	#endif	
	return 0;
}
//================================================================//
// MOAIParticlePlugin
//================================================================//
#if MOAI_WITH_TINYXML
#include <zl-gfx/headers.h>

// warning: modifies size argument
static
void read_n_values(float *array, TiXmlAttribute *attribute, int max, int *registers, int &size) {
	for (int i = 0; i < max; ++i) {
		if (attribute) {
			array[i] = atof(attribute->Value());
			if (array[i] != 0) {
				if (registers) {
					registers[i] = size++;
				}
			}
			attribute = attribute->Next();
		} else {
			array[i] = 0;
		}
	}
}

void MOAIParticlePexPlugin::Parse( cc8* filename, MOAIParticlePexPlugin& plugin, TiXmlNode* node )
{
	if ( !node ) return;
	
	plugin.mParticlePath = filename;
	
	STLString absFilePath = ZLFileSys::GetAbsoluteFilePath ( filename );
	STLString absDirPath = ZLFileSys::TruncateFilename ( absFilePath );
	
	TiXmlElement* element = node->ToElement();

	if ( element && !strcmp(element->Value (), "particleEmitterConfig") ) {
		
		plugin.mSize = 0;
		// round up the children
		STLSet < string > children;
		TiXmlElement* childElement = node->FirstChildElement ();
		
		for ( ; childElement; childElement = childElement->NextSiblingElement ()) {
			STLString text = childElement->Value ();
			TiXmlAttribute* attribute = childElement->FirstAttribute ();

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
				plugin.mBlendFuncSrc = zglMapFromGLEnum( atoi(attribute->Value()));
			else if(text == "blendFuncDestination")
				plugin.mBlendFuncDst = zglMapFromGLEnum( atoi(attribute->Value()));
			else if(text == "duration")
				plugin.mDuration = (float)atof(attribute->Value());
			else if(text == "emitterType")
				plugin.mEmitterType = (EmitterType)atoi(attribute->Value());
			else if(text == "finishColor")
				read_n_values(plugin.mFinishColor, attribute, 4, NULL, plugin.mSize);
			else if(text == "finishColorVariance")
			    read_n_values(plugin.mFinishColorVariance, attribute, 4, plugin.mFinishColorRegister, plugin.mSize);
			else if(text == "finishParticleSize")
				plugin.mFinishSize = (float)atof(attribute->Value());
			// this used to be spelled with a capital F, accept that for
			// compatibility.
			else if(text == "finishParticleSizeVariance" || text == "FinishParticleSizeVariance")
			{
				plugin.mFinishSizeVariance = (float)atof(attribute->Value());
				if(plugin.mFinishSizeVariance != 0)
						plugin.mFinishSizeRegister = plugin.mSize++;
			}
			else if(text == "gravity")
				read_n_values(plugin.mGravity, attribute, 2, NULL, plugin.mSize);
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
				read_n_values(plugin.mSourcePos, attribute, 2, NULL, plugin.mSize);
			else if(text == "sourcePositionVariance")
				read_n_values(plugin.mSourcePosVariance, attribute, 2, NULL, plugin.mSize);
			else if(text == "speed")
				plugin.mSpeed = (float)atof(attribute->Value());
			else if(text == "speedVariance")
			{
				plugin.mSpeedVariance = (float)atof(attribute->Value());
				if(plugin.mSpeedVariance != 0)
					plugin.mSpeedRegister = plugin.mSize++;
			}
			else if(text == "startColor")
				read_n_values(plugin.mStartColor, attribute, 4, NULL, plugin.mSize);
			else if(text == "startColorVariance")
				read_n_values(plugin.mStartColorVariance, attribute, 4, plugin.mStartColorRegister, plugin.mSize);
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
#endif

void  MOAIParticlePexPlugin::OnInit ( float* particle, float* registers)
{
	// Set colors.
	for(int i = 0; i < 4; i++)
	{

		if (mStartColorRegister[i] > -1 )
		{
			float minVal = mStartColor[i] - mStartColorVariance[i] < 0 ? 0 : mStartColor[i] - mStartColorVariance[i];	
			registers[mStartColorRegister[i]] = ZLFloat::Rand (minVal,  mStartColor[i] + mStartColorVariance[i] );
		}	

			
		if(mFinishColorRegister[i]  > -1 )
		{
			float minVal = mFinishColor[i] - mFinishColorVariance[i] < 0 ? 0 : mFinishColor[i] - mFinishColorVariance[i];	
			registers[mFinishColorRegister[i]] =ZLFloat::Rand (minVal,  mFinishColor[i] + mFinishColorVariance[i] );
		}
			
	}

	if(mStartSizeRegister > -1)
	{
		float minVal = mStartSize - mStartSizeVariance < 0 ? 0 :  mStartSize - mStartSizeVariance;
		registers[mStartSizeRegister] = ZLFloat::Rand (minVal,  mStartSize + mStartSizeVariance);
	}

			
	if(mFinishSizeRegister > -1)
	{
		float minVal = mFinishSize - mFinishSizeVariance < 0 ? 0 :  mFinishSize - mFinishSizeVariance;
		registers[mFinishSizeRegister] = ZLFloat::Rand (minVal,  mFinishSize + mFinishSizeVariance);
	}

	if(mRotStartRegister > -1 )
		registers[mRotStartRegister] = ZLFloat::Rand (mRotStart-mRotStartVariance, mRotStart+mRotStartVariance);
	

	if(mRotEndRegister > -1)
		registers[mRotStartRegister] = ZLFloat::Rand (mRotEnd-mRotEndVariance, mRotEnd+mRotEndVariance);

	float angleStartDeg;
	// have to compute the angle first, because the computed DX and DY may be used by later calculations.
	if(mEmitterType == EMITTER_GRAVITY)
	{
		// for gravity emitters, use mAngle as a modifier on the angle computed from dy and dx.
		angleStartDeg = (atan2f ( particle[MOAIParticle::PARTICLE_DY], particle[MOAIParticle::PARTICLE_DX] ) * R2D ) + mAngle;
	}
	else
	{
		angleStartDeg = mAngle;
	}

	if (mAngleRegister > -1)
	{
		angleStartDeg += ZLFloat::Rand (- mAngleVariance, + mAngleVariance );
	}
	particle[MOAIParticle::PARTICLE_DX] = Cos(angleStartDeg * (float)D2R);                  
	particle[MOAIParticle::PARTICLE_DY] = Sin(angleStartDeg * (float)D2R);

	if (mEmitterType == EMITTER_GRAVITY)
	{
		// Set initial speed
		if (mSpeedRegister > -1)
		{
			registers[mSpeedRegister] = ZLFloat::Rand(mSpeed - mSpeedVariance, mSpeed + mSpeedVariance);
			registers[mDirectionXRegister] = particle[MOAIParticle::PARTICLE_DX] * registers[mSpeedRegister];
			registers[mDirectionYRegister] = particle[MOAIParticle::PARTICLE_DY] * registers[mSpeedRegister];
		}
		else
		{
			registers[mDirectionXRegister] = particle[MOAIParticle::PARTICLE_DX] * mSpeed;
			registers[mDirectionYRegister] = particle[MOAIParticle::PARTICLE_DY] * mSpeed;
		}
		if (mRadialAccelRegister > -1)
			registers[mRadialAccelRegister] = ZLFloat::Rand(mRadialAcceleration - mRadialAccelVariance, mRadialAcceleration + mRadialAccelVariance);


		if (mTanAccelRegister > -1)
			registers[mTanAccelRegister] = ZLFloat::Rand(mTanAccel - mTanAccelVariance, mTanAccel + mTanAccelVariance);
	}
	else
	{
		if (mRotPerSecondVariance != 0)
		{
			float randVal =  ZLFloat::Rand (mRotPerSecond - mRotPerSecondVariance, mRotPerSecond + mRotPerSecondVariance);
			registers[mRotPerSecondRegister] = randVal;		
		} else {
			registers[mRotPerSecondRegister] = mRotPerSecond;
		}
		
		if (mMaxRadiusRegister > -1 )
		{
			registers[mMaxRadiusRegister] = ZLFloat::Rand (mMaxRadius - mMaxRadiusVariance, mMaxRadius + mMaxRadiusVariance);
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

	// pick a slightly different source position, based on mSourcePos
	registers[mStartXRegister] = particle[MOAIParticle::PARTICLE_X] + mSourcePos[0] + ZLFloat::Rand(- mSourcePosVariance[0], + mSourcePosVariance[0]);
	registers[mStartYRegister] = particle[MOAIParticle::PARTICLE_Y] + mSourcePos[1] + ZLFloat::Rand(- mSourcePosVariance[1], + mSourcePosVariance[1]);

	particle[MOAIParticle::PARTICLE_X] = registers[mStartXRegister];
	particle[MOAIParticle::PARTICLE_Y] = registers[mStartYRegister];
}

void  MOAIParticlePexPlugin::OnRender ( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1,  float term)
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
			sprite->mRed = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 1:
			sprite->mGreen = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 2:
			sprite->mBlue = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
			break;
		case 3:
			sprite->mAlpha = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
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
		sprite->mXScl = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
		sprite->mYScl = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
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
		sprite->mZRot = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, eVal, t1 );
	else
		sprite->mZRot = sVal;

	if (mEmitterType == EMITTER_GRAVITY)
	{
		float moveX = 0;
		float moveY = 0;
		
		float dx = particle[MOAIParticle::PARTICLE_X] - registers[mStartXRegister];
		float dy = particle[MOAIParticle::PARTICLE_Y] - registers[mStartYRegister];

		if(mRadialAcceleration != 0 || mRadialAccelVariance != 0 || 
			mTanAccel != 0 || mTanAccelVariance != 0)
		{
			float xVal, yVal;
			float denom = Sqrt( (dx * dx) + (dy * dy) );

			// Set radial acceleration
			if( mRadialAcceleration != 0 || mRadialAccelVariance != 0)
			{
				if(denom)
				{
					xVal = (float)( dx / denom );
					yVal = (float)( dy / denom );
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
					xVal = (float)( dx / denom );
					yVal = (float)( dy / denom );
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
		
		particle[ MOAIParticle::PARTICLE_X ] += moveX;
		particle[ MOAIParticle::PARTICLE_Y ] += moveY;
	}
	else
	{
		registers[mRadialRegister] += registers[mRotPerSecondRegister] * ((t1 - t0)*term);

		if(mMaxRadiusRegister > -1)
			sVal = registers[mMaxRadiusRegister];
		else
			sVal = mMaxRadius;
		float magVal = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, sVal, mMinRadius, t1 );

		particle[MOAIParticle::PARTICLE_X] = registers[mStartXRegister] - Cos(registers[mRadialRegister] * (float)D2R) * magVal;
	    particle[MOAIParticle::PARTICLE_Y] = registers[mStartYRegister] - Sin(registers[mRadialRegister] * (float)D2R) * magVal;
	}

	sprite->mXLoc = particle[ MOAIParticle::PARTICLE_X ];
	sprite->mYLoc = particle[ MOAIParticle::PARTICLE_Y ];

	sprite->mGfxID = 1;
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
	mSourcePos[0] = mSourcePos[1] = 0;
	mSourcePosVariance[0] = mSourcePosVariance[1] = 0;

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
