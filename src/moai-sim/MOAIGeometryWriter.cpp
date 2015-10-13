 // Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGeometryWriter.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIVertexBuffer.h>
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
/**	@lua	applyColor
	@text	Apply a color to every vertex in a mesh. A MOAIRegion and/or
			a blending mode may optionally be provided.
			
			Blending mode is one of COLOR_ADD, COLOR_MULTIPLY, COLOR_OVERWRITE,
			COLOR_SUBTRACT.

	@overload
	
		@in		MOAIVertexFormat format
		@in		MOAIStream vtxStream
		@in		ZLColorVec color
		@opt	number blendMode
		@out 	nil
	
	@overload
	
		@in		MOAIVertexFormat format
		@in		MOAIStream stream
		@in		ZLColorVec color
		@in		MOAIRegion region
		@opt	number pad
		@opt	number blendMode
		@out 	nil
*/
int MOAIGeometryWriter::_applyColor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( format && stream ) {
	
		ZLColorVec color	= state.GetColor ( 3, 0.0f, 0.0f, 0.0f, 1.0f );
		MOAIRegion* region	= state.GetLuaObject < MOAIRegion >( 7, false );
		
		if ( region ) {
		
			float pad	= state.GetValue < float >( 8, 0.0f );
			u32 mode	= state.GetValue < u32 >( 9, COLOR_MULTIPLY );
			
			MOAIGeometryWriter::ApplyColor ( *format, *stream, mode, *region, pad, color );
		}
		else {
		
			u32 mode	= state.GetValue < u32 >( 7, COLOR_MULTIPLY );
			MOAIGeometryWriter::ApplyColor ( *format, *stream, mode, color );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	applyLightFromImage
	@text	Apply vertex colors from an image as a sperical map indexed by
			vertex normals. Middle of image is the equator, with the poles at
			image top and bottom. (Resolution diminishes as sample approaches
			poles.)
			
			An optional linear alpha gradient may be also be applied.
			
			Blending mode is one of COLOR_ADD, COLOR_MULTIPLY, COLOR_OVERWRITE,
			COLOR_SUBTRACT.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@in		MOAIImage image
	@opt	number blendMode
	@opt	number a0
	@opt	number a1
	@opt	number x0
	@opt	number y0
	@opt	number z0
	@opt	number x1
	@opt	number y1
	@opt	number z1
	@out 	nil
*/
int MOAIGeometryWriter::_applyLightFromImage ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	MOAIImage* image			= MOAIImage::AffirmImage ( state, 3 );
	u32 mode					= state.GetValue < u32 >( 4, COLOR_MULTIPLY );
	
	bool gradient				= state.CheckParams ( 5, "NNNNNNNN", false );
	
	float a0					= state.GetValue < float >( 5, 0.0f );
	float a1					= state.GetValue < float >( 6, 0.0f );
	
	ZLVec3D v0					= state.GetVec3D ( 7, 0.0f );
	ZLVec3D v1					= state.GetVec3D ( 10, 0.0f );
	
	if ( format && stream && image ) {
		self->ApplyLightFromImage ( *format, *stream, mode, *image, gradient, a0, a1, v0, v1 );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	applyLinearGradient
	@text	Apply a linear color gradient.
			
			Blending mode is one of COLOR_ADD, COLOR_MULTIPLY, COLOR_OVERWRITE,
			COLOR_SUBTRACT.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@opt	number x0
	@opt	number y0
	@opt	number z0
	@opt	number x1
	@opt	number y1
	@opt	number z1
	@opt	number r0
	@opt	number g0
	@opt	number b0
	@opt	number a0
	@opt	number r1
	@opt	number g1
	@opt	number b1
	@opt	number a1
	@opt	boolean cap0
	@opt	boolean cap1
	@opt	number blendMode
	@out 	nil
*/
int MOAIGeometryWriter::_applyLinearGradient ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( format && stream  ) {
	
		ZLVec3D v0		= state.GetVec3D ( 3, 0.0f );
		ZLVec3D v1		= state.GetVec3D ( 6, 0.0f );
	
		ZLColorVec c0	= state.GetColor ( 9, 0.0f, 0.0f, 0.0f, 1.0f );
		ZLColorVec c1	= state.GetColor ( 13, 0.0f, 0.0f, 0.0f, 1.0f );
		
		bool cap0		= state.GetValue < bool >( 17, false );
		bool cap1		= state.GetValue < bool >( 18, false );
		
		u32 mode		= state.GetValue < u32 >( 19, COLOR_MULTIPLY );
		
		MOAIGeometryWriter::ApplyLinearGradient ( *format, *stream, mode, v0, v1, c0, c1, cap0, cap1 );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getMesh
	@text	Return the vertices as an MOAIVertexBuffer and MOAIIndexBuffer, or
			as a ready-to-render MOAIMesh.
			
	@override
	
		@in		MOAIVertexFormat format
		@in		MOAIStream vtxStream
		@in		MOAIStream idxStream
		@in		MOAIVertexBuffer vtxBuffer
		@in		MOAIIndexBuffer idxBuffer
		@opt	number idxSizeInBytes
		@out 	number totalElements
	
	@override
	
		@in		MOAIVertexFormat format
		@in		MOAIStream vtxStream
		@in		MOAIStream idxStream
		@opt	number idxSizeInBytes
		@out 	MOAIMesh mesh
*/
int MOAIGeometryWriter::_getMesh ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )

	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* vtxStream		= state.GetLuaObject < MOAIStream >( 2, false );
	MOAIStream* idxStream		= state.GetLuaObject < MOAIStream >( 3, false );
	
	if ( format && vtxStream && idxStream ) {
	
		MOAIVertexBuffer* vtxBuffer		= state.GetLuaObject < MOAIVertexBuffer >( 4, false );
		MOAIIndexBuffer* idxBuffer		= state.GetLuaObject < MOAIIndexBuffer >( 5, false );
	
		if ( vtxBuffer && idxBuffer ) {
			
			u32 idxSizeInBytes = state.GetValue < u32 >( 6, 4 );
			u32 totalElements = MOAIGeometryWriter::GetMesh ( *format, *vtxStream, *idxStream, *vtxBuffer, *idxBuffer, idxSizeInBytes );
			
			state.Push ( totalElements );
			return 1;
		}
		else {
	
			u32 idxSizeInBytes = state.GetValue < u32 >( 4, 4 );
			MOAIMesh* mesh = MOAIGeometryWriter::GetMesh ( *format, *vtxStream, *idxStream, idxSizeInBytes );
			state.Push ( mesh );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	pruneVertices
	@text	Remove duplicate vertices from the vertex stream and update
			or create the index stream. Index stream is assuming to be 4 bytes.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@in		MOAIStream idxStream
	@out 	nil
*/
int MOAIGeometryWriter::_pruneVertices ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* vtxStream		= state.GetLuaObject < MOAIStream >( 2, true );
	MOAIStream* idxStream		= state.GetLuaObject < MOAIStream >( 3, true );
	
	if ( format && vtxStream && idxStream ) {
		MOAIGeometryWriter::PruneVertices ( *format, *vtxStream, *idxStream );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	snapCoords
	@text	Snap vertex coordinates to a give resolution.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@in		number xSnap
	@opt	number ySnap		Default value is xSnap.
	@opt	number zSnap		Default valie is ySnap.
	@out 	nil
*/
int MOAIGeometryWriter::_snapCoords ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( format && stream  ) {
	
		float xSnap = state.GetValue < float >( 2, 0.0f );
		float ySnap = state.GetValue < float >( 3, xSnap );
		float zSnap = state.GetValue < float >( 4, ySnap );
	
		MOAIGeometryWriter::SnapCoords ( *format, *stream, xSnap, ySnap, zSnap );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	writeBox
	@text	Writes the vertices of a box.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@in		number xMin
	@in		number yMin
	@in		number zMin
	@in		number xMax
	@in		number yMax
	@in		number zMax
	@out 	nil
*/
int MOAIGeometryWriter::_writeBox ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( format && stream  ) {
		MOAIGeometryWriter::WriteBox ( *format, *stream, state.GetBox ( 3 ));
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeCone ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	writeCube
	@text	Writes the vertices of a cube.
	
	@in		MOAIVertexFormat format
	@in		MOAIStream vtxStream
	@opt	number size		Default value is 1.
	@opt	number x		Default value is 0.
	@opt	number y		Default value is 0.
	@opt	number z		Default value is 0.
	@out 	nil
*/
int MOAIGeometryWriter::_writeCube ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 1, true );
	MOAIStream* stream			= state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( format && stream  ) {
	
		float size = state.GetValue < float >( 3, 1.0f );
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
	
			MOAIGeometryWriter::WriteBox ( *format, *stream, box );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeCylinder ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeIcoSphere ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGeometryWriter::_writeUVSphere ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGeometryWriter, "" )
	return 0;
}

//================================================================//
// MOAIGeometryWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyColor ( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, const ZLColorVec& color ) {
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / format.GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
		format.SeekVertex ( stream, base, i );
		MOAIGeometryWriter::WriteColor ( format, stream, mode, color.mR, color.mG, color.mB, color.mA );
	}
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyColor ( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, const MOAIRegion& region, float pad, const ZLColorVec& color ) {
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / format.GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
	
		format.SeekVertex ( stream, base, i );

		ZLVec3D coord = format.ReadCoord ( stream );
		ZLVec2D point ( coord.mX, coord.mY );

		if ( region.PointInside ( point, pad )) {
			MOAIGeometryWriter::WriteColor ( format, stream, mode, color.mR, color.mG, color.mB, color.mA );
		}
	}
	
	stream.Seek ( base, SEEK_SET );
}


//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyLightFromImage ( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, MOAIImage& image, bool gradient, float a0, float a1, const ZLVec3D& v0, const ZLVec3D& v1 ) {
	
	float width		= image.GetWidth ();
	float height	= image.GetHeight ();
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / format.GetVertexSize ());
	
	ZLVec3D norm = v1;
	norm.Sub ( v0 );
	float length = norm.Norm ();
	
	ZLPlane3D plane;
	plane.Init ( v0, norm );
	
	for ( u32 i = 0; i < total; ++i ) {
	
		format.SeekVertex ( stream, base, i );
	
		float alpha = 1.0f;
	
		if ( gradient ) {
			
			ZLVec3D coord = format.ReadCoord ( stream );
			float dist = ZLDist::VecToPlane ( coord, plane );
			
			if ( dist <= 0.0f ) {
				alpha = a0;
			}
			else if ( dist >= length ) {
				alpha = a1;
			}
			else {
				alpha = a0 + (( a1 - a0 ) * ( dist / length ));
			}
		}
		
		ZLVec3D normal = format.ReadNormal ( stream );

		ZLVec2D hVec ( normal.mX, normal.mY );
		hVec.Norm ();

		float x = ( hVec.Radians () / TWOPI ) * width;
		float y = ( normal.Radians ( ZLVec3D::Z_AXIS ) / PI ) * height;

		u32 sampleColor = image.SampleColor ( x, y, MOAIImage::FILTER_LINEAR, true, true );
		ZLColorVec color ( sampleColor );

		MOAIGeometryWriter::WriteColor ( format, stream, mode, color.mR, color.mG, color.mB, color.mA * alpha );
	}
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::ApplyLinearGradient ( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, const ZLVec3D& v0, const ZLVec3D& v1, const ZLColorVec& c0, const ZLColorVec& c1, bool cap0, bool cap1 ) {
	
	ZLVec3D norm = v1;
	norm.Sub ( v0 );
	float length = norm.Norm ();
	
	ZLPlane3D plane;
	plane.Init ( v0, norm );
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / format.GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
	
		format.SeekVertex ( stream, base, i );
	
		ZLVec3D coord = format.ReadCoord ( stream );
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
		MOAIGeometryWriter::WriteColor ( format, stream, mode, color.mR, color.mG, color.mB, color.mA );
	}
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
MOAIMesh* MOAIGeometryWriter::GetMesh ( const MOAIVertexFormat& format, ZLStream& vtxStream, ZLStream& idxStream, u32 idxSizeInBytes ) {

	MOAIVertexBuffer* vtxBuffer = new MOAIVertexBuffer ();
	MOAIIndexBuffer* idxBuffer = new MOAIIndexBuffer ();

	u32 totalElements = MOAIGeometryWriter::GetMesh ( format, vtxStream, idxStream, *vtxBuffer, *idxBuffer, idxSizeInBytes );
	
	MOAIMesh* mesh = new MOAIMesh ();
	
	mesh->SetVertexBuffer ( 0, vtxBuffer, ( MOAIVertexFormat* )&format ); // not ideal, but we're gonna do it
	mesh->SetIndexBuffer ( idxBuffer );

	mesh->SetPrimType ( ZGL_PRIM_TRIANGLES );
	mesh->SetTotalElements ( totalElements );
	
	ZLBox bounds;
	format.ComputeBounds ( bounds, vtxBuffer->GetReadBuffer (), vtxBuffer->GetSize ());
	
	mesh->SetBounds ( bounds );
	
	return mesh;
}

//----------------------------------------------------------------//
u32 MOAIGeometryWriter::GetMesh ( const MOAIVertexFormat& format, ZLStream& vtxStream, ZLStream& idxStream, MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, u32 idxSizeInBytes ) {

	size_t vtxStreamBase = vtxStream.GetCursor ();
	size_t idxStreamBase = idxStream.GetCursor ();
	
	vtxStream.Seek ( 0, SEEK_SET );
	idxStream.Seek ( 0, SEEK_SET );
	
	if ( vtxBuffer.GetBufferCount () == 0 ) {
		vtxBuffer.ReserveVBOs ( 1 );
	}
	
	if ( idxBuffer.GetBufferCount () == 0 ) {
		idxBuffer.ReserveVBOs ( 1 );
	}
	
	vtxBuffer.CopyFromStream ( vtxStream );
	
	idxBuffer.SetIndexSize ( idxSizeInBytes );
	
	if ( idxStream.GetLength ()) {
		
		idxBuffer.CopyFromStream ( idxStream, 4 );
	}
	else {
	
		u32 totalVertices = vtxBuffer.GetSize () / format.GetVertexSize ();
		for ( u32 i = 0; i < totalVertices; ++i ) {
			idxBuffer.WriteIndex ( i );
		}
	}
	
	vtxStream.Seek ( vtxStreamBase, SEEK_SET );
	idxStream.Seek ( idxStreamBase, SEEK_SET );
	
	return idxBuffer.GetSize () / idxSizeInBytes;
}

//----------------------------------------------------------------//
MOAIGeometryWriter::MOAIGeometryWriter () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGeometryWriter::~MOAIGeometryWriter () {
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::PruneVertices ( const MOAIVertexFormat& format, MOAIStream& vtxStream, MOAIStream& idxStream ) {

	size_t vtxStreamBase = vtxStream.GetCursor ();
	size_t idxStreamBase = idxStream.GetCursor ();

	size_t vtxSize = format.GetVertexSize ();
	u32 inputVtxCount = ( u32 )(( vtxStream.GetLength () - vtxStreamBase ) / vtxSize );
	
	if ( !inputVtxCount ) return;
	
	ZLLeanArray < u32 > indexMap;
	indexMap.Init ( inputVtxCount );
	
	void* vtxBuffer0 = alloca ( vtxSize );
	void* vtxBuffer1 = alloca ( vtxSize );

	typedef STLMap < size_t, u32, VertexComparator > VertexMap;
	typedef VertexMap::iterator VertexMapIt;

	VertexMap vertexMap ( VertexComparator ( vtxStream, format, vtxBuffer0, vtxBuffer1 ));
	
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
	
	size_t totalIdx = idxStream.GetLength () >> 2; // index size is assumed to always be 4 bytes
	
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

	state.SetField ( -1, "COLOR_ADD",				( u32 )COLOR_ADD );
	state.SetField ( -1, "COLOR_MULTIPLY",			( u32 )COLOR_MULTIPLY );
	state.SetField ( -1, "COLOR_OVERWRITE",			( u32 )COLOR_OVERWRITE );
	state.SetField ( -1, "COLOR_SUBTRACT",			( u32 )COLOR_SUBTRACT );

	luaL_Reg regTable [] = {
		{ "applyColor",				_applyColor },
		{ "applyLightFromImage",	_applyLightFromImage },
		{ "applyLinearGradient",	_applyLinearGradient },
		{ "getMesh",				_getMesh },
		{ "pruneVertices",			_pruneVertices },
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
void MOAIGeometryWriter::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::SnapCoords ( const MOAIVertexFormat& format, ZLStream& stream, float xSnap, float ySnap, float zSnap ) {
	
	size_t base = stream.GetCursor ();
	u32 total = ( u32 )(( stream.GetLength () - base ) / format.GetVertexSize ());
	
	for ( u32 i = 0; i < total; ++i ) {
		
		format.SeekVertex ( stream, base, i );
		
		ZLVec4D coord = format.ReadCoord ( stream );
		
		coord.mX = xSnap == 0.0f ? coord.mX : floorf (( coord.mX / xSnap ) + 0.5f ) * xSnap;
		coord.mY = ySnap == 0.0f ? coord.mY : floorf (( coord.mY / ySnap ) + 0.5f ) * ySnap;
		coord.mZ = zSnap == 0.0f ? coord.mZ : floorf (( coord.mZ / zSnap ) + 0.5f ) * zSnap;
		
		format.WriteCoord ( stream, coord.mX, coord.mY, coord.mZ, coord.mW );
	}
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteBox ( const MOAIVertexFormat& format, ZLStream& stream, const ZLBox& box ) {

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

	MOAIGeometryWriter::WriteQuad ( format, stream, v0, v1, v2, v3, ZLVec3D ( 0.0f, 0.0f, -1.0f )); // back
	MOAIGeometryWriter::WriteQuad ( format, stream, v4, v5, v6, v7, ZLVec3D ( 0.0f, 0.0f, 1.0f )); // front
	
	MOAIGeometryWriter::WriteQuad ( format, stream, v1, v4, v7, v2, ZLVec3D ( -1.0f, 0.0f, 0.0f )); // left
	MOAIGeometryWriter::WriteQuad ( format, stream, v5, v0, v3, v6, ZLVec3D ( 1.0f, 0.0f, 0.0f )); // right

	MOAIGeometryWriter::WriteQuad ( format, stream, v5, v4, v1, v0, ZLVec3D ( 0.0f, -1.0f, 0.0f )); // bottom
	MOAIGeometryWriter::WriteQuad ( format, stream, v2, v7, v6, v3, ZLVec3D ( 0.0f, 1.0f, 0.0f )); // top
	
	stream.Seek ( base, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteColor ( const MOAIVertexFormat& format, ZLStream& stream, u32 mode, float r, float g, float b, float a ) {

	ZLColorVec dstColor ( r, g, b, a );
	
	if ( mode != COLOR_OVERWRITE ) {
	
		ZLColorVec srcColor = dstColor;
		ZLColorVec orgColor = format.ReadColor( stream );
	
		dstColor = orgColor;

		switch ( mode ) {
		
			case COLOR_ADD:
				srcColor.mA = 0.0f;
				dstColor.AddAndClamp ( srcColor );
				break;

			case COLOR_MULTIPLY:
				srcColor.mA = 1.0f;
				dstColor.Modulate ( srcColor );
				break;

			case COLOR_SUBTRACT:
				srcColor.mA = 0.0f;
				dstColor.SubAndClamp ( srcColor );
				break;
		}

		dstColor.Lerp ( ZLInterpolate::kLinear, orgColor, dstColor, a );
	}
	format.WriteColor ( stream, dstColor.mR, dstColor.mG, dstColor.mB, dstColor.mA );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteCylinder ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec2D& r0, const ZLVec2D& r1 ) {
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteIcoSphere ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r ) {
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteQuad ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0 ) {

	MOAIGeometryWriter::WriteQuad ( format, stream, v0, v1, v2, v3, n0, n0, n0, n0 );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteQuad ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2, const ZLVec3D& v3, const ZLVec3D& n0, const ZLVec3D& n1, const ZLVec3D& n2, const ZLVec3D& n3 ) {

	MOAIGeometryWriter::WriteVertex ( format, stream, v0, n0 );
	MOAIGeometryWriter::WriteVertex ( format, stream, v1, n1 );
	MOAIGeometryWriter::WriteVertex ( format, stream, v2, n2 );
	
	MOAIGeometryWriter::WriteVertex ( format, stream, v0, n0 );
	MOAIGeometryWriter::WriteVertex ( format, stream, v2, n2 );
	MOAIGeometryWriter::WriteVertex ( format, stream, v3, n3 );
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteUVSphere ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& v0, const ZLVec3D& r ) {
}

//----------------------------------------------------------------//
void MOAIGeometryWriter::WriteVertex ( const MOAIVertexFormat& format, ZLStream& stream, const ZLVec3D& coord, const ZLVec3D& normal ) {

	size_t base = stream.GetCursor ();
	format.WriteAhead ( stream );
	format.WriteCoord ( stream, coord.mX, coord.mY, coord.mZ, 1.0f );
	format.WriteNormal ( stream, normal.mX, normal.mY, normal.mZ );
	format.SeekVertex ( stream, base, 1 );
}
