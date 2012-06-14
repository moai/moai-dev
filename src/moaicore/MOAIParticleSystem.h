// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESYSTEM_H
#define	MOAIPARTICLESYSTEM_H

#include <aku/AKU-particles.h>
#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIProp.h>

class MOAIDeck;
class MOAIParticleScript;
class MOAIParticleState;

//================================================================//
// MOAIParticleSystem
//================================================================//
/**	@name	MOAIParticleSystem
	@text	Particle system.
*/
class MOAIParticleSystem :
	public MOAIProp,
	public MOAIAction {
private:

	USLeanArray < MOAIParticleState* >	mStates;
	USLeanArray < MOAIParticle >		mParticles;
	USLeanArray < float >				mParticleData;
	u32									mParticleSize;
	
	bool								mCapParticles;
	bool								mCapSprites;
	
	MOAIParticle*						mHead;
	MOAIParticle*						mTail;
	MOAIParticle*						mFree;
	
	USLeanArray < AKUParticleSprite >	mSprites;
	u32									mSpriteTop;
	
	bool								mComputeBounds;
	USBox								mParticleBounds;
	
	//----------------------------------------------------------------//
	static int		_capParticles			( lua_State* L );
	static int		_capSprites				( lua_State* L );
	static int		_clearSprites			( lua_State* L );
	static int		_isIdle					( lua_State* L );
	static int		_getState				( lua_State* L );
	static int		_pushParticle			( lua_State* L );
	static int		_pushSprite				( lua_State* L );
	static int		_reserveParticles		( lua_State* L );
	static int		_reserveRects			( lua_State* L );
	static int		_reserveSprites			( lua_State* L );
	static int		_reserveStates			( lua_State* L );
	static int		_setComputeBounds		( lua_State* L );
	static int		_setSpriteColor			( lua_State* L );
	static int		_setSpriteDeckIdx		( lua_State* L );
	static int		_setState				( lua_State* L );
	static int		_surge					( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ClearStates				();
	void					ClearQueue				();
	void					EnqueueParticle			( MOAIParticle& particle );
	AKUParticleSprite*		GetTopSprite			();
	MOAIParticleState*		GetState				( u32 id );
	void					OnUpdate				( float step );

public:
	
	friend class MOAIParticleEngine;
	friend class MOAIParticleScript;
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticleSystem )

	//----------------------------------------------------------------//
	void			Draw					( int subPrimID );
	u32				GetPropBounds			( USBox& bounds );
	bool			IsDone					();
					MOAIParticleSystem		();
					~MOAIParticleSystem		();
	bool			PushParticle			( float x, float y );
	bool			PushParticle			( float x, float y, float dx, float dy );
	bool			PushSprite				( const AKUParticleSprite& sprite );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveParticles		( u32 maxParticles, u32 particleSize );
	void			ReserveRects			( u32 total );
	void			ReserveSprites			( u32 maxSprites );
	void			ReserveStates			( u32 total );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetConstant				( u32 idx, float value );
	void			SetRect					( u32 idx, USRect& rect );
};

#endif
