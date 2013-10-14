// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorShape.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorShape
//================================================================//

const ZLVec3D MOAIVectorShape::sNormal = ZLVec3D ( 0.0f, 0.0f, 1.0f );

//----------------------------------------------------------------//
void MOAIVectorShape::AddFillContours ( TESStesselator* tess ) {
	UNUSED ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorShape::AddStrokeContours ( TESStesselator* tess ) {

	assert ( tess );

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );
	
	this->AddFillContours ( outline );
	tessTesselate ( outline, ( int )this->mStyle.mWindingRule, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	
	TESStesselator* exterior = tessNewTess ( 0 );
	TESStesselator* interior = tessNewTess ( 0 );
	
	assert ( exterior );
	assert ( interior );
	
	float width = this->mStyle.GetLineWidth ();
	
	float interiorWidth;
	float exteriorWidth;
	
	if ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) {
		interiorWidth = 0.0f;
		exteriorWidth = width;
	}
	else if ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR ) {
		interiorWidth = width;
		exteriorWidth = 0.0f;
	}
	else {
		interiorWidth = width * 0.5f;
		exteriorWidth = interiorWidth;
	}
	
	this->StrokeBoundaries ( exterior, outline, exteriorWidth, true, false );
	tessTesselate ( exterior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, exterior );
	
	this->StrokeBoundaries ( interior, outline, interiorWidth, true, true );
	tessTesselate ( interior, TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, interior );
	
	tessDeleteTess ( interior );
	tessDeleteTess ( exterior );
	tessDeleteTess ( outline );
}

//----------------------------------------------------------------//
void MOAIVectorShape::ComputeLineJoins ( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior ) {
	
	int top = nVerts - 1;
	float scale = interior ? -1.0f : 1.0f;
	
	if ( forward ) {
		for ( int i = 0; i < nVerts; ++i ) {
			joins [ i ].mVertex = verts [ i ];
		}
	}
	else {
		for ( int i = 0; i < nVerts; ++i ) {
			joins [ i ].mVertex = verts [ top - i ];
		}
	}
	
	for ( int i = 0; i < nVerts; ++i ) {
		
		ZLVec2D v0 = joins [ i ].mVertex;
		ZLVec2D v1 = joins [( i + 1 ) % nVerts ].mVertex;
		
		ZLVec2D n = v1;
		n.Sub ( v0 );
		
		joins [ i ].mEdgeVec = n;
		
		n.Rotate90Anticlockwise ();
		n.Norm ();
		n.Scale ( scale );
		
		joins [ i ].mEdgeNorm = n;
		joins [ i ].mIsCap = false;
	}
	
	int start = 0;
	int max = nVerts;
	
	if ( open ) {
		
		joins [ 0 ].mIsCap = true;
		joins [ 0 ].mJointNorm = joins [ 0 ].mEdgeNorm;
		
		joins [ top ].mIsCap = true;
		joins [ top ].mEdgeNorm = joins [ top - 1 ].mEdgeNorm;
		joins [ top ].mJointNorm = joins [ top ].mEdgeNorm;
		
		start = 1;
		max = top;
	}
	
	for ( int i = start; i < max; ++i ) {
		
		ZLVec2D n = joins [( i + top ) % nVerts ].mEdgeNorm;
		n.Add ( joins [ i ].mEdgeNorm );
		n.Norm ();
		
		joins [ i ].mJointNorm = n;
	}
}

//----------------------------------------------------------------//
void MOAIVectorShape::CopyBoundaries ( TESStesselator* dest, TESStesselator* src ) {

	const float* verts = tessGetVertices ( src );
	const int* elems = tessGetElements ( src );
	int nelems = tessGetElementCount ( src );

	for ( int i = 0; i < nelems; ++i ) {
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		tessAddContour ( dest, 2, &verts [ b * 2 ], sizeof ( float ) * 2, n );
	}
}

//----------------------------------------------------------------//
bool MOAIVectorShape::GroupShapes ( MOAIVectorShape** shapes, u32 total ) {
	UNUSED ( shapes );
	UNUSED ( total );
	return false;
}

//----------------------------------------------------------------//
bool MOAIVectorShape::GroupVertices ( MOAIVectorDrawing& drawing, u32 total ) {
	UNUSED ( drawing );
	UNUSED ( total );
	return false;
}

//----------------------------------------------------------------//
MOAIVectorShape::MOAIVectorShape () {
}

//----------------------------------------------------------------//
MOAIVectorShape::~MOAIVectorShape () {
}

//----------------------------------------------------------------//
void MOAIVectorShape::Stroke ( TESStesselator* tess, const ZLVec2D* verts, int nVerts, float width, bool forward, bool interior ) {

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
	this->ComputeLineJoins ( joins, verts, nVerts, false, forward, interior );
	
	bool exact = interior ? ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_EXTERIOR ) : ( this->mStyle.mStrokeStyle == MOAIVectorStyle::STROKE_INTERIOR );
	
	int contourVerts = this->Stroke ( 0, joins, nVerts, width, exact );
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
	
	this->Stroke ( contour, joins, nVerts, width, exact );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), contourVerts );
}

