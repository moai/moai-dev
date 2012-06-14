// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DDISTANCEJOINT_H
#define	MOAIBOX2DDISTANCEJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>
#include <moaicore/MOAILua.h>

class b2Joint;

//================================================================//
// MOAIBox2DDistanceJoint
//================================================================//
/**	@name MOAIBox2DDistanceJoint
	@text Box2D distance joint.
*/
class MOAIBox2DDistanceJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getDampingRatio			( lua_State* L );
	static int		_getFrequency				( lua_State* L );
	static int		_getLength					( lua_State* L );
	static int		_setDampingRatio			( lua_State* L );
	static int		_setFrequency				( lua_State* L );
	static int		_setLength					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DDistanceJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DDistanceJoint		();
					~MOAIBox2DDistanceJoint		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
#endif