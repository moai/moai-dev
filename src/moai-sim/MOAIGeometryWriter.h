// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGEOMETRYWRITER_H
#define	MOAIGEOMETRYWRITER_H

#include <moai-sim/MOAIVertexFormat.h>

class MOAIGfxBuffer;
class MOAIMesh;
class MOAIVertexFormat;

//================================================================//
// MOAIGeometryWriter
//================================================================//
class MOAIGeometryWriter :
	public MOAIGlobalClass < MOAIGeometryWriter, MOAILuaObject > {
private:
	
	//----------------------------------------------------------------//
	static int			_applyColor				( lua_State* L );
	static int			_applyLinearGradient	( lua_State* L );
	static int			_getMesh				( lua_State* L );
	static int			_pruneVertices			( lua_State* L );
	static int			_snapCoords				( lua_State* L );
	static int			_writeBox				( lua_State* L );
	static int			_writeCone				( lua_State* L );
	static int			_writeCube				( lua_State* L );
	static int			_writeCylinder			( lua_State* L );
	static int			_writeIcoSphere			( lua_State* L );
	static int			_writeUVSphere			( lua_State* L );
	
	//----------------------------------------------------------------//
	static void			WriteQuad				( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 );
	static void			WriteQuad				( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 );
	static void			WriteVertex				( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIGeometryWriter )
	
	//----------------------------------------------------------------//
	static void			ApplyColor				( const MOAIVertexFormat& format, ZLStream& stream, const ZLColorVec& color );
	static void			ApplyLinearGradient		( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 );
	static MOAIMesh*	GetMesh					( const MOAIVertexFormat& format, ZLStream* vtxStream, ZLStream* idxStream, u32 idxSizeInBytes );
	static u32			GetMesh					( const MOAIVertexFormat& format, ZLStream* vtxStream, ZLStream* idxStream, MOAIGfxBuffer* vtxBuffer, MOAIGfxBuffer* idxBuffer, u32 idxSizeInBytes );
						MOAIGeometryWriter		();
						~MOAIGeometryWriter		();
	static void			PruneVertices			( const MOAIVertexFormat& format, MOAIStream& vtxStream, MOAIStream& idxStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	static void			SnapCoords				( const MOAIVertexFormat& format, ZLStream& stream, float xSnap, float ySnap, float zSnap );
	static void			WriteBox				( const MOAIVertexFormat& format, ZLStream& stream, const ZLBox& box );
	static void			WriteCylinder			( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 );
	static void			WriteIcoSphere			( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r );
	static void			WriteUVSphere			( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r );
};

#endif
