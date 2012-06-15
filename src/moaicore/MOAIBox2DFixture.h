// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DFIXTURE_H
#define	MOAIBOX2DFIXTURE_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILua.h>

class b2Fixture;
class MOAIBox2DArbiter;

//================================================================//
// MOAIBox2DFixture
//================================================================//
/**	@name MOAIBox2DFixture
	@text Box2D fixture.
*/
class MOAIBox2DFixture :
	public MOAIBox2DPrim {
private:

	b2Fixture*		mFixture;

	MOAILuaLocal	mCollisionHandler;
	u32				mCollisionPhaseMask;
	u32				mCollisionCategoryMask;

	//----------------------------------------------------------------//
	static int	_destroy				( lua_State* L );
	static int	_getBody				( lua_State* L );
	static int  _getFilter				( lua_State* L );
	static int	_setCollisionHandler	( lua_State* L );
	static int	_setDensity				( lua_State* L );
	static int	_setFilter				( lua_State* L );
	static int	_setFriction			( lua_State* L );
	static int	_setRestitution			( lua_State* L );
	static int	_setSensor				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			HandleCollision		( u32 eventType, MOAIBox2DFixture* other, MOAIBox2DArbiter* arbiter );
	static u32		LoadVerts			( MOAILuaState& state, int idx, b2Vec2* verts, u32 max, float unitsToMeters  );
	void			SetFixture			( b2Fixture* fixture );


public:
	
	friend class MOAIBox2DArbiter;
	friend class MOAIBox2DBody;
	friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DFixture )
	
	//----------------------------------------------------------------//
	void			Destroy					();
					MOAIBox2DFixture		();
					~MOAIBox2DFixture		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif