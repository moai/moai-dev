// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DBODY_H
#define	MOAIBOX2DBODY_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransformBase.h>

class b2Body;

//================================================================//
// MOAIBox2DBody
//================================================================//
/**	@name	MOAIBox2DBody
	@text	Box2D body.

	@const	DYNAMIC
	@const	KINEMATIC
	@const	STATIC

*/
class MOAIBox2DBody :
	public MOAIBox2DPrim,
	public MOAITransformBase {
private:

	b2Body*			mBody;

	//----------------------------------------------------------------//
	static int		_addChain				( lua_State* L );
	static int		_addCircle				( lua_State* L );
	static int      _addEdges               ( lua_State* L );
	static int		_addPolygon				( lua_State* L );
	static int		_addRect				( lua_State* L );
	static int		_applyAngularImpulse	( lua_State* L );
	static int		_applyForce				( lua_State* L );
	static int		_applyLinearImpulse		( lua_State* L );
	static int		_applyTorque			( lua_State* L );
	static int		_destroy				( lua_State* L );
	static int		_getAngle				( lua_State* L );
	static int		_getAngularVelocity		( lua_State* L );
	static int		_getInertia				( lua_State* L );
	static int		_getLinearVelocity		( lua_State* L );
	static int		_getLocalCenter			( lua_State* L );
	static int		_getMass				( lua_State* L );
	static int		_getPosition			( lua_State* L );
	static int		_getWorldCenter			( lua_State* L );
	static int		_isActive				( lua_State* L );
	static int		_isAwake				( lua_State* L );
	static int		_isBullet				( lua_State* L );
	static int		_isFixedRotation		( lua_State* L );
	static int		_resetMassData			( lua_State* L );
	static int		_setActive				( lua_State* L );
	static int		_setAngularDamping		( lua_State* L );
	static int		_setAngularVelocity		( lua_State* L );
	static int		_setAwake				( lua_State* L );
	static int		_setBullet				( lua_State* L );
	static int		_setFixedRotation		( lua_State* L );
	static int		_setLinearDamping		( lua_State* L );
	static int		_setLinearVelocity		( lua_State* L );
	static int		_setMassData			( lua_State* L );
	static int		_setTransform			( lua_State* L );
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			SetBody					( b2Body* body );
	void			OnDepNodeUpdate			();

public:
	
	friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DBody )

	//----------------------------------------------------------------//
	void			Destroy					();
					MOAIBox2DBody			();
					~MOAIBox2DBody			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif