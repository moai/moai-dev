// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DBODY_H
#define	MOAIBOX2DBODY_H

#include <moai-box2d/MOAIBox2DWorld.h>

class b2Body;

//================================================================//
// MOAIBox2DBody
//================================================================//
/**	@lua	MOAIBox2DBody
	@text	Box2D body.

	@const	DYNAMIC
	@const	KINEMATIC
	@const	STATIC

*/
class MOAIBox2DBody :
	public MOAIBox2DPrim,
	public MOAIAbstractBaseTransform {
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
	static int 		_getContactList			( lua_State* L );
	static int		_getInertia				( lua_State* L );
	static int		_getGravityScale		( lua_State* L );
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
	static int		_setGravityScale		( lua_State* L );
	static int		_setLinearDamping		( lua_State* L );
	static int		_setLinearVelocity		( lua_State* L );
	static int		_setMassData			( lua_State* L );
	static int		_setTransform			( lua_State* L );
	static int		_setType				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			SetBody					( b2Body* body );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAbstractBaseTransform_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx );
	bool			MOAINode_ApplyAttrOp								( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update										();

public:
	
	friend class MOAIBox2DWorld;
	
	DECL_LUA_FACTORY ( MOAIBox2DBody )

	//----------------------------------------------------------------//
	void			Destroy					();
					MOAIBox2DBody			( ZLContext& context );
					~MOAIBox2DBody			();
};

#endif
