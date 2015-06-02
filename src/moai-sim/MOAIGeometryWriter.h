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
	public virtual MOAILuaObject {
private:
	
	MOAILuaSharedPtr < MOAIVertexFormat > mVertexFormat;
	
	//----------------------------------------------------------------//
	static int		_applyColor				( lua_State* L );
	static int		_applyLinearGradient	( lua_State* L );
	static int		_getMesh				( lua_State* L );
	static int		_pruneVertices			( lua_State* L );
	static int		_setVertexFormat		( lua_State* L );
	static int		_snapCoords				( lua_State* L );
	static int		_writeBox				( lua_State* L );
	static int		_writeCone				( lua_State* L );
	static int		_writeCube				( lua_State* L );
	static int		_writeCylinder			( lua_State* L );
	static int		_writeIcoSphere			( lua_State* L );
	static int		_writeUVSphere			( lua_State* L );
	
	//----------------------------------------------------------------//
	void			WriteQuad				( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 );
	void			WriteQuad				( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 );
	void			WriteVertex				( ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGeometryWriter )
	
	//----------------------------------------------------------------//
	void			ApplyColor				( ZLStream& stream, const ZLColorVec& color );
	void			ApplyLinearGradient		( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 );
	MOAIMesh*		GetMesh					( ZLStream* vtxStream, ZLStream* idxStream, u32 idxSizeInBytes );
					MOAIGeometryWriter		();
					~MOAIGeometryWriter		();
	void			PruneVertices			( MOAIStream& vtxStream, MOAIStream& idxStream );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SnapCoords				( ZLStream& stream, float xSnap, float ySnap, float zSnap );
	void			WriteBox				( ZLStream& stream, const ZLBox& box );
	void			WriteCylinder			( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 );
	void			WriteIcoSphere			( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r );
	void			WriteUVSphere			( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r );
};

#endif
