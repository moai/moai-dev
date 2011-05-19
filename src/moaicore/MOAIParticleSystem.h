// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESYSTEM_H
#define	MOAIPARTICLESYSTEM_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIProp2D.h>

class MOAIDeck;
class MOAIParticleEngine;
class MOAIParticleScript;
class MOAIParticleState;

//================================================================//
// MOAIParticleSystem
//================================================================//
/**	@name	MOAIParticleSystem
	@text	Particle system.
*/
class MOAIParticleSystem :
	public MOAIProp2D,
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
	
	USLeanArray < MOAIParticleSprite >	mSprites;
	u32									mSpriteTop;
	
	//----------------------------------------------------------------//
	static int		_capParticles		( lua_State* L );
	static int		_capSprites			( lua_State* L );
	static int		_clearSprites		( lua_State* L );
	static int		_getState			( lua_State* L );
	static int		_pushParticle		( lua_State* L );
	static int		_pushSprite			( lua_State* L );
	static int		_reserveParticles	( lua_State* L );
	static int		_reserveRects		( lua_State* L );
	static int		_reserveSprites		( lua_State* L );
	static int		_reserveStates		( lua_State* L );
	static int		_setSpriteColor		( lua_State* L );
	static int		_setSpriteDeckIdx	( lua_State* L );
	static int		_setState			( lua_State* L );
	static int		_surge				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					ClearStates				();
	void					ClearQueue				();
	void					EnqueueParticle			( MOAIParticle& particle );
	MOAIParticleSprite*		GetTopSprite			();
	MOAIParticleState*		GetState				( u32 id );
	void					OnUpdate				( float step );

public:
	
	friend class MOAIParticleEngine;
	friend class MOAIParticleScript;
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticleSystem )

	//----------------------------------------------------------------//
	void			Draw					();
	u32				GetLocalFrame			( USRect& frame );
	bool			IsDone					();
					MOAIParticleSystem		();
					~MOAIParticleSystem		();
	bool			PushParticle			( float x, float y );
	bool			PushParticle			( float x, float y, float dx, float dy );
	bool			PushSprite				( const MOAIParticleSprite& sprite );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			ReserveParticles		( u32 maxParticles, u32 particleSize );
	void			ReserveRects			( u32 total );
	void			ReserveSprites			( u32 maxSprites );
	void			ReserveStates			( u32 total );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void			SetConstant				( u32 idx, float value );
	void			SetRect					( u32 idx, USRect& rect );
	STLString		ToString				();
};

#endif
