// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DGEARJOINT_H
#define	MOAIBOX2DGEARJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DGearJoint
//================================================================//
/**	@name MOAIBox2DGearJoint
	@text Box2D gear joint.
*/
class MOAIBox2DGearJoint :
	public MOAIBox2DJoint {
private:

	friend class MOAIBox2DWorld;

	USRef < MOAIBox2DJoint > mJointA;
	USRef < MOAIBox2DJoint > mJointB;

	//----------------------------------------------------------------//
	static int		_getJointA				( lua_State* L );
	static int		_getJointB				( lua_State* L );
	static int		_getRatio				( lua_State* L );
	static int		_setRatio				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DGearJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DGearJoint		();
					~MOAIBox2DGearJoint		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
#endif