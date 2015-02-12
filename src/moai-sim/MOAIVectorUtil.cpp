// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>
#include <signal.h>
#include <setjmp.h>
#include <zl-vfs/assert.h>

#define TESS_PRECISION 10000

//================================================================//
// SafeTesselator
//================================================================//

const ZLVec3D SafeTesselator::sNormal = ZLVec3D ( 0.0f, 0.0f, 1.0f );

//----------------------------------------------------------------//
void SafeTesselator::AddContour ( int size, const void* vertices, int stride, int numVertices ) {
	
	tessAddContour ( this->mTess, size, vertices, sizeof ( TESSreal ) * size, numVertices );
}

//----------------------------------------------------------------//
u32 SafeTesselator::GetTriangles ( MOAIGfxBuffer& vtxBuffer, MOAIGfxBuffer& idxBuffer, u32 idxSizeInBytes ) {

	ZLMemStream idxStream;
	ZLMemStream vtxStream;

	const int* elems = tessGetElements ( this->mTess );
	const int nelems = tessGetElementCount ( this->mTess );
	
	for ( int i = 0; i < nelems; ++i ) {
		const int* tri = &elems [ i * 3 ];
		
		idxStream.Write < u32 >( tri [ 0 ]);
		idxStream.Write < u32 >( tri [ 1 ]);
		idxStream.Write < u32 >( tri [ 2 ]);
	}

	const float* verts = tessGetVertices ( this->mTess );
	const int nverts = tessGetVertexCount ( this->mTess );
	
	for ( int i = 0; i < nverts; ++i ) {
		const ZLVec2D& vert = (( const ZLVec2D* )verts )[ i ];
		
		vtxStream.Write < float >( vert.mX );
		vtxStream.Write < float >( vert.mY );
		vtxStream.Write < float >( 0.0f );
		vtxStream.Write < u32 >( 0xffffffff );
	}
	
	return MOAIVectorUtil::GetTriangles ( vtxStream, vtxBuffer, idxStream, idxBuffer, idxSizeInBytes );
}

//------------------------------------------------------------------//
void SafeTesselator::Reset () {
	if ( this->mTess ) {
		tessDeleteTess ( this->mTess );
	}
	this->mTess = tessNewTess ( 0 );
	tessSetPrecision ( this->mTess, TESS_PRECISION );
}

//------------------------------------------------------------------//
SafeTesselator::SafeTesselator () :
	mTess ( 0 ) {
	this->Reset ();
}

//------------------------------------------------------------------//
SafeTesselator::~SafeTesselator () {
	tessDeleteTess ( this->mTess );
}

//------------------------------------------------------------------//
int SafeTesselator::Tesselate ( int windingRule, int elementType, int polySize, int vertexSize, const TESSreal* normal ) {

	int err = zl_begin_assert_env ();
	if ( err == 0 ) {
		tessTesselate ( this->mTess, windingRule, elementType, polySize, vertexSize, normal ? normal : ( const TESSreal* )&sNormal );
	}
	zl_end_assert_env ();
	return err;
}

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
	
//	for ( int i = 0; i < nVerts; ++i ) {
//		
//		ZLVec2D& vertex = joins [ i ].mVertex;
//		vertex.mX = floorf ( vertex.mX * 1000 ) / 1000;
//		vertex.mY = floorf ( vertex.mY * 1000 ) / 1000;
//	}
	
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
u32 MOAIVectorUtil::GetTriangles ( ZLStream& vtxStream, MOAIGfxBuffer& vtxBuffer, ZLStream& idxStream, MOAIGfxBuffer& idxBuffer, u32 idxSizeInBytes ) {

	idxStream.Seek ( 0, SEEK_SET );
	vtxStream.Seek ( 0, SEEK_SET );

	vtxBuffer.Clear ();
	vtxBuffer.SetTarget ( ZGL_BUFFER_TARGET_ARRAY );
	vtxBuffer.Reserve ( vtxStream.GetLength ());
	vtxBuffer.WriteStream ( vtxStream );
	
	u32 totalIndices = ( u32 )( idxStream.GetLength () >> 2 );
	
	idxBuffer.Clear ();
	idxBuffer.SetTarget ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	idxBuffer.Reserve ( totalIndices * idxSizeInBytes );
	
	if ( idxSizeInBytes == 4 ) {
		idxBuffer.WriteStream ( idxStream );
	}
	else {
		for ( u32 i = 0; i < totalIndices; ++i ) {
			idxBuffer.Write < u16 >(( u16 )idxStream.Read < u32 >( 0 ));
		}
	}
	return totalIndices;
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

	if ( verts ) {
		printf ( "verts:\n" );
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
						
						printf ( "    x:%f, y:%f\n", v.mX, v.mY );
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
	
	if ( verts ) {
		printf ( "\n" );
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


