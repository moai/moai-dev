// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBOX2DWELDJOINT_H
#define	MOAIBOX2DWELDJOINT_H

#include <moai-box2d/MOAIBox2DJoint.h>

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//
/**	@lua MOAIBox2DWeldJoint
	@text Box2D weld joint.
*/
class MOAIBox2DWeldJoint :
	public MOAIBox2DJoint {
private:

	//----------------------------------------------------------------//
	static int		_getDampingRatio		( lua_State* L );
	static int		_getFrequency			( lua_State* L );
	static int		_setDampingRatio		( lua_State* L );
	static int		_setFrequency			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIBox2DWeldJoint )
	
	//----------------------------------------------------------------//
					MOAIBox2DWeldJoint		();
					~MOAIBox2DWeldJoint		();
};

#endif
