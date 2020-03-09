// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DJOINT_H
#define	MOAIBOX2DJOINT_H

#include <moai-box2d/MOAIBox2DWorld.h>

class b2Joint;

//================================================================//
// MOAIBox2DJoint
//================================================================//
/**	@lua MOAIBox2DJoint
	@text Box2D joint.
*/
class MOAIBox2DJoint :
	public MOAIBox2DPrim {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	b2Joint*	mJoint;

	//----------------------------------------------------------------//
	static int			_destroy				( lua_State* L );
	static int			_getAnchorA				( lua_State* L );
	static int			_getAnchorB				( lua_State* L );
	static int			_getBodyA				( lua_State* L );
	static int			_getBodyB				( lua_State* L );
	static int			_getReactionForce		( lua_State* L );
	static int			_getReactionTorque		( lua_State* L );
	
	//----------------------------------------------------------------//
	void				Clear					();
	void				SetJoint				( b2Joint* joint );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	friend class MOAIBox2DWorld;
	
	//----------------------------------------------------------------//
	void				Destroy					();
						MOAIBox2DJoint			();
						~MOAIBox2DJoint			();
};

#endif
