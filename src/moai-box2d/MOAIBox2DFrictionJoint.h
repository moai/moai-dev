// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DFRICTIONJOINT_H
#define	MOAIBOX2DFRICTIONJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

class b2Joint;

//================================================================//
// MOAIBox2DFrictionJoint
//================================================================//
/**	@lua MOAIBox2DFrictionJoint
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
