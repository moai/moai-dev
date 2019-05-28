// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIREGION_H
#define MOAIREGION_H

class MOAIIndexBufferGL;
class MOAITransform;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;

class SafeTesselator;

//================================================================//
// MOAIRegion
//================================================================//
// TODO: doxygen
class MOAIRegion :
	public virtual MOAILuaObject {
private:
	
	static const int DEFAULT_PRECISION = 1000;
	
	ZLLeanArray < ZLPolygon2D > mPolygons;
	
	//----------------------------------------------------------------//
	static int		_append				( lua_State* L );
	static int		_bless				( lua_State* L );
	static int		_boolean			( lua_State* L );
	static int		_clear				( lua_State* L );
	static int		_clip				( lua_State* L );
	static int		_convexHull			( lua_State* L );
	static int		_copy				( lua_State* L );
	static int		_countPolygons		( lua_State* L );
	static int		_cull				( lua_State* L );
	static int		_drawDebug			( lua_State* L );
	static int		_edge				( lua_State* L );
	static int		_findExtremity		( lua_State* L );
	static int		_getDistance		( lua_State* L );
	static int		_getPolygon			( lua_State* L );
	static int		_getTriangles		( lua_State* L );
	static int		_getVertices		( lua_State* L );
	static int		_pad				( lua_State* L );
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
	void					Read					( ZLStream& verts, ZLStream& polySizes );
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

	GET_CONST ( size_t, Size, mPolygons.Size ())

	//----------------------------------------------------------------//
	int						AddFillContours			( SafeTesselator& tess, float precision, u32 mask = 0xffffffff ) const;
	void					Append					( const MOAIRegion& regionA, const MOAIRegion& regionB );
	void					Bless					();
	void					Boolean					( const MOAIRegion& regionA, const MOAIRegion& regionB, u32 operation, float precision );
	void					BooleanAnd				( const MOAIRegion& regionA, const MOAIRegion& regionB, float precision );
	void					BooleanNot				( const MOAIRegion& regionA, const MOAIRegion& regionB, float precision );
	void					BooleanOr				( const MOAIRegion& regionA, const MOAIRegion& regionB, float precision );
	void					BooleanXor				( const MOAIRegion& regionA, const MOAIRegion& regionB, float precision );
	void					Clear					();
	void					Clip					( const MOAIRegion& region, ZLPlane2D plane );
	void					Clip					( const MOAIRegion& region, const MOAIRegion& clip, const ZLAffine3D* mtx = 0 );
	int						CombineAndTesselate		( const MOAIRegion& regionA, const MOAIRegion& regionB, int windingRule, float precision );
	ZLSizeResult			ConvexHull				( ZLStream& vtxStream, size_t nVerts );
	void					Copy					( const MOAIRegion& region );
	void					Copy					( const SafeTesselator& tess );
	void					Copy					( const SafeTesselator& tess, ZLAffine2D& transform );
	void					Cull					( const MOAIRegion& region, u32 flag, bool checkArea = false, float minArea = 0.0f );
	void					DrawDebug				() const;
	void					Edge					( const MOAIRegion& region, const ZLVec2D& offset );
	bool					FindExtremity			( ZLVec2D n, ZLVec2D& e );
	bool					GetDistance				( const ZLVec2D& point, float& d ) const;
	bool					GetDistance				( const ZLVec2D& point, float& d, ZLVec2D& p ) const;
	ZLPolygon2D&			GetPolygon				( ZLIndex idx );
	const ZLPolygon2D&		GetPolygon				( ZLIndex idx ) const;
	u32						GetTriangles			( SafeTesselator& tess ) const;
	u32						GetTriangles			( MOAIVertexFormatGL& format, ZLStream& vtxStream, ZLStream& idxStream ) const;
	u32						GetTriangles			( MOAIVertexFormatGL& format, MOAIVertexBufferGL& vtxBuffer, MOAIIndexBufferGL& idxBuffer, u32 idxSizeInBytex ) const;
	ZLSizeResult			GetVertices				( ZLStream& vtxStream ) const;
							MOAIRegion				();
							~MOAIRegion				();
	void					Pad						( const MOAIRegion& region, float pad );
	bool					PointInside				( const ZLVec2D& p, float pad ) const;
	void					Print					() const;
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	ZLResultCode			ReservePolygons			( ZLSize size );
	ZLResultCode			ReserveVertices			( ZLIndex idx, ZLSize size );
	void					ReverseWinding			( const MOAIRegion& region );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void					SetWinding				( u32 winding );
	void					Snap					( const MOAIRegion& region, float xSnap, float ySnap );
	void					Stroke					( const MOAIRegion& region, float exterior, bool strokeExterior, float interior, bool strokeInterior );
	int						Tesselate				( const MOAIRegion& region, int windingRule, float precision );
	void					Transform				( const MOAIRegion& region, const ZLAffine2D& transform );
};

#endif
