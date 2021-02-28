// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DPRISMATICJOINT_H
#define	MOAIBOX2DPRISMATICJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DPrismaticJoint
//================================================================//
/**	@lua MOAIBox2DPrismaticJoint
	@text Box2D prismatic joint.
*/
class MOAIBox2DPrismaticJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getJointSpeed			( lua_State* L );
	static int		_getJointTranslation	( lua_State* L );
	static int		_getLowerLimit			( lua_State* L );
	static int		_getMotorForce			( lua_State* L );
	static int		_getMotorSpeed			( lua_State* L );
	static int		_getUpperLimit			( lua_State* L );
	static int		_isLimitEnabled			( lua_State* L );
	static int		_isMotorEnabled			( lua_State* L );
	static int		_setLimit				( lua_State* L );
	static int		_setLimitEnabled		( lua_State* L );
	static int		_setMaxMotorForce		( lua_State* L );
	static int		_setMotor				( lua_State* L );
	static int		_setMotorSpeed			( lua_State* L );
	static int		_setMotorEnabled		( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DPrismaticJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DPrismaticJoint		( ZLContext& context );
					~MOAIBox2DPrismaticJoint	();
};

#endif
