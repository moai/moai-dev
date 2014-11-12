// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIREGION_H
#define MOAIREGION_H

//================================================================//
// MOAIRegion
//================================================================//
// TODO: doxygen
class MOAIRegion :
	public virtual MOAILuaObject,
	public ZLLeanArray < ZLPolygon2D > {
private:
	
	//----------------------------------------------------------------//
	static int		_pointInside		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIRegion )

	//----------------------------------------------------------------//
					MOAIRegion			();
					~MOAIRegion			();
	bool			PointInside			( const ZLVec2D& p );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
