// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPolygonShape.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <tesselator.h>

//http://www.songho.ca/opengl/gl_tessellation.html

//================================================================//
// MOAIPolygonShape
//================================================================//

//----------------------------------------------------------------//
void MOAIPolygonShape::AddContour ( USVec2D* verts, u32 total ) {

	if ( !this->mTess ) {
		this->mTess = tessNewTess ( 0 );
		assert ( this->mTess );
		
		this->mOutline = tessNewTess ( 0 );
		assert ( this->mOutline );
	}
	
	//tessAddContour ( this->mTess, 2, verts, sizeof ( USVec2D ), total );
	tessAddContour ( this->mOutline, 2, verts, sizeof ( USVec2D ), total );
}

//----------------------------------------------------------------//
void MOAIPolygonShape::Clear () {

	if ( this->mTess ) {
		tessDeleteTess (( TESStesselator* )this->mTess );
	}
	
	if ( this->mOutline ) {
		tessDeleteTess (( TESStesselator* )this->mOutline );
	}
}

//----------------------------------------------------------------//
void MOAIPolygonShape::DrawVertex ( u32 index, const float* verts ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	float x = verts [( index * 2 )];
	float y = verts [( index * 2 ) + 1 ];
	gfxDevice.WriteVtx ( x, y, 0.0f );
	gfxDevice.WriteFinalColor4b ();
}

//----------------------------------------------------------------//
void MOAIPolygonShape::DumpContourIndices ( TESStesselator* tess, MOAIIndexBuffer& idxBuffer ) {

	const int* elems = tessGetElements ( tess );
	const int nelems = tessGetElementCount ( tess );
	
	int totalLines = 0;
	for ( int i = 0; i < nelems; ++i ) {
		totalLines += elems [( i * 2 ) + 1 ];
	}
	
	idxBuffer.ReserveIndices ( totalLines * 2 );
	
	int idx = 0;
	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		
		for ( int j = 0; j < n; ++j ) {
			idxBuffer.SetIndex ( idx++, b + j );
			idxBuffer.SetIndex ( idx++, b + (( j + 1 ) % n ));
		}
	}
}

//----------------------------------------------------------------//
void MOAIPolygonShape::DumpTriangleIndices ( TESStesselator* tess, MOAIIndexBuffer& idxBuffer ) {

	const int* elems = tessGetElements ( tess );
	const int nelems = tessGetElementCount ( tess );
	
	idxBuffer.ReserveIndices ( nelems * 3 );
	idxBuffer.GetStream ().WriteBytes ( elems, nelems * sizeof ( u32 ) * 3 );
}

//----------------------------------------------------------------//
void MOAIPolygonShape::DumpVertices ( TESStesselator* tess, MOAIVertexBuffer& vtxBuffer ) {

	const float* verts = tessGetVertices ( tess );
	const int nverts = tessGetVertexCount ( tess );
	
	vtxBuffer.Reserve ( nverts * 16 );
	vtxBuffer.SetDefaultFormat ( MOAIVertexFormatMgr::XYZC );
	ZLStream& stream = vtxBuffer.GetStream ();
	
	for ( int i = 0; i < nverts; ++i ) {
	
		stream.WriteBytes ( &verts [ i * 2 ], 2 * sizeof ( float ));
		stream.Write < float >( 0.0f );
		stream.Write < u32 >( 0xffffffff );
	}
}

//----------------------------------------------------------------//
MOAIPolygonShape::MOAIPolygonShape () :
	mTess ( 0 ) {
	
	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
MOAIPolygonShape::~MOAIPolygonShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPolygonShape::Render () {

	if ( !this->mTess ) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	if ( this->mFillStyle == FILL_SOLID ) {
	
		gfxDevice.SetPenColor ( this->mFillColor );
		
		if ( this->mVtxBuffer.Bind ()) {
			if ( this->mIdxBuffer.LoadGfxState ()) {
				zglDrawElements ( ZGL_PRIM_TRIANGLES, this->mIdxBuffer.GetIndexCount (), ZGL_TYPE_UNSIGNED_INT, 0 );
			}
		}
	}

	if ( this->mLineStyle == LINE_VECTOR ) {
	
		gfxDevice.SetPenWidth ( 2.0f );
		gfxDevice.SetPenColor ( this->mLineColor );
		
		if ( this->mContourVtxBuffer.Bind ()) {
			if ( this->mContourIdxBuffer.LoadGfxState ()) {
				zglDrawElements ( ZGL_PRIM_LINES, this->mContourIdxBuffer.GetIndexCount (), ZGL_TYPE_UNSIGNED_INT, 0 );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPolygonShape::Tesselate ( u32 windingRule, bool preTessalate ) {

	if ( !this->mTess ) return;
	
	const int NVP = 3;
	ZLVec3D normal ( 0.0f, 0.0f, -1.0f );
	
	tessTesselate ( this->mOutline, ( int )windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal );
	
	if ( preTessalate ) {
	
		//if ( tessTesselate ( this->mTess, ( int )windingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&normal )) {
			const float* verts = tessGetVertices ( this->mOutline );
			const int* elems = tessGetElements ( this->mOutline );
			const int nelems = tessGetElementCount ( this->mOutline );

			for ( int i = 0; i < nelems; ++i ) {
				int b = elems [( i * 2 )];
				int n = elems [( i * 2 ) + 1 ];
				tessAddContour ( this->mTess, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
			}
		//}
	}
	tessTesselate ( this->mTess, ( int )windingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&normal );
	
	this->DumpTriangleIndices ( this->mTess, this->mIdxBuffer );
	this->DumpVertices ( this->mTess, this->mVtxBuffer );
	
	this->DumpContourIndices ( this->mOutline, this->mContourIdxBuffer );
	this->DumpVertices ( this->mOutline, this->mContourVtxBuffer );
}
