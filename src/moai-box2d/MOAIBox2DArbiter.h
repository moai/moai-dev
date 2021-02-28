// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DARBITER_H
#define	MOAIBOX2DARBITER_H

#include <Box2D/Box2D.h>

// Forward declaration
class MOAIBox2DWorld;

//================================================================//
// MOAIBox2DArbiter
//================================================================//
/**	@lua	MOAIBox2DArbiter
	@text	Box2D Arbiter.
	
	@flag	BEGIN
	@flag	END
	@flag	POST_SOLVE
	@flag	PRE_SOLVE
	@flag	ALL
*/
class MOAIBox2DArbiter :
	public virtual MOAILuaObject,
	public b2ContactListener {
private:

	b2Contact*					mContact;
	const b2ContactImpulse*		mImpulse;

	b2Vec2  mContactNormal;
	b2Vec2	mContactPoints [ 2 ];
	u32		mTotalPoints;
	float	mNormalImpulse;
	float	mTangentImpulse;
	bool	mContactDirty;

	/* For reference to get the unitsToMeters value */
	const MOAIBox2DWorld*       mWorld;

	//----------------------------------------------------------------//
	static int		_getContactNormal		( lua_State* L );
	static int		_getContactPoints		( lua_State* L );
	static int		_getNormalImpulse		( lua_State* L );
	static int		_getTangentImpulse		( lua_State* L );
	static int		_setContactEnabled		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			AffirmContactData 		();
	void			BeginContact			( b2Contact* contact );
	void			EndContact				( b2Contact* contact );
	float			GetUnitsToMeters		() const;
	void			PostSolve				( b2Contact* contact, const b2ContactImpulse* impulse );
	void			PreSolve				( b2Contact* contact, const b2Manifold* oldManifold );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DArbiter )
	
	enum {
		BEGIN			= 0x00000001,
		END				= 0x00000002,
		POST_SOLVE		= 0x00000004,
		PRE_SOLVE		= 0x00000008,
		ALL				= 0x0000000f,
	};
	
	GET_SET ( const MOAIBox2DWorld*, World, mWorld )
	
	//----------------------------------------------------------------//
					MOAIBox2DArbiter		( ZLContext& context );
					~MOAIBox2DArbiter		();
	
};

#endif
