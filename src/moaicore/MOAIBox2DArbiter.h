// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DARBITER_H
#define	MOAIBOX2DARBITER_H
#if USE_BOX2D

#include <Box2D/Box2D.h>

//================================================================//
// MOAIBox2DArbiter
//================================================================//
/**	@name	MOAIBox2DArbiter
	@text	Box2D Arbiter.
	
	@flag	BEGIN
	@flag	END
	@flag	POST_SOLVE
	@flag	PRE_SOLVE
	@flag	ALL
*/
class MOAIBox2DArbiter :
	public virtual USLuaObject,
	public b2ContactListener {
private:

	b2Contact*					mContact;
	const b2ContactImpulse*		mImpulse;

	float	mNormalImpulse;
	float	mTangentImpulse;

	//----------------------------------------------------------------//
	static int		_getNormalImpulse		( lua_State* L );
	static int		_getTangentImpulse		( lua_State* L );
	static int		_setContactEnabled		( lua_State* L );
	static int		_new					( lua_State* L );
	
	//----------------------------------------------------------------//
	void	BeginContact	( b2Contact* contact );
	void	EndContact		( b2Contact* contact );
	void	PostSolve		( b2Contact* contact, const b2ContactImpulse* impulse );
	void	PreSolve		( b2Contact* contact, const b2Manifold* oldManifold );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DArbiter )
	
	enum {
		BEGIN			= 0x00000001,
		END				= 0x00000002,
		POST_SOLVE		= 0x00000004,
		PRE_SOLVE		= 0x00000008,
		ALL				= 0x0000000f,
	};
	
	//----------------------------------------------------------------//
					MOAIBox2DArbiter		();
					~MOAIBox2DArbiter		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
#endif