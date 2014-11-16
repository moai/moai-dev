// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIREGION_H
#define MOAIREGION_H

class MOAIIndexBuffer;
class MOAIVertexBuffer;

//================================================================//
// MOAIRegion
//================================================================//
// TODO: doxygen
class MOAIRegion :
	public virtual MOAILuaObject,
	public ZLLeanArray < ZLPolygon2D > {
private:
	
	//----------------------------------------------------------------//
	static int		_getTriangles		( lua_State* L );
	static int		_pointInside		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIRegion )

	//----------------------------------------------------------------//
	void			GetTriangles		( MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer );
					MOAIRegion			();
					~MOAIRegion			();
	bool			PointInside			( const ZLVec2D& p );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
