// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHWRITER_H
#define	MOAIMESHWRITER_H

class MOAIRegion;

//================================================================//
// MOAIMeshWriter
//================================================================//
/**	@lua	MOAIMeshWriter
	@text	Interface for writing and manipulating procedural geometry.
			Geometry is written as vertices to a stream. Vertex layout is
			given by a MOAIVertexFormat. Stream cursor is reset to starting
			location after each write (to make writing multiple passes
			easier).
			
			Operations that affect every vertex of a mesh affect all
			vertices between the stream cursor and the end of the stream.
 
			Once all vertices have been written, stream may be converted
			into a mesh with an index buffer that is calculated
			by finding redundant vertices.
*/
class MOAIMeshWriter :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int				_applyColor					( lua_State* L );
	static int				_applyLightFromImage		( lua_State* L );
	static int				_applyLinearGradient		( lua_State* L );
	static int				_getMesh					( lua_State* L );
	static int				_offsetIndices				( lua_State* L );
	static int				_pruneVertices				( lua_State* L );
	static int				_snapCoords					( lua_State* L );
	static int				_translateCoords			( lua_State* L );
	static int				_writeBox					( lua_State* L );
	static int				_writeCone					( lua_State* L );
	static int				_writeCube					( lua_State* L );
	static int				_writeCylinder				( lua_State* L );
	static int				_writeIcoSphere				( lua_State* L );
	static int				_writeUVSphere				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					WriteColor					( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, float r, float g, float b, float a );
	void					WriteQuad					( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 );
	void					WriteQuad					( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 );
	void					WriteVertex					( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIMeshWriter )
	
	enum {
		COLOR_ADD,
		COLOR_MULTIPLY,
		COLOR_OVERWRITE,
		COLOR_SUBTRACT,
	};
	
	//----------------------------------------------------------------//
	void					ApplyColor					( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, u32 mode, const ZLColorVec& color );
	void					ApplyColor					( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, u32 mode, const MOAIRegion& region, float pad, const ZLColorVec& color );
	void					ApplyLightFromImage			( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, u32 mode, MOAIImage& image, bool gradient, float a0, float a1, const ZLVec3D& v0, const ZLVec3D& v1 );
	void					ApplyLinearGradient			( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, u32 mode, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 );
	MOAIMeshDeck*			GetMesh						( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t vtxStreamLength, ZLStream& idxStream, size_t idxStreamLength, u32 idxSizeInBytes );
	u32						GetMesh						( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t vtxStreamLength, ZLStream& idxStream, size_t idxStreamLength, MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, u32 idxSizeInBytes );
							MOAIMeshWriter				( ZLContext& context );
							~MOAIMeshWriter				();
	void					OffsetIndices				( ZLStream& idxStream, size_t length, s32 offset );
	void					PruneVertices				( const MOAIVertexFormat& format, MOAIStream& vtxStream, MOAIStream& idxStream );
	void					SnapCoords					( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, float xSnap, float ySnap, float zSnap );
	void					TranslateCoords				( const MOAIVertexFormat& format, ZLStream& vtxStream, size_t length, float xOff, float yOff, float zOff );
	void					WriteBox					( const MOAIVertexFormat& format, ZLStream& vtxStream, const ZLBox& box );
	void					WriteCylinder				( const MOAIVertexFormat& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 );
	static void				WriteIcoSphere				( const MOAIVertexFormat& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& r );
	void					WriteUVSphere				( const MOAIVertexFormat& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& r );
};

#endif
