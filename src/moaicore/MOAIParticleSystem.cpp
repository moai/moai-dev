// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <float.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAITexture.h>

class MOAIDataBuffer;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	capParticles
	@text	Controls capping vs. wrapping of particles in overflow situation.
			Capping will prevent emission of additional particles when system
			is full. Wrapping will overwrite the oldest particles with new particles.
	
	@in		MOAIParticleSystem self
	@opt	boolean cap Default value is true.
	@out	nil
*/
int MOAIParticleSystem::_capParticles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "U" )

	self->mCapParticles = state.GetValue < bool >( 2, true );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	capSprites
	@text	Controls capping vs. wrapping of sprites.
	
	@in		MOAIParticleSystem self
	@opt	boolean cap Default value is true.
	@out	nil
*/
int MOAIParticleSystem::_capSprites ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "U" )

	self->mCapSprites = state.GetValue < bool >( 2, true );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clearSprites
	@text	Flushes any existing sprites in system.
	
	@in		MOAIParticleSystem self
	@out	nil
*/
int MOAIParticleSystem::_clearSprites ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "U" )

	self->mSpriteTop = 0;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getState
	@text	Returns a particle state for an index or nil if none exists.
	
	@in		MOAIParticleSystem self
	@out	MOAIParticleState state
*/
int MOAIParticleSystem::_getState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAIParticleState* particleState = self->GetState ( idx );
	if ( particleState ) {
		particleState->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pushParticle
	@text	Adds a particle to the system.
	
	@in		MOAIParticleSystem self
	@opt	number x Default value is 0.
	@opt	number y Default value is 0.
	@opt	number dx Default value is 0.
	@opt	number dy Default value is 0.
	@out	boolean result 'true' is particle was added, 'false' if not.
*/
int MOAIParticleSystem::_pushParticle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "U" )

	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );
	
	float dx = state.GetValue < float >( 4, 0.0f );
	float dy = state.GetValue < float >( 5, 0.0f );

	bool result = self->PushParticle ( x, y, dx, dy );
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	pushSprite
	@text	Adds a sprite to the system. Sprite will persist until
			particle simulation is begun or 'clearSprites' is called.
	
	@in		MOAIParticleSystem self
	@in		number x
	@in		number y
	@opt	number rot Rotation in degrees. Default value is 0.
	@opt	number xScale Default value is 1.
	@opt	number yScale Default value is 1.
	@out	boolean result 'true' is sprite was added, 'false' if not.
*/
int MOAIParticleSystem::_pushSprite ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UNN" )

	MOAIParticleSprite sprite;

	sprite.mLoc.mX		= state.GetValue < float >( 2, 0.0f );
	sprite.mLoc.mY		= state.GetValue < float >( 3, 0.0f );
	sprite.mRot			= state.GetValue < float >( 4, 0.0f );
	sprite.mScl.mX		= state.GetValue < float >( 5, 1.0f );
	sprite.mScl.mY		= state.GetValue < float >( 6, 1.0f );
	
	sprite.mColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	sprite.mGfxID = 1;
	
	bool result = self->PushSprite ( sprite );
	
	if ( result ) {
		self->ScheduleUpdate ();
	}
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	reserveParticles
	@text	Reserve particle capacity of system.
	
	@in		MOAIParticleSystem self
	@in		number nParticles		Total number of particle records.
	@in		number particleSize		Number of params reserved for the particle.
	@out	nil
*/
int MOAIParticleSystem::_reserveParticles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	u32 total	= state.GetValue < u32 >( 2, 0 );
	u32 size	= state.GetValue < u32 >( 3, 0 );

	self->ReserveParticles ( total, size );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveSprites
	@text	Reserve sprite capacity of system.
	
	@in		MOAIParticleSystem self
	@in		number nSprites
	@out	nil
*/
int MOAIParticleSystem::_reserveSprites ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	self->ReserveSprites ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveStates
	@text	Reserve total number of states for system.
	
	@in		MOAIParticleSystem self
	@in		number nStates
	@out	nil
*/
int MOAIParticleSystem::_reserveStates ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	self->ClearStates ();
	self->ReserveStates ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpriteColor
	@text	Set the color of the most recently added sprite.
	
	@in		MOAIParticleSystem self
	@in		number r
	@in		number g
	@in		number b
	@in		number a
	@out	nil
