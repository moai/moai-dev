// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DROPEJOINT_H
#define	MOAIBOX2DROPEJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DRopeJoint
//================================================================//
/**	@lua MOAIBox2DRopeJoint
	@text Box2D rope joint.
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
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
};

#endif
