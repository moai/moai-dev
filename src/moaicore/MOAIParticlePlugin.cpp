// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <tinyxml.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleTimedEmitter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Return the particle size expected by the plugin.
	
	@in		MOAIParticlePlugin self
	@out	number size
*/
int MOAIParticlePlugin::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticlePlugin, "U" )
	
	lua_pushnumber ( state, self->mSize );
	
	return 1;
}

/**	@name	loadExternal
	@text	Create a particle plugin from an XML file
	
	@in		String file to load
	@out	MOAIParticleSystem system
	@out	MOAITParticleTimedEmitter emitter
	@out	String	texture file associated with particle system
*/
int MOAIParticlePlugin::_loadExternal( lua_State* L ){

	//MOAI_LUA_SETUP ( MOAIParticlePlugin, "S" )
	MOAILuaState state ( L );										
	if ( !state.CheckParams ( 1, "S" )) {							
		MOAILog ( L, MOAILogMessages::MOAI_ParamTypeMismatch );		
		return 0;													
	}																
		
	cc8* xml = lua_tostring ( state, 1 );
	if ( MOAILogMessages::CheckFileExists ( xml, L )) {
		TiXmlDocument doc;
		doc.LoadFile ( xml );
		MOAIParticlePlugin::Parse ( state, doc.RootElement ());
		return 3;
	}
	return 0;
}
//================================================================//
// MOAIParticlePlugin
//================================================================//

//----------------------------------------------------------------//
void MOAIParticlePlugin::Init ( AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size ) {

	this->mInitFunc		= initFunc;
	this->mRenderFunc	= renderFunc;
	this->mSize			= size;
}

