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
	public virtual MOAILuaObject {
private:
	
	ZLLeanArray < ZLPolygon2D > mPolygons;
	
	//----------------------------------------------------------------//
	static int		_bless				( lua_State* L );
	static int		_drawDebug			( lua_State* L );
	static int		_getDistance		( lua_State* L );
	static int		_getTriangles		( lua_State* L );
	static int		_pointInside		( lua_State* L );
	static int		_reservePolygons	( lua_State* L );
	static int		_reserveVertices	( lua_State* L );
	static int		_setVertex			( lua_State* L );
	static int		_translatePolygon	( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIRegion )

	//----------------------------------------------------------------//
	void					DrawDebug				() const;
	bool					GetDistance				( const ZLVec2D& point, float& d, ZLVec2D& p ) const;
	ZLPolygon2D&			GetPolygon				( u32 idx );
	const ZLPolygon2D&		GetPolygon				( u32 idx ) const;
	u32						GetTriangles			( MOAIGfxBuffer& vtxBuffer, MOAIGfxBuffer& idxBuffer, u32 idxSizeInBytex ) const;
							MOAIRegion				();
							~MOAIRegion				();
	bool					PointInside				( const ZLVec2D& p ) const;
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ReservePolygons			( u32 size );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