*/
int MOAIParticleSystem::_setSpriteColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UNNNN" )

	MOAIParticleSprite* sprite = self->GetTopSprite ();
	if ( sprite ) {
	
		float r = state.GetValue < float >( 2, 1.0f );
		float g = state.GetValue < float >( 3, 1.0f );
		float b = state.GetValue < float >( 4, 1.0f );
		float a = state.GetValue < float >( 5, 1.0f );
		
		sprite->mColor.Set ( r, g, b, a );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpriteDeckIdx
	@text	Set the sprite's deck index.
	
	@in		MOAIParticleSystem self
	@in		number index
	@out	nil
*/
int MOAIParticleSystem::_setSpriteDeckIdx ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	MOAIParticleSprite* sprite = self->GetTopSprite ();
	if ( sprite ) {
		sprite->mGfxID = state.GetValue < u32 >( 2, sprite->mGfxID );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setState
	@text	Set a particle state.
	
	@in		MOAIParticleSystem self
	@in		number index
	@in		MOAIParticleState state
	@out	nil
*/
int MOAIParticleSystem::_setState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UNU" )

	u32 idx = state.GetValue < u32 >( 2, 0 ) - 1;

	if ( idx < self->mStates.Size ()) {
	
		MOAIParticleState* particleState =  state.GetLuaObject < MOAIParticleState >( 3 );
		if ( particleState ) {
		
			particleState->Retain ();
			if ( self->mStates [ idx ]) {
				self->mStates [ idx ]->Release ();
			}
			self->mStates [ idx ] = particleState;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	surge
	@text	Release a batch emission or particles into the system.
	
	@in		MOAIParticleSystem self
	@opt	number total Default value is 1.
	@opt	number x Default value is 0.
	@opt	number y Default value is 0.
	@opt	number dx Default value is 0.
	@opt	number dy Default value is 0.
	@out	nil
*/
int MOAIParticleSystem::_surge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleSystem, "UN" )

	u32 n		= state.GetValue < u32 >( 2, 1 );
	float x		= state.GetValue < float >( 3, 0.0f );
	float y		= state.GetValue < float >( 4, 0.0f );
	float dx	= state.GetValue < float >( 5, 0.0f );
	float dy	= state.GetValue < float >( 6, 0.0f );
	
	for ( u32 i = 0; i < n; ++i ) {
		self->PushParticle ( x, y, dx, dy );
	}

	return 0;
}

//================================================================//
// MOAIParticleSystem
//================================================================//

//----------------------------------------------------------------//
void MOAIParticleSystem::ClearStates () {

	for ( u32 i = 0; i < this->mStates.Size (); ++i ) {
		if ( this->mStates [ i ]) {
			this->mStates [ i ]->Release ();
		}
	}
	this->mStates.Clear ();
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ClearQueue () {

	this->mHead = 0;
	this->mTail = 0;
}

//----------------------------------------------------------------//
void MOAIParticleSystem::Draw () {

	if ( !this->BindDeck ()) return;
	this->LoadShader ();

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	USAffine2D drawingMtx;
	USAffine2D spriteMtx;
	
	u32 maxSprites = this->mSprites.Size ();
	u32 total = this->mSpriteTop;
	u32 base = 0;
	if ( total > maxSprites ) {
		base = total % maxSprites;
		total = maxSprites;
	}
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( base + i ) % maxSprites;
	
		MOAIParticleSprite& sprite = this->mSprites [ idx ];
		drawbuffer.SetPenColor ( sprite.mColor );
		
		spriteMtx.ScRoTr ( sprite.mScl, sprite.mRot * ( float )D2R, sprite.mLoc );
		
		drawingMtx = this->GetLocalToWorldMtx ();
		drawingMtx.Append ( spriteMtx );
		
		this->mDeck->Draw ( drawingMtx, ( u32 )sprite.mGfxID, this->mRemapper );
	}
}

//----------------------------------------------------------------//
void MOAIParticleSystem::EnqueueParticle ( MOAIParticle& particle ) {

	if ( this->mTail ) {
		this->mTail->mNext = &particle;
	}
	else {
		this->mHead = &particle;
	}
	
	particle.mNext = 0;
	this->mTail = &particle;
}

//----------------------------------------------------------------//
u32 MOAIParticleSystem::GetLocalFrame ( USRect& frame ) {

	frame.Init ( -32.0f, -32.0f, 32.0f, 32.0f );
	return MOAIProp::BOUNDS_GLOBAL;
}

//----------------------------------------------------------------//
MOAIParticleState* MOAIParticleSystem::GetState ( u32 id ) {

	if ( id < this->mStates.Size ()) {
		return this->mStates [ id ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIParticleSprite* MOAIParticleSystem::GetTopSprite () {

	if ( this->mSpriteTop ) {
		u32 idx = this->mSpriteTop - 1;
		return &this->mSprites [ idx ];
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIParticleSystem::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAIParticleSystem::MOAIParticleSystem () :
	mParticleSize ( 0 ),
	mCapParticles ( false ),
	mCapSprites ( false ),
	mHead ( 0 ),
	mTail ( 0 ),
	mFree ( 0 ),
	mSpriteTop ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp2D )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleSystem::~MOAIParticleSystem () {

	this->ClearStates ();
}

//----------------------------------------------------------------//
void MOAIParticleSystem::OnUpdate ( float step ) {

	// clear out the sprites
	this->mSpriteTop = 0;

	// bail if no particles
	if ( !this->mHead ) return;

	// grab the head then clear out the queue
	MOAIParticle* cursor = this->mHead;
	this->ClearQueue ();
	
	// update the particles and rebuild the queue
	while ( cursor ) {
		MOAIParticle* particle = cursor;
		cursor = cursor->mNext;
		
		// update the particle
		if ( particle->mState ) {
			particle->mState->ProcessParticle ( *this, *particle, step );
		}
		
		// if is still to be killed, move it to the free list, else put it back in the queue
		if ( !particle->mState ) {
			particle->mNext = this->mFree;
			this->mFree = particle;
		}
		else {
			// and put it back in the queue
			this->EnqueueParticle ( *particle );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIParticleSystem::PushParticle ( float x, float y ) {
	
	return this->PushParticle ( x, y, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
bool MOAIParticleSystem::PushParticle ( float x, float y, float dx, float dy ) {
	
	if (( !this->mFree ) && this->mCapParticles ) {
		return false;
	}
	
	MOAIParticleState* state = this->GetState ( 0 );
	if ( !state ) return false;
	
	MOAIParticle* particle = 0;
	
	if ( this->mFree ) {
		particle = this->mFree;
		this->mFree = particle->mNext;
	}
	else if ( this->mHead ) {
		particle = this->mHead;
		this->mHead = particle->mNext;
	}
	
	if ( particle ) {
		
		float* r = particle->mData;
		
		r [ MOAIParticle::PARTICLE_X ] = x;
		r [ MOAIParticle::PARTICLE_Y ] = y;
		r [ MOAIParticle::PARTICLE_DX ] = dx;
		r [ MOAIParticle::PARTICLE_DY ] = dy;
		
		for ( u32 i = MOAIParticle::TOTAL_PARTICLE_REG; i < this->mParticleSize; ++i ) {
			r [ i ] = 0.0f;
		}
		
		state->InitParticle ( *this, *particle );
		this->EnqueueParticle ( *particle );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIParticleSystem::PushSprite ( const MOAIParticleSprite& sprite ) {

	u32 size = this->mSprites.Size ();
	
	if ( size ) {
	
		if (( this->mSpriteTop >= size ) && this->mCapSprites ) {
			return false;
		}
	
		u32 idx = ( this->mSpriteTop++ ) % size;
		this->mSprites [ idx ] = sprite;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIParticleSystem::RegisterLuaClass ( USLuaState& state ) {

	MOAIProp2D::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "capParticles",		_capParticles },
		{ "capSprites",			_capSprites },
		{ "clearSprites",		_clearSprites },
		{ "getState",			_getState },
		{ "pushParticle",		_pushParticle },
		{ "pushSprite",			_pushSprite },
		{ "reserveParticles",	_reserveParticles },
		{ "reserveSprites",		_reserveSprites },
		{ "reserveStates",		_reserveStates },
		{ "setSpriteColor",		_setSpriteColor },
		{ "setSpriteDeckIdx",	_setSpriteDeckIdx },
		{ "setState",			_setState },
		{ "surge",				_surge },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ReserveParticles ( u32 maxParticles, u32 particleSize ) {
	
	particleSize += MOAIParticle::TOTAL_PARTICLE_REG;
	
	this->mHead = 0;
	this->mTail = 0;
	this->mFree = 0;
	
	this->mParticleSize = particleSize;
	
	this->mParticles.Init ( maxParticles );
	this->mParticleData.Init ( maxParticles * particleSize );
	this->mParticleData.Fill ( 0.0f );
	
	for ( u32 i = 0; i < maxParticles; ++i ) {
		MOAIParticle& particle = this->mParticles [ i ];
		particle.mNext = this->mFree;
		this->mFree = &particle;
		
		if ( particleSize ) {
			particle.mData = &this->mParticleData [ i * particleSize ];
		}
		else {
			particle.mData = 0;
		}
	}
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ReserveSprites ( u32 maxSprites ) {

	this->mSprites.Init ( maxSprites );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ReserveStates ( u32 total ) {

	this->mStates.Init ( total );
	this->mStates.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {

	MOAIProp2D::SerializeIn ( state, serializer );
	MOAIAction::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {

	MOAIProp2D::SerializeOut ( state, serializer );
	MOAIAction::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
STLString MOAIParticleSystem::ToString () {

	STLString repr ( "TODO" );

	return repr;
}
