// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGEOMETRYWRITER_H
#define	MOAIGEOMETRYWRITER_H

class MOAIIndexBufferGL;
class MOAIMesh;
class MOAIRegion;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;

//================================================================//
// MOAIGeometryWriter
//================================================================//
/**	@lua	MOAIGeometryWriter
	@text	Interface for writing and manipulating procedural geometry.
			Geometry is written as vertices to a stream. Vertex layout is
			given by a MOAIVertexFormatGL. Stream cursor is reset to starting
			location after each write (to make writing multiple passes
			easier).
			
			Operations that affect every vertex of a mesh affect all
			vertices between the stream cursor and the end of the stream.
 
			Once all vertices have been written, stream may be converted
			into a mesh with an index buffer that is calculated
			by finding redundant vertices.
*/
class MOAIGeometryWriter :
	public ZLContextClass < MOAIGeometryWriter >,
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int			_applyColor				( lua_State* L );
	static int			_applyLightFromImage	( lua_State* L );
	static int			_applyLinearGradient	( lua_State* L );
	static int			_getMesh				( lua_State* L );
	static int			_offsetIndices			( lua_State* L );
	static int			_pruneVertices			( lua_State* L );
	static int			_snapCoords				( lua_State* L );
	static int			_translateCoords		( lua_State* L );
	static int			_writeBox				( lua_State* L );
	static int			_writeCone				( lua_State* L );
	static int			_writeCube				( lua_State* L );
	static int			_writeCylinder			( lua_State* L );
	static int			_writeIcoSphere			( lua_State* L );
	static int			_writeUVSphere			( lua_State* L );
	
	//----------------------------------------------------------------//
	static void			WriteColor				( const MOAIVertexFormatGL& format, ZLStream& stream, u32 mode, float r, float g, float b, float a );
	static void			WriteQuad				( const MOAIVertexFormatGL& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 );
	static void			WriteQuad				( const MOAIVertexFormatGL& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 );
	static void			WriteVertex				( const MOAIVertexFormatGL& format, ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIGeometryWriter )
	
	enum {
		COLOR_ADD,
		COLOR_MULTIPLY,
		COLOR_OVERWRITE,
		COLOR_SUBTRACT,
	};
	
	//----------------------------------------------------------------//
	static void			ApplyColor				( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, u32 mode, const ZLColorVec& color );
	static void			ApplyColor				( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, u32 mode, const MOAIRegion& region, float pad, const ZLColorVec& color );
	static void			ApplyLightFromImage		( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, u32 mode, MOAIImage& image, bool gradient, float a0, float a1, const ZLVec3D& v0, const ZLVec3D& v1 );
	static void			ApplyLinearGradient		( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, u32 mode, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 );
	static MOAIMesh*	GetMesh					( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t vtxStreamLength, ZLStream& idxStream, size_t idxStreamLength, u32 idxSizeInBytes );
	static u32			GetMesh					( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t vtxStreamLength, ZLStream& idxStream, size_t idxStreamLength, MOAIVertexBufferGL& vtxBuffer, MOAIIndexBufferGL& idxBuffer, u32 idxSizeInBytes );
						MOAIGeometryWriter		();
						~MOAIGeometryWriter		();
	static void			OffsetIndices			( ZLStream& idxStream, size_t length, s32 offset );
	static void			PruneVertices			( const MOAIVertexFormatGL& format, MOAIStream& vtxStream, MOAIStream& idxStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	static void			SnapCoords				( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, float xSnap, float ySnap, float zSnap );
	static void			TranslateCoords			( const MOAIVertexFormatGL& format, ZLStream& vtxStream, size_t length, float xOff, float yOff, float zOff );
	static void			WriteBox				( const MOAIVertexFormatGL& format, ZLStream& vtxStream, const ZLBox& box );
	static void			WriteCylinder			( const MOAIVertexFormatGL& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 );
	static void			WriteIcoSphere			( const MOAIVertexFormatGL& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& r );
	static void			WriteUVSphere			( const MOAIVertexFormatGL& format, ZLStream& vtxStream, const ZLVec3D& v0, const ZLVec3D& r );
};

#endif
