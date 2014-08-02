//
//  MOAICCParticleSystem.cpp
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#include "pch.h"
#include <tinyxml.h>

#include <moaicore/MOAICCParticleSystem.h>

int MOAICCParticleSystem::_initializeProperties( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	
	
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

int MOAICCParticleSystem::_start ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->StartSystem();
	return 0;
}

int MOAICCParticleSystem::_stop ( lua_State *L ) {
	
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->StopSystem();
	
	return 0;
}

int MOAICCParticleSystem::_reset ( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->ResetSystem();
	return 0;
}

bool MOAICCParticleSystem::AddParticle(){
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
}

void MOAICCParticleSystem::InitParticle ( MOAICCParticle *particle ) {
	UNUSED(particle);
}


bool MOAICCParticleSystem::IsFull(){
	return (this->mParticleCount == this->mTotalParticles);
}

MOAICCParticleSystem::MOAICCParticleSystem () {
	RTTI_BEGIN
		RTTI_EXTEND( MOAIProp )
	RTTI_END
}

MOAICCParticleSystem::~MOAICCParticleSystem	() {
	
}

void MOAICCParticleSystem::OnUpdate ( float step ) {
	UNUSED(step);
	
	if (this->mActive && this->mEmissionRate) {
		float rate = 1.0f / this->mEmissionRate;
		
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
					tmp.Init(this->mGravity[0], this->mGravity[1]);
					tmp.Add(radialVector);
					tmp.Add(tangentialVector);
					
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

void MOAICCParticleSystem::ParseXML( cc8 *filename, TiXmlNode *node ){
	UNUSED(filename);
	UNUSED(node);
}

void MOAICCParticleSystem::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIProp::RegisterLuaClass ( state );
}

void MOAICCParticleSystem::RegisterLuaFuncs( MOAILuaState &state ) {
	MOAIProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "initializeProperties",			_initializeProperties },
		{ "load",							_load },
		{ "reset",							_reset },
		{ "start",							_start },
		{ "stop",							_stop },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAICCParticleSystem::ResetSystem () {
	this->mActive = true;
	this->mElapsed = 0.0f;
	for (int i = 0; i < (int)this->mParticleCount; ++i) {
		MOAICCParticle *p = &(this->mParticles[i]);
		p->mTimeToLive = 0;
	}
}

void MOAICCParticleSystem::SetVisible(bool visible) {
	MOAIProp::SetVisible(visible);
}

void MOAICCParticleSystem::StartSystem () {
	this->mActive = true;
}


void MOAICCParticleSystem::StopSystem () {
	this->mActive = false;
	this->mElapsed = this->mDuration;
	this->mEmitCounter = 0.0f;
}
