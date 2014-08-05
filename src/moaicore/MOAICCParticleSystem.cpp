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

//----------------------------------------------------------------//
int MOAICCParticleSystem::_setTotalParticles( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "UN" )
	u32 total = state.GetValue < u32 >( 2, 1 );
	
	
	
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
	// TODO: implement relative type for particle position
	// free particle position
	{
		USVec3D loc;
		loc.Init(0.0f, 0.0f, 0.0f);
		USAffine3D modelToWorld = this->GetLocalToWorldMtx ();
		modelToWorld.Transform(loc);
		particle->mStartPosition.Init(loc.mX, loc.mY);
	}
	
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
}

void MOAICCParticleSystem::InitializeEmitter () {
	this->mEmissionRate = 1.0f / (this->mTotalParticles);
	
	float minLifespan = this->mLifespan - this->mLifespanVariance;
	if ( minLifespan < 0.0f ) {
		minLifespan = 0.0f;
	}
	
	this->mLifespanTerm[0] = minLifespan;
	
	float maxLifespan = this->mLifespan + this->mLifespanVariance;
	this->mLifespanTerm[1] = maxLifespan;
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
	mActive( true )
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
					tmp.Init(p->mGravity);
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
			else if (text == "duration") {
				this->mDuration = (float)atof(attribute->Value());
			}
			else if (text == "emitterType") {
				this->mEmitterType = (EmitterType)atoi(attribute->Value());
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
			else if (text == "maxParticles") {
				this->mTotalParticles = atoi(attribute->Value());
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
}

void MOAICCParticleSystem::RegisterLuaFuncs( MOAILuaState &state ) {
	MOAIProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "initializeProperties",			_initializeProperties },
		{ "load",							_load },
		{ "reset",							_reset },
		{ "setTotalParticles",				_setTotalParticles },
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

void MOAICCParticleSystem::SetTotalParticles ( u32 numberOfParticles ){
	this->mTotalParticles = numberOfParticles;
	
	if (!this->mParticles ||  numberOfParticles > this->mAllocatedParticles) {
		// allocate new memory
		size_t particlesize = numberOfParticles * sizeof( MOAICCParticle );
		
		this->mParticles = (MOAICCParticle *)realloc(this->mParticles, particlesize);
		bzero(this->mParticles, particlesize);
		
		this->mAllocatedParticles = numberOfParticles;
	}
	
	this->mTotalParticles = numberOfParticles;
	this->ResetSystem();
	
}


void MOAICCParticleSystem::SetVisible ( bool visible ) {
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
