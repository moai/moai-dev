// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DGEARJOINT_H
#define	MOAIBOX2DGEARJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DGearJoint
//================================================================//
/**	@lua MOAIBox2DGearJoint
	@text Box2D gear joint.
*/
class MOAIBox2DGearJoint :
	public MOAIBox2DJoint {
private:

	friend class MOAIBox2DWorld;

	MOAILuaSharedPtr < MOAIBox2DJoint > mJointA;
	MOAILuaSharedPtr < MOAIBox2DJoint > mJointB;

	//----------------------------------------------------------------//
	static int		_getJointA				( lua_State* L );
	static int		_getJointB				( lua_State* L );
	static int		_getRatio				( lua_State* L );
	static int		_setRatio				( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DGearJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DGearJoint		();
					~MOAIBox2DGearJoint		();
};

#endif
