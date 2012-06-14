// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DROPEJOINT_H
#define	MOAIBOX2DROPEJOINT_H
#if USE_BOX2D

#include <moaicore/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//
/**	@name MOAIBox2DWeldJoint
 @text Box2D weld joint.
 */
class MOAIBox2DRopeJoint :
public MOAIBox2DJoint {
private:
	
	//----------------------------------------------------------------//
	static int _getLimitState			( lua_State* L );
	static int _getMaxLength			( lua_State* L );
	static int _setMaxLength			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DRopeJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DRopeJoint		();
					~MOAIBox2DRopeJoint		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif