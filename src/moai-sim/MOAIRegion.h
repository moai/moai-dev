// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIREGION_H
#define MOAIREGION_H

class MOAIGfxBuffer;

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
	u32				GetTriangles		( MOAIGfxBuffer& vtxBuffer, MOAIGfxBuffer& idxBuffer, u32 idxSizeInBytex );
					MOAIRegion			();
					~MOAIRegion			();
	bool			PointInside			( const ZLVec2D& p );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
