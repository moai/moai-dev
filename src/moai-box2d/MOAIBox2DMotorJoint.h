// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DMOTORJOINT_H
#define	MOAIBOX2DMOTORJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DMotorJoint
//================================================================//
/**	@lua MOAIBox2DMotorJoint
	@text Box2D motor joint.
*/
class MOAIBox2DMotorJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getAngularOffset			( lua_State* L );
	static int		_getCorrectionFactor		( lua_State* L );
	static int		_getLinearOffset			( lua_State* L );
	static int		_getMaxForce				( lua_State* L );
	static int		_getMaxTorque				( lua_State* L );
	static int		_setAngularOffset			( lua_State* L );
	static int		_setCorrectionFactor		( lua_State* L );
	static int		_setLinearOffset			( lua_State* L );
	static int		_setMaxForce				( lua_State* L );
	static int		_setMaxTorque				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DMotorJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DMotorJoint			();
					~MOAIBox2DMotorJoint		();
	
};

#endif
