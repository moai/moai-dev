// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESYSTEM_H
#define	MOAIPARTICLESYSTEM_H

#include <moai-sim/host_particles.h>
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIParticle.h>

class MOAIDeck;
class MOAIParticleScript;
class MOAIParticleState;

//================================================================//
// MOAIParticleSystem
//================================================================//
/**	@lua	MOAIParticleSystem
	@text	Particle system.
*/
class MOAIParticleSystem :
	public MOAIGraphicsProp,
	public MOAIAction {
private:

	ZLLeanArray < MOAIParticleState* >	mStates;
	ZLLeanArray < MOAIParticle >		mParticles;
	ZLLeanArray < float >				mParticleData;
	u32									mParticleSize;
	
	bool								mCapParticles;
	bool								mCapSprites;
	
	MOAIParticle*						mHead;
	MOAIParticle*						mTail;
	MOAIParticle*						mFree;
	
	ZLLeanArray < AKUParticleSprite >	mSprites;
	u32									mSpriteTop;
	u32									mDrawOrder;
	
	bool								mComputeBounds;
	ZLBox								mParticleBounds;
	
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
	static int		_setDrawOrder			( lua_State* L );
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
	u32						OnGetModelBounds		( ZLBox& bounds );
	void					OnUpdate				( double step );

public:

	enum {
		ORDER_NORMAL,
		ORDER_REVERSE,
	};

	friend class MOAIParticleEngine;
	friend class MOAIParticleScript;
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticleSystem )

	//----------------------------------------------------------------//
	void			Draw					( int subPrimID, float lod );
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
	void			SetRect					( u32 idx, ZLRect& rect );
};

#endif
