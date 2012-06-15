// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DJOINT_H
#define	MOAIBOX2DJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DWorld.h>

class b2Joint;

//================================================================//
// MOAIBox2DJoint
//================================================================//
/**	@name MOAIBox2DJoint
	@text Box2D joint.
*/
class MOAIBox2DJoint :
	public MOAIBox2DPrim {
protected:
	
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
	void				SetJoint				( b2Joint* joint );

public:
	
	friend class MOAIBox2DWorld;
	
	//----------------------------------------------------------------//
	void				Destroy					();
						MOAIBox2DJoint			();
						~MOAIBox2DJoint			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif