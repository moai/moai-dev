// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DPULLEYJOINT_H
#define	MOAIBOX2DPULLEYJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DPulleyJoint
//================================================================//
/**	@lua MOAIBox2DPulleyJoint
	@text Box2D pulley joint.
*/
class MOAIBox2DPulleyJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getGroundAnchorA		( lua_State* L );
	static int		_getGroundAnchorB		( lua_State* L );
	static int		_getLength1				( lua_State* L );
	static int		_getLength2				( lua_State* L );
	static int		_getRatio				( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DPulleyJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DPulleyJoint	();
					~MOAIBox2DPulleyJoint	();
};

#endif
