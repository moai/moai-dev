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
	static int		_boolean			( lua_State* L );
	static int		_drawDebug			( lua_State* L );
	static int		_edge				( lua_State* L );
	static int		_getDistance		( lua_State* L );
	static int		_getTriangles		( lua_State* L );
	static int		_pointInside		( lua_State* L );
	static int		_reservePolygons	( lua_State* L );
	static int		_reserveVertices	( lua_State* L );
	static int		_setVertex			( lua_State* L );
	static int		_stroke				( lua_State* L );


public:
	
	enum {
		BOOLEAN_AND,
		BOOLEAN_NOT,
		BOOLEAN_OR,
		BOOLEAN_XOR,
	};
	
	DECL_LUA_FACTORY ( MOAIRegion )

	GET ( size_t, Size, mPolygons.Size ())

	//----------------------------------------------------------------//
	int						AddFillContours			( SafeTesselator& tess, u32 mask = 0xffffffff ) const;
	//int						AddStrokeContours		( SafeTesselator& tess, bool inside, bool outside );
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
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					ReservePolygons			( u32 size );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void					Stroke					( const MOAIRegion& region, float exterior, bool strokeExterior, float interior, bool strokeInterior );
	void					Transform				( const ZLAffine2D& transform );
};

#endif
