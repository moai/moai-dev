// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DPULLEYJOINT_H
#define	MOAIBOX2DPULLEYJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIBox2DPulleyJoint
//================================================================//
/**	@name MOAIBox2DPulleyJoint
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

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DPulleyJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DPulleyJoint	();
					~MOAIBox2DPulleyJoint	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif