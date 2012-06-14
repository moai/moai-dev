// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DFRICTIONJOINT_H
#define	MOAIBOX2DFRICTIONJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>
#include <moaicore/MOAILua.h>

class b2Joint;

//================================================================//
// MOAIBox2DFrictionJoint
//================================================================//
/**	@name MOAIBox2DFrictionJoint
	@text Box2D friction joint.
*/
class MOAIBox2DFrictionJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getMaxForce			( lua_State* L );
	static int		_getMaxTorque			( lua_State* L );
	static int		_setMaxForce			( lua_State* L );
	static int		_setMaxTorque			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DFrictionJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DFrictionJoint		();
					~MOAIBox2DFrictionJoint		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
#endif