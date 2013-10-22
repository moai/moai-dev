// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMATH_H
#define MOAIMATH_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIMath
//================================================================//
// TODO: doxygen
class MOAIMath :
	public MOAIGlobalClass < MOAIMath, MOAILuaObject > {
private:
	
	struct SFMT_T*	mSFMT;
	
	//----------------------------------------------------------------//
	static int		_pointsForBezierCurve			( lua_State* L );
	static int		_pointsForCardinalSpline		( lua_State* L );
	static int		_pointsForCardinalSplineLoop	( lua_State* L );
	static int		_randSFMT						( lua_State* L );
	static int		_seedSFMT						( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMath )

	//----------------------------------------------------------------//
					MOAIMath						();
					~MOAIMath						();
	static int		PointsForCardinalSpline			( lua_State* L, u32 subdivide, float tension);
	static int		PointsForCardinalSplineLoop		( lua_State* L, u32 subdivide, float tension);
	void			RegisterLuaClass				( MOAILuaState& state );
};

#endif
