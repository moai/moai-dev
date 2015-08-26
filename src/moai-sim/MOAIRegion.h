// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIREGION_H
#define MOAIREGION_H

class MOAIIndexBuffer;
class MOAIVertexBuffer;
class MOAIVertexFormat;

class SafeTesselator;

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
	static int		_copy				( lua_State* L );
	static int		_cull				( lua_State* L );
	static int		_boolean			( lua_State* L );
	static int		_drawDebug			( lua_State* L );
	static int		_edge				( lua_State* L );
	static int		_getDistance		( lua_State* L );
	static int		_getTriangles		( lua_State* L );
	static int		_pointInside		( lua_State* L );
	static int		_print				( lua_State* L );
	static int		_reservePolygons	( lua_State* L );
	static int		_reserveVertices	( lua_State* L );
	static int		_reverseWinding		( lua_State* L );
	static int		_setVertex			( lua_State* L );
	static int		_setWinding			( lua_State* L );
	static int		_snap				( lua_State* L );
	static int		_stroke				( lua_State* L );
	static int		_tesselate			( lua_State* L );
	static int		_translate			( lua_State* L );

	//----------------------------------------------------------------//
	bool					ShouldCull				( const ZLPolygon2D& poly, u32 flag, bool checkArea, float minArea );

public:
	
	enum {
		BOOLEAN_AND,
		BOOLEAN_NOT,
		BOOLEAN_OR,
		BOOLEAN_XOR,
	};
	
	enum {
		WINDING_CLOCKWISE,
		WINDING_ANTICLOCKWISE,
	};
	
	DECL_LUA_FACTORY ( MOAIRegion )

	GET ( size_t, Size, mPolygons.Size ())

	//----------------------------------------------------------------//
	int						AddFillContours			( SafeTesselator& tess, u32 mask = 0xffffffff ) const;
	void					Bless					();
	void					Boolean					( const MOAIRegion& regionA, const MOAIRegion& regionB, u32 operation );
	void					BooleanAnd				( const MOAIRegion& regionA, const MOAIRegion& regionB );
	void					BooleanNot				( const MOAIRegion& regionA, const MOAIRegion& regionB );
	void					BooleanOr				( const MOAIRegion& regionA, const MOAIRegion& regionB );
	void					BooleanXor				( const MOAIRegion& regionA, const MOAIRegion& regionB );
	int						CombineAndTesselate		( const MOAIRegion& regionA, const MOAIRegion& regionB, int windingRule );
	void					Copy					( const MOAIRegion& region );
	void					Copy					( const SafeTesselator& tess );
	void					Copy					( const SafeTesselator& tess, ZLAffine2D& transform );
	void					Cull					( const MOAIRegion& region, u32 flag, bool checkArea = false, float minArea = 0.0f );
	void					DrawDebug				() const;
	void					Edge					( const MOAIRegion& region, const ZLVec2D& offset );
	bool					GetDistance				( const ZLVec2D& point, float& d ) const;
	bool					GetDistance				( const ZLVec2D& point, float& d, ZLVec2D& p ) const;
	ZLPolygon2D&			GetPolygon				( u32 idx );
	const ZLPolygon2D&		GetPolygon				( u32 idx ) const;
	u32						GetTriangles			( SafeTesselator& tess ) const;
	u32						GetTriangles			( MOAIVertexFormat& format, ZLStream& vtxStream, ZLStream& idxStream ) const;
	u32						GetTriangles			( MOAIVertexFormat& format, MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, u32 idxSizeInBytex ) const;
							MOAIRegion				();
							~MOAIRegion				();
	bool					PointInside				( const ZLVec2D& p, float pad ) const;
	void					Print					() const;
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ReservePolygons			( u32 size );
	void					ReverseWinding			( const MOAIRegion& region );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void					SetWinding				( u32 winding );
	void					SetWinding				( u32 oldWinding, u32 newWinding );
	void					Snap					( const MOAIRegion& region, float xSnap, float ySnap );
	void					Stroke					( const MOAIRegion& region, float exterior, bool strokeExterior, float interior, bool strokeInterior );
	int						Tesselate				( const MOAIRegion& region, int windingRule );
	void					Transform				( const MOAIRegion& region, const ZLAffine2D& transform );
};

#endif
