// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DWHEELJOINT_H
#define	MOAIBOX2DWHEELJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//
/**	@name MOAIBox2DWeldJoint
 @text Box2D weld joint.
 */
class MOAIBox2DWheelJoint :
public MOAIBox2DJoint {
private:
	
	//----------------------------------------------------------------//
	static int		_getJointTranslation	( lua_State* L );
	static int		_getJointSpeed			( lua_State* L );
	static int		_isMotorEnabled			( lua_State* L );	
	static int		_getMotorSpeed			( lua_State* L );
	static int		_getSpringDampingRatio	( lua_State* L );	
	static int		_getMaxMotorTorque		( lua_State* L );
	static int		_getMotorTorque			( lua_State* L );	
	static int		_getSpringFrequencyHz	( lua_State* L );
	static int		_setMotor				( lua_State* L );
	static int		_setMotorSpeed			( lua_State* L );
	static int		_setMotorEnabled		( lua_State* L );
	static int		_setMaxMotorTorque		( lua_State* L );	
	static int		_setSpringDampingRatio	( lua_State* L );
	static int		_setSpringFrequencyHz	( lua_State* L );

	
public:
	
	DECL_LUA_FACTORY ( MOAIBox2DWheelJoint )

	//----------------------------------------------------------------//
					MOAIBox2DWheelJoint		();
					~MOAIBox2DWheelJoint	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif