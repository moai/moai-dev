// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorUtil
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorUtil::ComputeLineJoins ( MOAIVectorLineJoin* joins, const ZLVec2D* verts, int nVerts, bool open, bool forward, bool interior ) {
	
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
		n.Norm ();
		
		joins [ i ].mEdgeVec = n;
		
		n.Rotate90Anticlockwise ();
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
		joins [ top ].mEdgeVec = joins [ top - 1 ].mEdgeVec;
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
int MOAIVectorUtil::StrokeLine ( const MOAIVectorStyle& style, ZLVec2D* verts, const MOAIVectorLineJoin* joins, int nJoins, float width, bool exact ) {

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
		
		u32 joinStyle = style.mJoinStyle;
		u32 capStyle = style.mCapStyle;
		
		if ( join.mIsCap ) {
			
			switch ( capStyle ) {
				
				case MOAIVectorStyle::CAP_BUTT: {
					
					if ( verts ) {
						ZLVec2D v = join.mVertex;
						v.Add ( join.mJointNorm, width );
						*( verts++ ) = v;
					}
					count = count + 1;
					break;
				}
				case MOAIVectorStyle::CAP_ROUND: {
					
					if ( i == 0 ) {
					
						ZLVec2D n0 = join.mEdgeVec;
						n0.Scale ( -1.0f );
					
						count = count + MOAIVectorUtil::StrokeWedge ( style, verts, join.mVertex, n0, join.mEdgeNorm, width );
					}
					else {
						count = count + MOAIVectorUtil::StrokeWedge ( style, verts, join.mVertex, join.mEdgeNorm, join.mEdgeVec, width );
					}
					break;
				}
				case MOAIVectorStyle::CAP_POINTY: {
				
					if ( verts ) {
					
						ZLVec2D v = join.mVertex;
					
						if ( i == 0 ) {
							
							v.Add ( join.mEdgeVec, -width );
							*( verts++ ) = v;
							
							v = join.mVertex;
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
						}
						else {
							
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
							
							v = join.mVertex;
							v.Add ( join.mEdgeVec, width );
							*( verts++ ) = v;
						}
					}
					count = count + 2;
					break;
				}
				case MOAIVectorStyle::CAP_SQUARE: {
				
					if ( verts ) {
					
						ZLVec2D v = join.mVertex;
					
						if ( i == 0 ) {
							
							v.Add ( join.mEdgeVec, -width );
							*( verts++ ) = v;
							
							v.Add ( join.mEdgeNorm, width );
							*( verts++ ) = v;
						}
						else {
							
							v.Add ( join.mEdgeNorm, width );
							v.Add ( join.mEdgeVec, width );
							*( verts++ ) = v;
							
							v.Add ( join.mEdgeNorm, -width );
							*( verts++ ) = v;
						}
					}
					count = count + 2;
					break;
				}
			}
		}
		else {
			
			const MOAIVectorLineJoin& prev = joins [ j0 ];
			
			if ( join.mJointNorm.Dot ( prev.mEdgeVec ) <= 0.0f ) {
				joinStyle = MOAIVectorStyle::JOIN_MITER;
			}
			else if ( joinStyle == MOAIVectorStyle::JOIN_MITER ) {
				if (( miter / width ) > style.mMiterLimit ) {
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
					
					count = count + MOAIVectorUtil::StrokeWedge ( style, verts, join.mVertex, prev.mEdgeNorm, join.mEdgeNorm, width );
					break;
				}
			}
		}
	}
	
	return count;
}

//----------------------------------------------------------------//
int MOAIVectorUtil::StrokeWedge ( const MOAIVectorStyle& style, ZLVec2D*& verts, const ZLVec2D& origin, const ZLVec2D& n0, const ZLVec2D& n1, float width ) {
	
	float wedge = n0.Radians ( n1 ); // angle between two normals
	
	u32 steps = style.GetResolutionForWedge ( wedge );
	
	if ( verts ) {
		
		float angle = n0.Radians (); // angle of first normal against x axis
		float angleStep = wedge / ( float )steps;
		
		for ( u32 i = 0; i <= steps; ++i, angle += angleStep ) {
			ZLVec2D v;
			v.mX = origin.mX + ( Cos ( angle ) * width );
			v.mY = origin.mY + ( Sin ( angle ) * width );
			*( verts++ ) = v;
		}
	}
	return ( int )( steps + 1 );
}
