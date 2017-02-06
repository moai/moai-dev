// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMoveConstraint2D.h>

// uncomment me to debug log
//#define MOAIMOVECONSTRAINT2D_DEBUG

#ifdef MOAIMOVECONSTRAINT2D_DEBUG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIMoveConstraintAccumulator2D
//================================================================//

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::FindConstraints ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1 ) {

	if (( nPoly0 < 3 ) || ( nPoly1 < 2 )) return;

	// rename: contact points -> move constraints
	// this needs a lot of refinement.
	// see if we can generate fewer constraints.
	// better results with move hint passed in?
	// better naming options for the myriad variables below?
	
	// also: worth generating or storing edge normals for body?

	static const float PAD = 0.001f;
	static const float CORNER_PAD = 0.01f;

	ZLVec2D* bodyEdges			= ( ZLVec2D* )alloca ( nPoly0 * sizeof ( ZLVec2D ));
	ZLVec2D* bodyEdgeNormals	= ( ZLVec2D* )alloca ( nPoly0 * sizeof ( ZLVec2D ));
	ZLVec2D* bodyVertexNormals	= ( ZLVec2D* )alloca ( nPoly0 * sizeof ( ZLVec2D ));

	// calculate all the body's edges and normals up front
	for ( size_t i = 0; i < nPoly0; i++ ) {
	
		ZLVec2D& edge = bodyEdges [ i ];
		ZLVec2D& edgeNormal = bodyEdgeNormals [ i ];
		
		edge = poly0 [( i + 1 ) % nPoly0 ] - poly0 [ i ];
		edge.Norm ();
		edgeNormal = edge;
		edgeNormal.Rotate90Clockwise ();
	}

	// calculate the body's vertex normals
	for ( size_t i = 0; i < nPoly0; i++ ) {
		ZLVec2D& vertexNormal = bodyVertexNormals [ i ];
		vertexNormal = bodyEdgeNormals [( i + ( nPoly0 - 1 )) % nPoly0 ] + bodyEdgeNormals [ i ];
		vertexNormal.Norm ();
	}

	for ( size_t i = 0; i < nPoly0; i++ ) {
		
		ZLVec2D v0 = poly0 [ i ];
        ZLVec2D v1 = poly0 [( i + 1 ) % nPoly0 ];
		ZLVec2D e0 = bodyEdges [ i ];
		ZLVec2D n0 = bodyEdgeNormals [ i ];
		
		//if ( n0.mY > ( EPSILON - 1.0f )) continue;
		
		ZLVec2D invE0 = e0;
		invE0.Invert ();
		
		ZLVec2D invN0 = n0;
		invN0.Invert ();
		
		float d0 = n0.Dot ( v0 );
		float ve0 = e0.Dot ( v0 );
		float ve1 = e0.Dot ( v1 );
	
		float innerD0 = d0 - PAD;
		float outerD0 = d0 + PAD;
	
		DEBUG_LOG ( "EDGE N: (%g, %g) V: (%g, %g)->(%g, %g)\n", n0.mX, n0.mY, v0.mX, v0.mY, v1.mX, v1.mY );
	
		for ( size_t j = 0; j < nPoly1; j++ ) {
		
			ZLVec2D v2 = poly1 [ j ];
			ZLVec2D v3 = poly1 [( j + 1 ) % nPoly1 ];
			ZLVec2D ext = v3;
			
			float p0 = n0.Dot ( v2 );
			float p1 = n0.Dot ( v3 );
			
			if ( p1 < p0 ) {
				std::swap ( p0, p1 );
				ext = v2;
			}
			
			ZLVec2D e1 = v3 - v2;
			
			if (( p1 < innerD0 ) || ( outerD0 < p0 )) continue; // edge is fully inside or outside; ignore it
			
			if (( innerD0 <= p0 ) && ( p1 <= outerD0 )) {
			
				// parallel
				
				ZLVec2D n1 = e1;
				n1.Rotate90Clockwise ();
				if ( n0.Dot ( n1 ) > -EPSILON ) continue;
				
				// project surface points along poly edge
				float ve2 = e0.Dot ( v2 );
				float ve3 = e0.Dot ( v3 );
				
				// make sure they are in order (will make next steps easier)
				if ( ve3 < ve2 ) {
					std::swap ( ve2, ve3 );
					std::swap ( v2, v3 );
				}
				
				if (( ve2 <= ( ve1 + PAD )) && ( ve3 >= ( ve0 - PAD ))) {
					
					// overlap
					
					if ( ve3 <= ( ve0 + CORNER_PAD )) {

						ZLVec2D slideNormal = n0;
						slideNormal.Rotate90Clockwise ();

						ZLVec2D cornerTangent = bodyVertexNormals [ i ];
						cornerTangent.Rotate90Clockwise ();
						
						this->PushCorner ( v3, invE0, invN0, v0, v1, slideNormal, cornerTangent );
					}
					else if ( ve2 >= ( ve1 - CORNER_PAD )) {
						
						ZLVec2D slideNormal = n0;
						slideNormal.Rotate90Anticlockwise ();
						
						ZLVec2D cornerTangent = bodyVertexNormals [( i + 1 ) % nPoly0 ];
						cornerTangent.Rotate90Anticlockwise ();
						
						this->PushCorner ( v2, invE0, invN0, v0, v1, slideNormal, cornerTangent );
					}
					else {
					
						if (( ve0 >= ve2 ) && ( ve0 <= ( ve2 + CORNER_PAD ))) {
							
							ZLVec2D slideNormal = n0;
							slideNormal.Rotate90Clockwise ();
							
							ZLVec2D cornerTangent = bodyVertexNormals [ i ];
							cornerTangent.Rotate90Clockwise ();

							this->PushCorner ( v0, invE0, invN0, v2, v3, slideNormal, cornerTangent );
						}
					
						if (( ve1 <= ve3 ) && ( ve1 >= ( ve3 - CORNER_PAD ))) {
							
							ZLVec2D slideNormal = n0;
							slideNormal.Rotate90Anticlockwise ();
							
							ZLVec2D cornerTangent = bodyVertexNormals [( i + 1 ) % nPoly0 ];
							cornerTangent.Rotate90Anticlockwise ();
							
							this->PushCorner ( v1, invE0, invN0, v2, v3, slideNormal, cornerTangent );
						}
					}
				}
			}
			else {
	
				float d = n0.Dot ( e1 );
				assert ( d != 0.0f ); // should never happen
				float t = ( n0.Dot ( v2 ) - d0 ) / -d;
				
				// get the intersection point
				ZLVec2D s = e1;
				s.Scale ( t );
				s.Add ( v2 );
				
				// get its projection on the edge
				float ve2 = e0.Dot ( s );
			
				if (( ve2 >= ( ve0 - PAD )) && ( ve2 <= ( ve1 + PAD ))) {
					
					// ok, it's within the bounds
					
					// get the normal for the surface
					e1.Norm ();
					ZLVec2D n1 = e1;
					n1.Rotate90Clockwise ();

					if (( innerD0 <= p0 ) || ( outerD0 >= p1 )) {
						
						// leaving or corner
						
						if (( ve2 <= ( ve0 + CORNER_PAD )) || ( ve2 >= ( ve1 - CORNER_PAD ))) {
						
							// here's that "corner case"

							// surface must be going *away* from edge; otherwise don't care
							if ( p0 >= innerD0 ) {
								
								ZLVec2D corner;
								ZLVec2D nextCorner;
								ZLVec2D wallTangent;
								ZLVec2D wallNorm;
								
								ZLVec2D cornerTangent;
								
								if ( ve2 <= ( ve0 + CORNER_PAD )) {
									// v0
									corner = v0;
									nextCorner = poly0 [( i + ( nPoly0 - 1 )) % nPoly0 ];
									wallTangent = bodyEdges [( i + ( nPoly0 - 1 )) % nPoly0 ];
									wallNorm = wallTangent;
									wallNorm.Rotate90Anticlockwise ();
									
									cornerTangent = bodyVertexNormals [ i ];
									cornerTangent.Rotate90Anticlockwise ();
								}
								else {
									// v1
									corner = v1;
									nextCorner = poly0 [( i + 2 ) % nPoly0 ];
									wallTangent = bodyEdges [( i + 1 ) % nPoly0 ];
									wallTangent.Invert ();
									wallNorm = wallTangent;
									wallNorm.Rotate90Clockwise ();
									
									cornerTangent = bodyVertexNormals [( i + 1 ) % nPoly0 ];
									cornerTangent.Rotate90Clockwise ();
								}
								
								// get the ray from the corner to the exterior point.
								ZLVec2D ray = ext - s;
								
								// make sure we're in front of the body edge
								if ( n0.Dot ( ray ) > -EPSILON ) {
								
									// we have to figure out which side of the 'wall' we're on.
									if ( wallNorm.Dot ( ray ) > -EPSILON ) {
									
										// ray is on the wall interior, 'visible' to the edge.
										// now some trickery: we to properly reject surface edges
										// facing the wrong way, we need to check my quadrants using
										// both the wall tangent and the wall norm.
										
										bool front = ( wallTangent.Dot ( ray ) >= 0.0f );
										
										if (
											( front && ( wallNorm.Dot ( n1 ) >= 0.0f )) ||
											( !front && ( n0.Dot ( n1 ) < EPSILON ))
										) {
											// surface facing edge
											this->PushCorner ( s, e1, n1, v2, v3, n0, cornerTangent );
										}
									}
									else {
									
										// ray is on the wall exterior, in the 'shadow' of the wall.
										if ( n0.Dot ( n1 ) > -EPSILON ) {
										
											// surface facing *away* from edge
											this->PushCorner ( s, wallTangent, wallNorm, s, nextCorner, wallTangent, cornerTangent );
										}
									}
								}
							}
						}
						else {
							// leaving
							this->Push ( s, invE0, invN0, v0, v1, MOAIMoveConstraint2D::LEAVING );
						}
					}
					else {
						// crossing
						this->Push ( s, e1, n1, v2, v3, MOAIMoveConstraint2D::CROSSING );
					}
				}
			}
		}
	}
}
