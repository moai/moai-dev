// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DMOUSEJOINT_H
#define	MOAIBOX2DMOUSEJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DMouseJoint
//================================================================//
/**	@lua MOAIBox2DMouseJoint
	@text Box2D 'mouse' joint.
*/
class MOAIBox2DMouseJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getDampingRatio		( lua_State* L );
	static int		_getFrequency			( lua_State* L );
	static int		_getMaxForce			( lua_State* L );
	static int		_getTarget				( lua_State* L );
	static int		_setDampingRatio		( lua_State* L );
	static int		_setFrequency			( lua_State* L );
	static int		_setMaxForce			( lua_State* L );
	static int		_setTarget				( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DMouseJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DMouseJoint		();
					~MOAIBox2DMouseJoint	();
};

#endif
