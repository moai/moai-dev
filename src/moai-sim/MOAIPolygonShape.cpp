// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIPolygonShape.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <tesselator.h>

//http://www.songho.ca/opengl/gl_tessellation.html

//================================================================//
// MOAIPolygonShape
//================================================================//

//----------------------------------------------------------------//
void MOAIPolygonShape::AddContour ( USVec2D* verts, u32 total ) {

	if ( !this->mTess ) {
		this->mTess = tessNewTess( 0 );
		assert ( this->mTess );
	}
	
	tessAddContour ( this->mTess, 2, verts, sizeof ( USVec2D ), total );
}

//----------------------------------------------------------------//
void MOAIPolygonShape::Clear () {

	if ( this->mTess ) {
		tessDeleteTess (( TESStesselator* )this->mTess );
	}
}

//----------------------------------------------------------------//
MOAIPolygonShape::MOAIPolygonShape () :
	mTess ( 0 ) {
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
		this->RenderPrims ( ZGL_PRIM_TRIANGLE_FAN );
	}
	
	if ( this->mLineStyle == LINE_VECTOR ) {
		gfxDevice.SetPenColor ( this->mLineColor );
		this->RenderPrims ( ZGL_PRIM_LINE_LOOP );
	}
}

//----------------------------------------------------------------//
void MOAIPolygonShape::RenderPrims ( u32 primType ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	assert ( this->mTess );

	const float* verts = tessGetVertices ( this->mTess );
	const int* elems = tessGetElements ( this->mTess );
	const int nelems = tessGetElementCount ( this->mTess );
	
	for ( int i = 0; i < nelems; ++i ) {
	
		gfxDevice.BeginPrim ( primType );
		
		const int* p = &elems [ i * 3 ];
		for ( int j = 0; ( j < 3 ) && ( p [ j ] != TESS_UNDEF ); ++j ) {
		
			float x = verts[ p [ j ] * 2 ];
			float y = verts[( p [ j ] * 2 ) + 1 ];
			
			gfxDevice.WriteVtx ( x, y, 0.0f );
			gfxDevice.WriteFinalColor4b ();
		}
		
		gfxDevice.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIPolygonShape::Tesselate ( u32 windingRule ) {

	if ( !this->mTess ) return;

	const int NVP = 3;
	int result;
	
	//ZLLeanArray < USVec2D > contour1;
	//contour1.Init ( 4 );
	//contour1 [ 0 ].Init ( 50.0f, -50.0f );
	//contour1 [ 1 ].Init ( -50.0f, -50.0f );
	//contour1 [ 2 ].Init ( -50.0f, 50.0f );
	//contour1 [ 3 ].Init ( 50.0f, 50.0f );
	//
	//ZLLeanArray < USVec2D > contour2;
	//contour2.Init ( 3 );
	//contour2 [ 0 ].Init ( -50.0f, 150.0f );
	//contour2 [ 1 ].Init ( 50.0f, 150.0f );
	//contour2 [ 2 ].Init ( 0.0f, 0.0f );
	//
	//ZLLeanArray < USVec2D > contour3;
	//contour3.Init ( 4 );
	//contour3 [ 0 ].Init ( 25.0f, -25.0f );
	//contour3 [ 1 ].Init ( -25.0f, -25.0f );
	//contour3 [ 2 ].Init ( -25.0f, 25.0f );
	//contour3 [ 3 ].Init ( 25.0f, 25.0f );
	
	//tessAddContour ( tess, 2, this->mVertices.Data (), sizeof ( USVec2D ), this->mVertices.Size ());
	
	//tessAddContour ( tess, 2, contour2.Data (), sizeof ( USVec2D ), contour2.Size ());
	//tessAddContour ( tess, 2, contour3.Data (), sizeof ( USVec2D ), contour3.Size ());
	
	// First combine contours and then triangulate, this removes unnecessary inner vertices.
	//result = tessTesselate ( tess, TESS_WINDING_POSITIVE, TESS_BOUNDARY_CONTOURS, 0, 0, 0 );
	result = tessTesselate ( this->mTess, ( int )windingRule, TESS_POLYGONS, NVP, 2, 0 );
	
	//if ( result ) {
	//	const float* verts = tessGetVertices ( tess );
	//	const int* elems = tessGetElements ( tess );
	//	const int nelems = tessGetElementCount ( tess );

	//	for ( int i = 0; i < nelems; ++i ) {
	//		int b = elems [ i * 2 ];
	//		int n = elems [( i * 2 ) + 1 ];
	//		tessAddContour ( tess, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	//	}
	//	
	//	result = tessTesselate ( tess, TESS_WINDING_POSITIVE, TESS_POLYGONS, NVP, 2, 0 );
	//}
}
