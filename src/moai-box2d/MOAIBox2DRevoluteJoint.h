// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DREVOLUTEJOINT_H
#define	MOAIBOX2DREVOLUTEJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DRevoluteJoint
//================================================================//
/**	@lua MOAIBox2DRevoluteJoint
	@text Box2D revolute joint.
*/
class MOAIBox2DRevoluteJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getJointAngle			( lua_State* L );
	static int		_getJointSpeed			( lua_State* L );
	static int		_getLowerLimit			( lua_State* L );
	static int		_getMotorSpeed			( lua_State* L );
	static int		_getMotorTorque			( lua_State* L );
	static int		_getUpperLimit			( lua_State* L );
	static int		_isLimitEnabled			( lua_State* L );
	static int		_isMotorEnabled			( lua_State* L );
	static int		_setLimit				( lua_State* L );
	static int		_setLimitEnabled		( lua_State* L );
	static int		_setMaxMotorTorque		( lua_State* L );
	static int		_setMotor				( lua_State* L );
	static int		_setMotorSpeed			( lua_State* L );
	static int		_setMotorEnabled		( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DRevoluteJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DRevoluteJoint		( ZLContext& context );
					~MOAIBox2DRevoluteJoint		();
};

#endif
