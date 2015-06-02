// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGeometryWriter.h>
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// VertexComparator
//================================================================//

//----------------------------------------------------------------//
class VertexComparator {
public:

	ZLStream&					mStream;
	const MOAIVertexFormat&		mVertexFormat;
	
	void*	mBuffer0;
	void*	mBuffer1;
	
	//----------------------------------------------------------------//
	bool operator () ( const size_t v0, const size_t v1 ) const {
		
		size_t vertexSize = this->mVertexFormat.GetVertexSize ();
		
		this->mStream.Seek ( v0, SEEK_SET );
		this->mStream.ReadBytes ( this->mBuffer0, vertexSize );
		
		this->mStream.Seek ( v1, SEEK_SET );
		this->mStream.ReadBytes ( this->mBuffer1, vertexSize );
		
		return ( this->mVertexFormat.Compare ( this->mBuffer0, this->mBuffer1, 0.00001f, 0.00001f ) == -1 );
	}

	//----------------------------------------------------------------//
	VertexComparator ( ZLStream& stream, const MOAIVertexFormat& format, void* buffer0, void* buffer1 ) :
		mStream ( stream ),
		mVertexFormat ( format ),
		mBuffer0 ( buffer0 ),
		mBuffer1 ( buffer1 ) {
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_applyColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		ZLColorVec color = state.GetColor ( 3, 0.0f, 0.0f, 0.0f, 1.0f );
		self->ApplyColor ( *stream, color );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_applyLinearGradient ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		ZLVec3D v0		= state.GetVec3D ( 3, 0.0f );
		ZLVec3D v1		= state.GetVec3D ( 6, 0.0f );
	
		ZLColorVec c0	= state.GetColor ( 9, 0.0f, 0.0f, 0.0f, 1.0f );
		ZLColorVec c1	= state.GetColor ( 13, 0.0f, 0.0f, 0.0f, 1.0f );
		
		bool cap0		= state.GetValue < bool >( 17, false );
		bool cap1		= state.GetValue < bool >( 17, false );
		
		self->ApplyLinearGradient ( *stream, v0, v1, c0, c1, cap0, cap1 );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_getMesh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )

	MOAIStream* vtxStream	= state.GetLuaObject < MOAIStream >( 2, false );
	MOAIStream* idxStream	= state.GetLuaObject < MOAIStream >( 3, false );
	
