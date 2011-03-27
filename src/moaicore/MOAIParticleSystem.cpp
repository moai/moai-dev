// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <float.h>
#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIParticleEngine.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAITexture.h>

class MOAIDataBuffer;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_capParticles ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UB" )

	self->mCapParticles = state.GetValue < bool >( 2, self->mCapParticles );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_capSprites ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UB" )

	self->mCapSprites = state.GetValue < bool >( 2, self->mCapSprites );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_clearSprites ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "U" )

	self->mSpriteTop = 0;
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_getState ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UN" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAIParticleState* particleState = self->GetState ( idx );
	if ( particleState ) {
		particleState->PushLuaInstance ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_pushParticle ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "U" )

	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );
	
	float dx = state.GetValue < float >( 4, 0.0f );
	float dy = state.GetValue < float >( 5, 0.0f );

	bool result = self->PushParticle ( x, y, dx, dy );
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_pushSprite ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UNN" )

	MOAIParticleSprite sprite;

	sprite.mLoc.mX		= state.GetValue < float >( 2, 0.0f );
	sprite.mLoc.mY		= state.GetValue < float >( 3, 0.0f );
	
	sprite.mRot			= state.GetValue < float >( 4, 0.0f );

	sprite.mScale.mX	= state.GetValue < float >( 5, 1.0f );
	sprite.mScale.mY	= state.GetValue < float >( 6, 1.0f );
	
	sprite.mColor		= 0xffffffff;
	sprite.mGfxID		= 0;
	
	bool result = self->PushSprite ( sprite );
	
	if ( result ) {
		self->ScheduleUpdate ();
	}
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_reserveConstants ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UN" )

	self->ReserveConstants ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_reserveParticles ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UNN" )

	u32 total	= state.GetValue < u32 >( 2, 0 );
	u32 size	= state.GetValue < u32 >( 3, 0 );

	self->ReserveParticles ( total, size );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_reserveSprites ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UN" )

	self->ReserveSprites ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_reserveStates ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UN" )

	self->ClearStates ();
	self->ReserveStates ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_setConstant ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "U" )

	u32 idx			= state.GetValue < u32 >( 2, 0 );
	float value		= state.GetValue < float >( 3, 0.0f );

	self->SetConstant ( idx, value );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_setSpriteColor ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UNNNN" )

	MOAIParticleSprite* sprite = self->GetTopSprite ();
	if ( sprite ) {
	
		float r = state.GetValue < float >( 2, 1.0f );
		float g = state.GetValue < float >( 3, 1.0f );
		float b = state.GetValue < float >( 4, 1.0f );
		float a = state.GetValue < float >( 5, 1.0f );
	
		sprite->mColor = USColor::PackRGBA ( r, g, b, a );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_setSpriteGfxID ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UN" )

	MOAIParticleSprite* sprite = self->GetTopSprite ();
	if ( sprite ) {
		sprite->mGfxID = state.GetValue < u32 >( 2, sprite->mGfxID );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_setSpriteGlow ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "U" )

	MOAIParticleSprite* sprite = self->GetTopSprite ();
	if ( sprite ) {
	
		sprite->mGlow = state.GetValue < float >( 2, 0.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_setState ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "UNU" )

	u32 idx = state.GetValue < u32 >( 2, 0 ) - 1;

	if ( idx < self->mStates.Size ()) {
	
		MOAIParticleState* particleState =  state.GetLuaData < MOAIParticleState >( 3 );
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleSystem::_surge ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleSystem, "U" )

	u32 n		= state.GetValue < u32 >( 2, 0 );
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
	
	USColorVec spriteColor;
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( base + i ) % maxSprites;
	
		MOAIParticleSprite& sprite = this->mSprites [ idx ];
		spriteColor.SetRGBA ( sprite.mColor );
		spriteColor.mA *= ( 1.0f - sprite.mGlow );
		drawbuffer.SetPenColor ( spriteColor );
		
		spriteMtx.ScRoTr ( sprite.mScale, sprite.mRot * ( float )D2R, sprite.mLoc );
		
		drawingMtx = this->GetLocalToWorldMtx ();
		drawingMtx.Append ( spriteMtx );
		
		this->mDeck->Draw ( drawingMtx, sprite.mGfxID );
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
bool MOAIParticleSystem::IsBusy () {

	return true;
}

//----------------------------------------------------------------//
void MOAIParticleSystem::LoadParticle ( u32 particleID, float registers []) {

	u32 idx = particleID * this->mParticleSize;
	size_t size = this->mParticleSize * sizeof ( float );
	memmove ( registers, &this->mParticleData [ idx ], size );
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

	// bail if no particles
	if ( !this->mHead ) return;

	// registers for particle machine
	float registers [ 256 ];

	// clear out the sprites
	this->mSpriteTop = 0;

	// grab the head then clear out the queue
	MOAIParticle* cursor = this->mHead;
	this->ClearQueue ();
	
	// update the particles and rebuild the queue
	while ( cursor ) {
		MOAIParticle* particle = cursor;
		cursor = cursor->mNext;
		
		// set up the current particle
		this->mParticle = particle;
		this->mKill = true;
		
		// do the timestep
		particle->mAge += step;
		if ( particle->mAge > particle->mDuration ) {
			particle->mAge = particle->mDuration;
		}
		
		// get the particle state handler
		MOAIParticleState* state = particle->mState;
		if ( state ) {
		
			this->mKill = false;
			
			// load the particle into the registers
			this->LoadParticle ( particle->mID, registers );
			
			// update the particle
			u8* update = state->mUpdate->GetBytecode ();
			MOAIParticleEngine::RunBytecode ( *this, update, state, step, registers );
			
			// kill the particle if it's finished
			if ( particle->mAge >= particle->mDuration ) {
				this->mKill = true;
			}
		}
		
		// if flagged for killing, first try to move the particle on to the next state
		if ( this->mKill && state && state->mNext ) {
		
			this->mKill = false;
		
			particle->mAge = 0.0f;
			particle->mDuration = FLT_MAX;
			particle->mState = state->mNext;
			state = particle->mState;
			
			u8* init = state->mInit->GetBytecode ();
			MOAIParticleEngine::RunBytecode ( *this, init, registers );
			
			u8* update = state->mUpdate->GetBytecode ();
			MOAIParticleEngine::RunBytecode ( *this, update, state, 0.0f, registers );
		}
		
		// if is still to be killed, move it to the free list, else put it back in the queue
		if ( this->mKill ) {
			particle->mNext = this->mFree;
			this->mFree = particle;
		}
		else {
			
			// store the updated particle
			this->StoreParticle ( particle->mID, registers );
			
			// render the particle
			u8* render = state->mRender->GetBytecode ();
			MOAIParticleEngine::RunBytecode ( *this, render, state, registers );
		
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

	MOAIParticle temp;
	temp.mAge = 0.0f;
	temp.mDuration = FLT_MAX;
	temp.mState = state;
	
	this->mParticle = &temp;
	this->mKill = false;
	
	float registers [ 256 ];
	memset ( registers, 0, ( this->mParticleSize * sizeof ( float )));
	
	registers [ 0 ] = x;
	registers [ 1 ] = y;
	registers [ 2 ] = dx;
	registers [ 3 ] = dy;
	
	u8* init = state->mInit->GetBytecode ();
	MOAIParticleEngine::RunBytecode ( *this, init, registers );
	
	if ( !this->mKill ) {
		
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
			
			particle->mAge = temp.mAge;
			particle->mDuration = temp.mDuration;
			particle->mState = temp.mState;
			
			this->StoreParticle ( particle->mID, registers );
			this->EnqueueParticle ( *particle );
			
			return true;
		}
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
	
	LuaReg regTable [] = {
		{ "capParticles",		_capParticles },
		{ "capSprites",			_capSprites },
		{ "clearSprites",		_clearSprites },
		{ "getState",			_getState },
		{ "pushParticle",		_pushParticle },
		{ "pushSprite",			_pushSprite },
		{ "reserveConstants",	_reserveConstants },
		{ "reserveParticles",	_reserveParticles },
		{ "reserveSprites",		_reserveSprites },
		{ "reserveStates",		_reserveStates },
		{ "setConstant",		_setConstant },
		{ "setSpriteColor",		_setSpriteColor },
		{ "setSpriteGfxID",		_setSpriteGfxID },
		{ "setSpriteGlow",		_setSpriteGlow },
		{ "setState",			_setState },
		{ "surge",				_surge },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ReserveConstants ( u32 total ) {

	this->mConsts.Init ( total );
}

//----------------------------------------------------------------//
void MOAIParticleSystem::ReserveParticles ( u32 maxParticles, u32 particleSize ) {

	this->mHead = 0;
	this->mTail = 0;
	this->mFree = 0;

	this->mParticleSize = particleSize;

	this->mParticles.Init ( maxParticles );
	this->mParticleData.Init ( maxParticles * particleSize );
	this->mParticleData.Fill ( 0.0f );
	
	for ( u32 i = 0; i < maxParticles; ++i ) {
		MOAIParticle& particle = this->mParticles [ i ];
		particle.mID = i;
		particle.mNext = this->mFree;
		this->mFree = &particle;
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
void MOAIParticleSystem::SetConstant ( u32 idx, float value ) {

	this->mConsts [ idx ] = value;
}

//----------------------------------------------------------------//
void MOAIParticleSystem::StoreParticle ( u32 particleID, float registers []) {
	
	u32 idx = particleID * this->mParticleSize;
	size_t size = this->mParticleSize * sizeof ( float );
	memmove ( &this->mParticleData [ idx ], registers, size );
}

//----------------------------------------------------------------//
STLString MOAIParticleSystem::ToString () {

	STLString repr ( "TODO" );

	return repr;
}