//----------------------------------------------------------------//
int MOAIVectorShape::Stroke ( ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact ) {

	if ( exact ) {
		if ( verts ) {
			for ( int i = 0; i < nJoins; ++i ) {
				*( verts++ ) = joins [ i ].mVertex;
			}
		}
		return nJoins;
	}

	u32 count = 0;
	for ( int i = 0; i < nJoins; ++i ) {
		
		int j0 = ( i + nJoins -1 ) % nJoins;
		int j1 = i;
		
		const MOAIVectorLineJoin& join = joins [ j1 ];
		
		float d = join.mEdgeNorm.Dot ( join.mJointNorm );
		float miter = width / d;
		
		u32 joinStyle = this->mStyle.mJoinStyle;
		
		if ( join.mIsCap ) {
			
			if ( verts ) {
				ZLVec2D v = join.mJointNorm;
				v.Scale ( width );
				v.Add ( join.mVertex );
				*( verts++ ) = v;
			}
			count = count + 1;
		}
		else {
			
			const MOAIVectorLineJoin& prev = joins [ j0 ];
			
			if ( join.mJointNorm.Dot ( prev.mEdgeVec ) <= 0.0f ) {
				joinStyle = MOAIVectorStyle::JOIN_MITER;
			}
			else if ( joinStyle == MOAIVectorStyle::JOIN_MITER ) {
				if (( miter / width ) > this->mStyle.mMiterLimit ) {
					joinStyle = MOAIVectorStyle::JOIN_BEVEL;
				}
			}
			
			switch ( joinStyle ) {
				
				case MOAIVectorStyle::JOIN_BEVEL: {
					
					if ( verts ) {
					
						ZLVec2D v0 = prev.mEdgeNorm;
						v0.Scale ( width );
						v0.Add ( join.mVertex );
						*( verts++ ) = v0;
						
						ZLVec2D v1 = join.mEdgeNorm;
						v1.Scale ( width );
						v1.Add ( join.mVertex );
						*( verts++ ) = v1;
					}
					count = count + 2;
					break;
				}
				
				case MOAIVectorStyle::JOIN_MITER: {
				
					if ( verts ) {
						ZLVec2D v = join.mJointNorm;
						v.Scale ( miter );
						v.Add ( join.mVertex );
						*( verts++ ) = v;
					}
					count = count + 1;
					break;
				}
				
				case MOAIVectorStyle::JOIN_ROUND: {
					
					u32 steps = 16;
					
					if ( verts ) {
					
						ZLVec2D n0 = prev.mEdgeNorm;
						ZLVec2D n1 = join.mEdgeNorm;
						
						float a0 = n0.Radians ();
						float a1 = n0.Radians ( n1 );
						
						float angle = a0;
						float angleStep = a1 / ( float )steps;
						
						for ( u32 i = 0; i <= steps; ++i, angle += angleStep ) {
							ZLVec2D v;
							v.mX = join.mVertex.mX + ( Cos ( angle ) * width );
							v.mY = join.mVertex.mY + ( Sin ( angle ) * width );
							*( verts++ ) = v;
						}
					}
					count = count + ( steps + 1 );
					break;
				}
			}
		}
	}
	
	return count;
}

//----------------------------------------------------------------//
void MOAIVectorShape::StrokeBoundaries ( TESStesselator* tess, TESStesselator* outline, float width, bool forward, bool interior ) {

	const float* verts = tessGetVertices ( outline );
	const int* elems = tessGetElements ( outline );
	int nelems = tessGetElementCount ( outline );

	for ( int i = 0; i < nelems; ++i ) {

		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];

		this->Stroke ( tess, ( const ZLVec2D* )&verts [ b * 2 ], n, width, forward, interior );
	}
}

//----------------------------------------------------------------//
void MOAIVectorShape::Tessalate ( MOAIVectorDrawing& drawing ) {
	
	if ( this->mStyle.GetFillStyle () == MOAIVectorStyle::FILL_SOLID ) {
	
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddFillContours ( triangles );
		tessTesselate ( triangles, ( int )this->mStyle.mWindingRule, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
			
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mFillColor.PackRGBA ());
		
		tessDeleteTess ( triangles );
	}
	
	if (( this->mStyle.GetLineStyle () == MOAIVectorStyle::LINE_STROKE ) && ( this->mStyle.GetLineWidth () > 0.0f )) {
		
		TESStesselator* triangles = tessNewTess ( 0 );
		
		this->AddStrokeContours ( triangles );
		tessTesselate ( triangles, TESS_WINDING_ODD, TESS_POLYGONS, NVP, 2, ( const TESSreal* )&sNormal );
		
		drawing.WriteTriangleIndices ( triangles, drawing.CountVertices ());
		drawing.WriteVertices ( triangles, this->mStyle.mLineColor.PackRGBA ());
		
		tessDeleteTess ( triangles );
	}
}