void MOAIParticlePlugin::Parse( MOAILuaState& state, TiXmlNode* node ) 
{
	if ( !node ) return;
	
	TiXmlElement* element = node->ToElement();

	if ( element && strcmp(element->Value (), "patricleEmitterConfig") ) {
		
		MOAIParticleSystem* system = new MOAIParticleSystem();
		MOAIParticleState*	particleState = new MOAIParticleState();
		MOAIParticleScript* initScript = new MOAIParticleScript();
		MOAIParticleScript* renderScript = new MOAIParticleScript();
		MOAIParticleTimedEmitter *emitter = new MOAIParticleTimedEmitter();
		int numParticles;
		float lifespan, lifespanVariance;
		float angle, angleVariance;
		float startColor[4], startColorVariance[4];
		float finishColor[4], finishColorVariance[4];
		float startSize, startSizeVariance;
		float finishSize, finishSizeVariance;
		float gravity[2];
		float maxRadius, maxRadiusVariance;
		float minRadius;
		float radialAcceleration, radialAccelVariance;
		float tanAccel, tanAccelVariance;
		float rotStart, rotStartVariance;
		float rotEnd, rotEndVariance;
		float speed, speedVariance;
		float rotPerSecond, rotPerSecondVariance;
		float sourcePosVariance[2];
		float duration;
		int curRegister = 0;
		u32 blendFuncSrc, blendFuncDst;
		cc8 *textureName;

		EmitterType emitterType;

		// Constant registers
		const u64 PARTICLE_X = MOAIParticleScript::Pack64( MOAIParticle::PARTICLE_X, MOAIParticleScript::PARAM_TYPE_PARTICLE_REG );
		const u64 PARTICLE_Y =  MOAIParticleScript::Pack64( MOAIParticle::PARTICLE_Y, MOAIParticleScript::PARAM_TYPE_PARTICLE_REG );
		const u64 PARTICLE_DX = MOAIParticleScript::Pack64( MOAIParticle::PARTICLE_DX, MOAIParticleScript::PARAM_TYPE_PARTICLE_REG );
		const u64 PARTICLE_DY =  MOAIParticleScript::Pack64( MOAIParticle::PARTICLE_DY, MOAIParticleScript::PARAM_TYPE_PARTICLE_REG );
		const u64 SPRITE_X_SCL =  MOAIParticleScript::Pack64 ( MOAIParticleScript::SPRITE_X_SCL, MOAIParticleScript::PARAM_TYPE_SPRITE_REG );
		const u64 SPRITE_Y_SCL =  MOAIParticleScript::Pack64 ( MOAIParticleScript::SPRITE_Y_SCL, MOAIParticleScript::PARAM_TYPE_SPRITE_REG );
		const u64 SPRITE_ROT = MOAIParticleScript::Pack64 ( MOAIParticleScript::SPRITE_ROT, MOAIParticleScript::PARAM_TYPE_SPRITE_REG );
		
		// Variable registers
		u64 regList[32];
		for(int i = 0; i < 32; i++)
		{
			u8 val = i + MOAIParticle::TOTAL_PARTICLE_REG;
			regList[i] = MOAIParticleScript::Pack64 ( val, MOAIParticleScript::PARAM_TYPE_PARTICLE_REG );
		}

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
				angle = (float)atof(attribute->Value());
			else if(text == "angleVariance")
				angleVariance = (float)atof(attribute->Value());
			else if(text == "blendFuncSource")
				blendFuncSrc = atoi(attribute->Value());
			else if(text == "blendFuncDestination")
				blendFuncDst = atoi(attribute->Value());
			else if(text == "duration")
				duration = (float)atof(attribute->Value());
			else if(text == "emitterType")
				emitterType = (EmitterType)atoi(attribute->Value());
			else if(text == "finishColor")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					finishColor[i] = (float)atof(attribute->Value());
				}
			else if(text == "finishColorVariance")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					finishColorVariance[i] = (float)atof(attribute->Value());
				}
			else if(text == "finishParticleSize")
				finishSize = (float)atof(attribute->Value());
			else if(text == "FinishParticleSizeVariance")
				finishSizeVariance = (float)atof(attribute->Value());
			else if(text == "gravity")
				for ( ; attribute; attribute = attribute->Next (), i++) {
					gravity[i] = (float)atof(attribute->Value());
				}
			else if(text == "maxParticles")
				numParticles = atoi(attribute->Value());
			else if(text == "maxRadius")
				maxRadius = (float)atof(attribute->Value());
			else if(text == "maxRadiusVariance")
				maxRadiusVariance = (float)atof(attribute->Value());
			else if(text == "minRadius")
				minRadius = (float)atof(attribute->Value());
			else if(text == "particleLifeSpan")
				lifespan = (float)atof(attribute->Value());
			else if(text == "particleLifespanVariance")
				lifespanVariance = (float)atof(attribute->Value());
			else if(text == "radialAcceleration")
				radialAcceleration = (float)atof(attribute->Value());
			else if(text == "radialAccelVariance")
				radialAccelVariance = (float)atof(attribute->Value());
			else if(text == "rotatePerSecond")
				rotPerSecond = (float)atof(attribute->Value());
			else if(text == "rotatePerSecondVariance")
				rotPerSecondVariance = (float)atof(attribute->Value());
			else if(text == "rotationEnd")
				rotEnd = (float)atof(attribute->Value());
			else if(text == "rotationEndVariance")
				rotEndVariance = (float)atof(attribute->Value());
			else if(text == "rotationStart")
				rotStart = (float)atof(attribute->Value());
			else if(text == "rotationStartVariance")
				rotStartVariance = (float)atof(attribute->Value());
			else if(text == "sourcePosition")
				for ( ; attribute; attribute = attribute->Next (), i++) {
						sourcePosVariance[i] = (float)atof(attribute->Value());
					}
			else if(text == "sourcePositionVariance")
				rotPerSecondVariance = (float)atof(attribute->Value());
			else if(text == "speed")
				speed = (float)atof(attribute->Value());
			else if(text == "speedVariance")
				speedVariance = (float)atof(attribute->Value());
			else if(text == "startColor")
					for ( ; attribute; attribute = attribute->Next (), i++) {
						startColor[i] = (float)atof(attribute->Value());
					}
			else if(text == "startColorVariance")
					for ( ; attribute; attribute = attribute->Next (), i++) {
						startColorVariance[i] = (float)atof(attribute->Value());
					}
			else if(text == "startParticleSize")
				startSize = (float)atof(attribute->Value());
			else if(text == "startParticleSizeVariance")
				startSizeVariance = (float)atof(attribute->Value());
			else if(text == "tangentialAcceleration")
				tanAccel = (float)atof(attribute->Value());
			else if(text == "tangentialAccelVariance")
				tanAccelVariance = (float)atof(attribute->Value());
			else if(text == "texture")
				textureName = attribute->Value();
			
		}
		
		
		
		// Push sprite
		_sprite(renderScript);
		

		// Set colors.
		for(int i = 0; i < 4; i++)
		{
			u64 startVal, finishVal;
			if(startColorVariance[i] && startColorVariance[i] != 0)
			{
				float minVal = startColor[i] - startColorVariance[i] < 0 ? 0 : startColor[i] - startColorVariance[i];
				_rand(initScript, regList[curRegister], _packConst(minVal), _packConst(startColor[i] + startColorVariance[i]));
				startVal = regList[curRegister];
				curRegister++;
			}	
			else
				startVal = _packConst(startColor[i]);
			
			if(finishColorVariance[i] && finishColorVariance[i] != 0 )
			{
				float minVal = finishColor[i] - finishColorVariance[i] < 0 ? 0 : finishColor[i] - finishColorVariance[i];
				_rand(initScript, regList[curRegister], _packConst(minVal), _packConst(finishColor[i] + finishColorVariance[i]));
				finishVal = regList[curRegister];
				curRegister++;
			}
			else
				finishVal = _packConst( finishColor[i] );

			_ease(renderScript, MOAIParticleScript::Pack64 ( MOAIParticleScript::SPRITE_RED + i, MOAIParticleScript::PARAM_TYPE_SPRITE_REG ), startVal, finishVal, USInterpolate::kLinear);
		}


		// Set start and ending scales
		u64 sizeStartVal, sizeEndVal;
		if(startSizeVariance != 0)
		{
			float minVal = startSize - startSizeVariance < 0 ? 0 :  startSize - startSizeVariance;
			_rand(initScript, regList[curRegister], _packConst(minVal), _packConst(startSize+ startSizeVariance));
			sizeStartVal = regList[curRegister];
			curRegister++;
		}
		else
			sizeStartVal = _packConst(startSize);
			
		if(finishSizeVariance != 0)
		{
			float minVal = finishSize - finishSizeVariance < 0 ? 0 :  finishSize - finishSizeVariance;
			_rand(initScript, regList[curRegister], _packConst(minVal), _packConst (finishSize + finishSizeVariance));
			sizeEndVal = regList[curRegister];
			curRegister++;
		}
		else
			sizeEndVal = _packConst(finishSize);

		_set(initScript, SPRITE_X_SCL, sizeStartVal);
		_set(initScript, SPRITE_Y_SCL, sizeStartVal);
		if(sizeStartVal != sizeEndVal)
		{
			
			_ease(renderScript, SPRITE_X_SCL, sizeStartVal, sizeEndVal, USInterpolate::kLinear);
			_ease(renderScript, SPRITE_Y_SCL, sizeStartVal, sizeEndVal, USInterpolate::kLinear);
		}


		// Set start and end rotational values
		u64 rotStartVal, rotEndVal;
		if(rotStartVariance != 0 )
		{
			_rand(initScript, regList[curRegister],
				_packConst(rotStart-rotStartVariance),
				_packConst(rotStart+rotStartVariance));
			rotStartVal = regList[curRegister];
			curRegister++;
		}
		else
			rotStartVal = _packConst(rotStart);

		if(rotEndVariance != 0)
		{
			_rand(initScript, regList[curRegister],
				_packConst(rotEnd-rotEndVariance),
				_packConst(rotEnd+rotEndVariance));
			rotEndVal = regList[curRegister];
			curRegister++;
		}
		else
			rotEndVal = _packConst(rotEnd);
		
		_set(initScript, SPRITE_ROT, rotStartVal);
		if(rotStartVal != rotEndVal)
			_ease(renderScript, SPRITE_ROT, rotStartVal,rotEndVal, USInterpolate::kLinear);

		// Set angle to emit particles
		u64 angleStartValX, angleStartValY, angleStartDeg;
		if(angleVariance != 0)
		{
			_rand( initScript, regList[curRegister + 2], _packConst(angle - angleVariance), _packConst(angle + angleVariance));
			_angleVec( initScript, regList[curRegister], regList[curRegister + 1], regList[curRegister + 2]);
			angleStartValX = regList[curRegister];
			angleStartValY = regList[curRegister + 1];
			angleStartDeg = regList[curRegister + 2];
			curRegister += 3;
		}
		else
		{
			angleStartValX = _packConst(Cos(angle * (float)D2R));
			angleStartValY = _packConst(Sin(angle * (float)D2R));
			angleStartDeg = _packConst(angle);
		}
		_set( initScript, PARTICLE_DX, angleStartValX );
		_set( initScript, PARTICLE_DY, angleStartValY );

		// Initialize Gravity or Radial specific information.
		if(emitterType == EMITTER_GRAVITY)
		{ 
			u64 movementXVal = regList[curRegister];
			u64 movementYVal = regList[curRegister+1];
			u64 startXVal = regList[curRegister+2];
			u64 startYVal = regList[curRegister+3];
			u64 directionXVal = regList[curRegister+4];
			u64 directionYVal = regList[curRegister+5];
			u64 timeVal = regList[curRegister+6];

			curRegister+= 7;


			u64 speedVal;
			// Set initial speed
			if(speedVariance != 0)
			{
				speedVal = regList[curRegister];
				_rand(initScript, speedVal, _packConst(speed - speedVariance), _packConst(speed + speedVariance));
				curRegister++;
			}
			else
				speedVal = _packConst(speed);

			_mul(initScript, directionXVal, PARTICLE_DX, speedVal);
			_mul(initScript, directionYVal, PARTICLE_DY, speedVal);

			_set(initScript, startXVal, PARTICLE_X);
			_set(initScript, startYVal, PARTICLE_Y);
			_set(renderScript, movementXVal, _packConst(0));
			_set(renderScript, movementYVal, movementXVal);
			
			

			_sub(renderScript, PARTICLE_X, PARTICLE_X, startXVal);
			_sub(renderScript, PARTICLE_Y, PARTICLE_Y, startYVal);
		
			// Set radial acceleration
			if( radialAcceleration != 0 || radialAccelVariance != 0)
			{
				u64 radialXVal = regList[curRegister];
				u64 radialYVal = regList[curRegister+1];
				u64 radAccelVal;
				curRegister+=2;

				_norm(renderScript, radialXVal, radialYVal, PARTICLE_X, PARTICLE_Y);
				if( radialAccelVariance != 0)
				{
					radAccelVal = regList[curRegister];
					_rand(initScript, radAccelVal, _packConst(radialAcceleration - radialAccelVariance), 
												_packConst(radialAcceleration + radialAccelVariance));
					curRegister++;
				}
				else
					radAccelVal = _packConst(radialAcceleration);

				_mul(renderScript, radialXVal, radialXVal, radAccelVal);
				_mul(renderScript, radialYVal, radialYVal, radAccelVal);
				_add(renderScript, movementXVal, movementXVal, radialXVal);
				_add(renderScript, movementYVal, movementYVal, radialYVal);
			}

			// Set tangential acceleration
			if(tanAccel != 0 || tanAccelVariance != 0)
			{
				u64 tanXVal, tanYVal;
				u64 tanAccelVal;
				tanXVal = regList[curRegister];
				tanYVal = regList[curRegister+1];
				curRegister+=2;

				_norm(renderScript, tanYVal, tanXVal, PARTICLE_X, PARTICLE_Y);

				if( tanAccelVariance != 0 )
				{
					tanAccelVal = regList[curRegister];
					_rand(initScript, tanAccelVal, _packConst(tanAccel - tanAccelVariance), 
													_packConst(tanAccel + tanAccelVariance));
					curRegister++;
				}
				else
					tanAccelVal = _packConst(tanAccel);

				_mul( renderScript, tanXVal, _packConst(-1.0f), tanXVal);
				_mul( renderScript, tanXVal, tanXVal, tanAccelVal);
				_mul( renderScript, tanYVal, tanYVal, tanAccelVal);
				_add(renderScript, movementXVal, movementXVal, tanXVal);
				_add(renderScript, movementYVal, movementYVal, tanYVal);
			}
			
			// Set Gravity
			if(gravity[0] != 0 )
				_add(renderScript, movementXVal, movementXVal, _packConst(gravity[0]));

			if(gravity[1] != 0 )
				_add(renderScript, movementYVal, movementYVal, _packConst(gravity[1]));

			_timeDelta(renderScript, timeVal);
			_mul(renderScript, movementXVal, movementXVal, timeVal);
			_mul(renderScript, movementYVal, movementYVal, timeVal);

			_add(renderScript, directionXVal, directionXVal, movementXVal);
			_add(renderScript, directionYVal, directionYVal, movementYVal);

			_mul(renderScript, movementXVal, directionXVal, timeVal);
			_mul(renderScript, movementYVal, directionYVal, timeVal);

			_add(renderScript, PARTICLE_X, PARTICLE_X, movementXVal);
			_add(renderScript, PARTICLE_Y, PARTICLE_Y, movementYVal);

			_add(renderScript, PARTICLE_X, PARTICLE_X, startXVal);
			_add(renderScript, PARTICLE_Y, PARTICLE_Y, startYVal);	
				
		}
		else // Radial Emitter
		{
			u64 maxRadiusVal, rotPerSecondVal;

			rotPerSecondVal = regList[curRegister];
			u64 radialReg = regList[curRegister+1];
			u64 magReg = regList[curRegister + 2];
			curRegister += 3;

			
			
			if( maxRadiusVariance != 0 )
			{
				maxRadiusVal = regList[curRegister];
				_rand(initScript, maxRadiusVal, _packConst(maxRadius - maxRadiusVariance), _packConst(maxRadius + maxRadiusVariance));
				curRegister++;
			}
			else
				maxRadiusVal = _packConst(maxRadius);
					
			
			if( rotPerSecondVariance != 0 )
			{
				
				_rand(initScript, rotPerSecondVal, _packConst(rotPerSecond - rotPerSecondVariance), _packConst(rotPerSecond + rotPerSecondVariance));
				_add(initScript, rotPerSecondVal, rotPerSecondVal, angleStartDeg);
				
			}
			else
				_add(initScript, rotPerSecondVal, _packConst(rotPerSecond), angleStartDeg);
			
			
			_set(initScript, radialReg, angleStartDeg);
			_easeDelta(renderScript, radialReg, radialReg, rotPerSecondVal, USInterpolate::kLinear);
			_ease(renderScript, magReg, maxRadiusVal, _packConst(minRadius), USInterpolate::kLinear);
			
			_angleVec(renderScript, PARTICLE_X, PARTICLE_Y, radialReg);
			_mul(renderScript, PARTICLE_X, PARTICLE_X, magReg);
			_mul(renderScript, PARTICLE_Y, PARTICLE_Y, magReg);
			

			_angleVec(initScript,  PARTICLE_X, PARTICLE_Y, radialReg);
			_mul(initScript, PARTICLE_X, PARTICLE_X, maxRadiusVal);
			_mul(initScript, PARTICLE_Y, PARTICLE_Y, maxRadiusVal);
			
				
		}

		// Initialize particle system
		system->ReserveParticles(numParticles, curRegister);
		system->ReserveSprites(numParticles);
		system->mBlendMode.SetBlend ( blendFuncSrc, blendFuncDst+1 );

		// Finialize the state with a lifespan & compile scripts
		particleState->mTermRange[0] = lifespan - lifespanVariance < 0 ? 0 : lifespan - lifespanVariance;
		particleState->mTermRange[1] = lifespan + lifespanVariance;
		
		initScript->Compile();
		renderScript->Compile();
		particleState->mInit.Set(*particleState, initScript);
		particleState->mRender.Set(*particleState, renderScript);

		system->ReserveStates( 1 );
		system->mStates [ 0 ] = particleState;
		system->PushLuaUserdata( state );


		// Set up emitter information and push.
		float emissionRate =  1.0f / (numParticles / lifespan );
		u32 emissionCount = 1;
		if( emissionRate <  0.05 )
		{
			emissionCount = (u32)ceil( 0.05 / emissionRate );
			emissionRate *= emissionCount;
		}
		emitter->SetSystem(system);
		emitter->SetEmissionRange ( emissionCount, emissionCount );
		emitter->SetFrequencyRange ( emissionRate, emissionRate );

		emitter->PushLuaUserdata( state );

		// TODO: The XML exports particle size in pixels while
		//		 the system does so in scale. Since we want
		//		 to cache the texture, we'll have to set the
		//		 deck's rext to 1x1 and have the particle script
		//		 scale.
		lua_pushstring ( state, textureName );
		
	}
}
void MOAIParticlePlugin::SetRegister( MOAIParticleScript::Instruction &instruction, u32 idx, u64 reg)
{
	
	u8 type;
	instruction.mSize += sizeof ( u8 );
	instruction.mSize += sizeof ( u8 );
					
	type = ( reg >> 32 ) & MOAIParticleScript::PARAM_TYPE_MASK;

	if ( !( type & MOAIParticleScript::PARAM_TYPE_REG_MASK )) {
					
		instruction.mTypes [ idx ] = MOAIParticleScript::PARAM_TYPE_CONST; // force load of 0 on decode
		instruction.mParams [ idx ] = 0;
	}
	else {
					
		instruction.mTypes [ idx ] = type;
		instruction.mParams [ idx ] = ( u32 )( reg & 0xff );
	}
}
void MOAIParticlePlugin::SetValue( MOAIParticleScript::Instruction &instruction, u32 idx, u64 value)
{
	u8 type;

	instruction.mSize += sizeof ( u8 );
					
					
	type = ( value >> 32 ) & MOAIParticleScript::PARAM_TYPE_MASK;
	instruction.mTypes [ idx ] = type;
					
	if ( type & MOAIParticleScript::PARAM_TYPE_REG_MASK ) {
						
		instruction.mParams [ idx ] = ( u32 )( value & 0xff );
		instruction.mSize += sizeof ( u8 );
	}
	else if ( type == MOAIParticleScript::PARAM_TYPE_CONST ) {
					
		instruction.mParams [ idx ] = ( u32 )value;
		instruction.mSize += sizeof ( u32 );
	}
	else {
						
		// unrecognized; treat as const
		// TODO: issue warning
		instruction.mTypes [ idx ] = MOAIParticleScript::PARAM_TYPE_CONST;
		instruction.mParams [ idx ] = 0;
		instruction.mSize += sizeof ( u32 );
	}
}
void MOAIParticlePlugin::SetEase( MOAIParticleScript::Instruction &instruction, u32 idx, u32 easeType)
{
	instruction.mSize += sizeof ( u32 );		
	instruction.mParams [ idx ] = easeType;
	instruction.mTypes [ idx ] = MOAIParticleScript::PARAM_TYPE_FLAG;
}
void MOAIParticlePlugin::_add( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::ADD, "RVV");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);
}
void MOAIParticlePlugin::_cycle( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u64 val2){}
void MOAIParticlePlugin::_div( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1){}
void MOAIParticlePlugin::_ease( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u32 ease)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::EASE, "RVVI");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);
	SetEase(instr, 3, ease);
}
void MOAIParticlePlugin::_easeDelta( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u32 ease)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::EASE_DELTA, "RVVI");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);
	SetEase(instr, 3, ease);
}
void MOAIParticlePlugin::_mul( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 )
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::MUL, "RVV");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);
}
void MOAIParticlePlugin::_norm( MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val0, u64 val1 )
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::NORM, "RRVV");
	SetRegister(instr, 0, reg0);
	SetRegister(instr, 1, reg1);
	SetValue(instr, 2, val0);
	SetValue(instr, 3, val1);
}
void MOAIParticlePlugin::_rand( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 )
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::RAND, "RVV");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);
}
void MOAIParticlePlugin::_randVec(MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val0, u64 val1){}
void MOAIParticlePlugin::_set(MOAIParticleScript* script, u64 reg, u64 val0)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::SET, "RV");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
}
void MOAIParticlePlugin::_sprite(MOAIParticleScript* script)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::SPRITE, "");
}
void MOAIParticlePlugin::_sub( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1 )
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::SUB, "RVV");
	SetRegister(instr, 0, reg);
	SetValue(instr, 1, val0);
	SetValue(instr, 2, val1);

}
void MOAIParticlePlugin::_time( MOAIParticleScript* script, u64 reg)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::TIME, "R");
	SetRegister(instr, 0, reg);
}
void MOAIParticlePlugin::_timeDelta( MOAIParticleScript* script, u64 reg)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::TIME_DELTA, "R");
	SetRegister(instr, 0, reg);
}
void MOAIParticlePlugin::_vecAngle( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1){}
void MOAIParticlePlugin::_angleVec(MOAIParticleScript* script, u64 reg0, u64 reg1, u64 val)
{
	MOAIParticleScript::Instruction &instr = script->PushInstruction(MOAIParticleScript::ANGLE_VEC, "RRV");
	SetRegister( instr, 0, reg0);
	SetRegister( instr, 1, reg1);
	SetValue( instr, 2, val);
}
void MOAIParticlePlugin::_wrap( MOAIParticleScript* script, u64 reg, u64 val0, u64 val1, u64 val2){}

u64	MOAIParticlePlugin::_packConst( float val )
{	
	u32 bits;
	memcpy ( &bits, &val, sizeof ( u32 ));
	return MOAIParticleScript::Pack64 ( bits, MOAIParticleScript::PARAM_TYPE_CONST );
}
//----------------------------------------------------------------//
MOAIParticlePlugin::MOAIParticlePlugin () :
	mSize ( 0 ),
	mInitFunc ( 0 ),
	mRenderFunc ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIParticlePlugin::~MOAIParticlePlugin () {
}

//----------------------------------------------------------------//
void MOAIParticlePlugin::RegisterLuaClass ( MOAILuaState& state ) {
	//UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "loadExternal", _loadExternal },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticlePlugin::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "getSize",			_getSize },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