	if ( vtxStream && idxStream ) {
		u32 idxSizeInBytes = state.GetValue < u32 >( 4, 4 );
		MOAIMesh* mesh = self->GetMesh ( vtxStream, idxStream, idxSizeInBytes );
		state.Push ( mesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIGeometryWriter::_pruneVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* vtxStream	= state.GetLuaObject < MOAIStream >( 2, true );
	MOAIStream* idxStream	= state.GetLuaObject < MOAIStream >( 3, true );
	
	if ( vtxStream && idxStream ) {
		self->PruneVertices ( *vtxStream, *idxStream );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_setVertexFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	self->mVertexFormat.Set ( *self, MOAIVertexFormat::AffirmVertexFormat ( state, 2 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_snapCoords ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		float xSnap = state.GetValue < float >( 2, 0.0f );
		float ySnap = state.GetValue < float >( 3, xSnap );
		float zSnap = state.GetValue < float >( 4, ySnap );
	
		self->SnapCoords ( *stream, xSnap, ySnap, zSnap );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeBox ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
		self->WriteBox ( *stream, state.GetBox ( 3 ));
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeCone ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeCube ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		float size = state.GetValue < float >( 3, 0.0f );
		if ( size > 0.0f ) {
			
			ZLVec3D origin	= state.GetVec3D ( 4, 0.0f );
			
			float halfSize = size * 0.5f;
			
			ZLBox box;
			
			box.mMin.mX = origin.mX - halfSize;
			box.mMin.mY = origin.mY - halfSize;
			box.mMin.mZ = origin.mZ - halfSize;
			
			box.mMax.mX = origin.mX + halfSize;
			box.mMax.mY = origin.mY + halfSize;
			box.mMax.mZ = origin.mZ + halfSize;
	
			self->WriteBox ( *stream, box );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeCylinder ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeIcoSphere ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeUVSphere ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGeometryWriter, "U" )
	return 0;
}

//================================================================//
// MOAIGeometryWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyColor ( ZLStream& stream, const ZLColorVec& color ) {

	assert ( this->mVertexFormat );
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / this->mVertexFormat->GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
		this->mVertexFormat->SeekVertex ( stream, base, i );
		this->mVertexFormat->WriteColor ( stream, color.mR, color.mG, color.mB, color.mA );
	}
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyLinearGradient ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 ) {

	assert ( this->mVertexFormat );
	
	ZLVec3D norm = v1;
	norm.Sub ( v0 );
	float length = norm.Norm ();
	
	ZLPlane3D plane;
	plane.Init ( v0, norm );
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / this->mVertexFormat->GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
	
		this->mVertexFormat->SeekVertex ( stream, base, i );
	
		ZLVec3D coord = this->mVertexFormat->ReadCoord ( stream );
		float dist = ZLDist::VecToPlane ( coord, plane );
		
		ZLColorVec color;
		
		if ( dist <= 0.0f ) {
		
			if ( cap0 && ( dist < 0.0f )) continue;
			color = c0;
		}
		else if ( dist >= length ) {
		
			if ( cap1 && ( dist > length )) continue;
			color = c1;
		}
		else {
		
			color.Lerp ( ZLInterpolate::kLinear, c0, c1, dist / length );
		}
		this->mVertexFormat->WriteColor ( stream, color.mR, color.mG, color.mB, color.mA );
	}
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
MOAIMesh* MOAIGeometryWriter::GetMesh ( ZLStream* vtxStream, ZLStream* idxStream, u32 idxSizeInBytes ) {

	assert ( vtxStream );
	assert ( idxStream );
	
	assert ( this->mVertexFormat );
	
	size_t vtxStreamBase = vtxStream->GetCursor ();
	size_t idxStreamBase = idxStream->GetCursor ();
	
	MOAIGfxBuffer* vtxBuffer = new MOAIGfxBuffer ();
	vtxBuffer->SetTarget ( ZGL_BUFFER_TARGET_ARRAY );
	vtxBuffer->CopyFromStream ( *vtxStream );
	
	MOAIGfxBuffer* idxBuffer = new MOAIGfxBuffer ();
	idxBuffer->SetTarget ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	idxBuffer->CopyFromStream ( *idxStream, idxSizeInBytes, 4 );
	
	vtxStream->Seek ( vtxStreamBase, SEEK_SET );
	idxStream->Seek ( idxStreamBase, SEEK_SET );
	
	MOAIMesh* mesh = new MOAIMesh ();
	
	mesh->SetVertexBuffer ( 0, vtxBuffer, this->mVertexFormat );
	mesh->SetIndexBuffer ( idxBuffer );
	mesh->SetIndexSizeInBytes ( idxSizeInBytes );

	mesh->SetPrimType ( ZGL_PRIM_TRIANGLES );
	mesh->SetTotalElements ( idxBuffer->GetSize () / idxSizeInBytes );
	
	ZLBox bounds;
	this->mVertexFormat->ComputeBounds ( bounds, vtxBuffer->GetReadBuffer (), vtxBuffer->GetSize ());
	
	mesh->SetBounds ( bounds );
	
	return mesh;
}

//----------------------------------------------------------------//
MOAIGeometryWriter::MOAIGeometryWriter () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGeometryWriter::~MOAIGeometryWriter () {

	this->mVertexFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::PruneVertices ( MOAIStream& vtxStream, MOAIStream& idxStream ) {

	size_t vtxStreamBase = vtxStream.GetCursor ();
	size_t idxStreamBase = idxStream.GetCursor ();

	assert ( this->mVertexFormat );

	size_t vtxSize = this->mVertexFormat->GetVertexSize ();
	u32 inputVtxCount = ( u32 )(( vtxStream.GetLength () - vtxStreamBase ) / vtxSize );
	
	if ( !inputVtxCount ) return;
	
	ZLLeanArray < u32 > indexMap;
	indexMap.Init ( inputVtxCount );
	
	void* vtxBuffer0 = alloca ( vtxSize );
	void* vtxBuffer1 = alloca ( vtxSize );

	typedef STLMap < size_t, u32, VertexComparator > VertexMap;
	typedef VertexMap::iterator VertexMapIt;

	VertexMap vertexMap ( VertexComparator ( vtxStream, *this->mVertexFormat, vtxBuffer0, vtxBuffer1 ));
	
	size_t vtxCount = 0;
	
	for ( size_t i = 0; i < inputVtxCount; ++i ) {
	
		size_t vertex = vtxStreamBase + ( i * vtxSize );
		
		u32 index = 0;
		
		VertexMapIt existing = vertexMap.find ( vertex );
		
		if ( existing != vertexMap.end ()) {
			index = existing->second;
			//printf ( "REDUCE: %d -> %d\n", i, index );
		}
		else {
			
			vtxStream.Seek ( vertex, SEEK_SET );
			vtxStream.ReadBytes ( vtxBuffer0, vtxSize );
		
			index = vtxCount++;
			vertex = vtxStreamBase + ( index * vtxSize );
			
			vtxStream.Seek ( vertex, SEEK_SET );
			vtxStream.WriteBytes ( vtxBuffer0, vtxSize );
			
			vertexMap [ vertex ] = index;
		}
		indexMap [ i ] = index;
		
		//printf ( "INDEX: %d -> %d\n", i, index );
	}
	
	size_t totalIdx = idxStream.GetLength () >> 2; // index size is assumed always be 4 bytes
	
	if ( totalIdx ) {
	
		for ( size_t i = 0; i < totalIdx; ++i ) {
		
			u32 idx = idxStream.Read < u32 >( 0 );
			idx = indexMap [ idx ];
			idxStream.Seek ( -4, SEEK_CUR );
			idxStream.Write < u32 >( idx );
		}
	}
	else {
		
		for ( size_t i = 0; i < inputVtxCount; ++i ) {
			
			u32 idx = indexMap [ i ];
			idxStream.Write < u32 >( idx );
		}
	}
	
	if ( vtxCount < inputVtxCount ) {
		vtxStream.SetLength ( vtxCount * vtxSize );
		vtxStream.Compact ();
	}
	
	vtxStream.Seek ( vtxStreamBase, SEEK_SET );
	idxStream.Seek ( idxStreamBase, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "applyColor",				_applyColor },
		{ "applyLinearGradient",	_applyLinearGradient },
		{ "getMesh",				_getMesh },
		{ "pruneVertices",			_pruneVertices },
		{ "setVertexFormat",		_setVertexFormat },
		{ "snapCoords",				_snapCoords },
		{ "writeBox",				_writeBox },
		{ "writeCone",				_writeCone },
		{ "writeCube",				_writeCube },
		{ "writeCylinder",			_writeCylinder },
		{ "writeIcoSphere",			_writeIcoSphere },
		{ "writeUVSphere",			_writeUVSphere },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::SnapCoords ( ZLStream& stream, float xSnap, float ySnap, float zSnap ) {

	assert ( this->mVertexFormat );
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / this->mVertexFormat->GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
		
		this->mVertexFormat->SeekVertex ( stream, base, i );
		
		ZLVec4D coord = this->mVertexFormat->ReadCoord ( stream );
		
		coord.mX = xSnap == 0.0f ? coord.mX : floorf (( coord.mX / xSnap ) + 0.5f ) * xSnap;
		coord.mY = ySnap == 0.0f ? coord.mY : floorf (( coord.mY / ySnap ) + 0.5f ) * ySnap;
		coord.mZ = zSnap == 0.0f ? coord.mZ : floorf (( coord.mZ / zSnap ) + 0.5f ) * zSnap;
		
		this->mVertexFormat->WriteCoord ( stream, coord.mX, coord.mY, coord.mZ, coord.mW );
	}
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteBox ( ZLStream& stream, const ZLBox& box ) {

	assert ( this->mVertexFormat );

	float xMin = box.mMin.mX;
	float yMin = box.mMin.mY;
	float zMin = box.mMin.mZ;
	
	float xMax = box.mMax.mX;
	float yMax = box.mMax.mY;
	float zMax = box.mMax.mZ;
	
	// back
	ZLVec3D v0 ( xMax, yMin, zMin );
	ZLVec3D v1 ( xMin, yMin, zMin );
	ZLVec3D v2 ( xMin, yMax, zMin );
	ZLVec3D v3 ( xMax, yMax, zMin );

	// front
	ZLVec3D v4 ( xMin, yMin, zMax );
	ZLVec3D v5 ( xMax, yMin, zMax );
	ZLVec3D v6 ( xMax, yMax, zMax );
	ZLVec3D v7 ( xMin, yMax, zMax );

	size_t base = stream.GetCursor ();

	this->WriteQuad ( stream, v0, v1, v2, v3, ZLVec3D ( 0.0f, 0.0f, -1.0f )); // back
	this->WriteQuad ( stream, v4, v5, v6, v7, ZLVec3D ( 0.0f, 0.0f, 1.0f )); // front
	
	this->WriteQuad ( stream, v1, v4, v7, v2, ZLVec3D ( -1.0f, 0.0f, 0.0f )); // left
	this->WriteQuad ( stream, v5, v0, v3, v6, ZLVec3D ( 1.0f, 0.0f, 0.0f )); // right

	this->WriteQuad ( stream, v5, v4, v1, v0, ZLVec3D ( 0.0f, -1.0f, 0.0f )); // bottom
	this->WriteQuad ( stream, v2, v7, v6, v3, ZLVec3D ( 0.0f, 1.0f, 0.0f )); // top
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteCylinder ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 ) {

	assert ( this->mVertexFormat );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteIcoSphere ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r ) {

	assert ( this->mVertexFormat );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteQuad ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 ) {

	this->WriteQuad ( stream, v0, v1, v2, v3, n0, n0, n0, n0 );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteQuad ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 ) {

	this->WriteVertex ( stream, v0, n0 );
	this->WriteVertex ( stream, v1, n1 );
	this->WriteVertex ( stream, v2, n2 );
	
	this->WriteVertex ( stream, v0, n0 );
	this->WriteVertex ( stream, v2, n2 );
	this->WriteVertex ( stream, v3, n3 );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteUVSphere ( ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r ) {

	assert ( this->mVertexFormat );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteVertex ( ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal ) {

	size_t base = stream.GetCursor ();
	this->mVertexFormat->WriteAhead ( stream );
	this->mVertexFormat->WriteCoord ( stream, coord.mX, coord.mY, coord.mZ, 1.0f );
	this->mVertexFormat->WriteNormal ( stream, normal.mX, normal.mY, normal.mZ );
	this->mVertexFormat->SeekVertex ( stream, base, 1 );
}
