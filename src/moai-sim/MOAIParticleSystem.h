// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLESYSTEM_H
#define	MOAIPARTICLESYSTEM_H

#include <moai-sim/host_particles.h>
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
	ZLSize								mSpriteTop;
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
	MOAIParticleState*		GetState				( ZLIndex id );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIAbstractProp_GetModelBounds			();
	bool					MOAIAction_IsDone						();
	void					MOAIAction_Update						( double step );
	void					MOAIGraphicsPropBase_Draw				( int subPrimID );
	bool					MOAIGraphicsPropBase_LoadGfxState		();

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
					MOAIParticleSystem		();
					~MOAIParticleSystem		();
	bool			PushParticle			( float x, float y );
	bool			PushParticle			( float x, float y, float dx, float dy );
	bool			PushParticle			( float x, float y, float dx, float dy, ZLIndex stateIdx );
	bool			PushSprite				( const AKUParticleSprite& sprite );
	void			ReserveParticles		( ZLSize maxParticles, ZLSize particleSize );
	void			ReserveRects			( ZLSize total );
	void			ReserveSprites			( ZLSize maxSprites );
	void			ReserveStates			( ZLSize total );
	void			SetConstant				( ZLIndex idx, float value );
	void			SetRect					( ZLIndex idx, ZLRect& rect );
};

#endif
