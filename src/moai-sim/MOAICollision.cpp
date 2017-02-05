// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollision.h>
#include <moai-sim/MOAICollisionWorld.h>

// TODO: all this will eventualy be componetized and tidied up; just a scratch area for now.

//================================================================//
// MOAICollision
//================================================================//

//----------------------------------------------------------------//
void CalculateInterval ( const ZLVec2D* poly, size_t nPoly, const ZLVec2D& axis, float& min, float& max ) {
	
    min = max = axis.Dot ( poly [ 0 ]);
	
    for ( size_t i = 0; i < nPoly; ++i ) {

        float d = axis.Dot ( poly [ i ]);
        if ( d < min ) {
			min = d;
		}
        else if ( d > max ) {
			max = d;
		}
    } 
}

//----------------------------------------------------------------//
// return false if no overlap OR find the shortist displays poly has to move (along N) to no longer overlap poly1
bool AxisSeparatePolygon ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, const ZLVec2D& axis, float& t ) {

    float min0, max0;
    float min1, max1; 

    CalculateInterval ( poly0, nPoly0, axis, min0, max0 );
    CalculateInterval ( poly1, nPoly1, axis, min1, max1 );

	if (( max1 <= min0 ) || ( max0 <= min1 )) return false; // no overlap

    float d0 = max1 - min0; // if overlapped, d0 < 0
    float d1 = min1 - max0; // if overlapped, d1 > 0

	t = ABS ( d0 ) < ABS ( d1 ) ? d0 : d1;
	return true;
}

//----------------------------------------------------------------//
// return false if no overlap OR find the shortist displays poly has to move (along N) to no longer overlap poly1
bool AxisSeparatePolygons ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, ZLVec2D& outN, float& outT ) {

	bool first = true;
	float bestT = 0.0f;
	ZLVec2D bestN ( 0.0f, 0.0f );

	for ( size_t i = 0; i < nPoly0; i++ ) {
	
		// get the edge normal
		ZLVec2D e0 = poly0 [ i ];
        ZLVec2D e1 = poly0 [( i + 1 ) % nPoly0 ];
        ZLVec2D e = ZLVec2D::Sub ( e1, e0 );
        ZLVec2D n = ZLVec2D ( -e.mY, e.mX );
		n.Norm ();
		
		float t;
		
		if ( !AxisSeparatePolygon ( poly0, nPoly0, poly1, nPoly1, n, t )) return false;
		
		if (( first ) || ( ABS ( t ) < ABS ( bestT ))) {
			bestT = t;
			bestN = n;
			first = false;
		}
	}
	
	outN = bestN;
	outT = bestT;
	
	return true;
}

//----------------------------------------------------------------//
void MOAICollision::FindContactPoints ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, MOAIContactPointAccumulator2D& accumulator ) {

	// rename: contact points -> move constraints
	// this needs a lot of refinement.
	// see if we can generate fewer constraints.
	// better results with move hint passed in?
	// better naming options for the myriad variables below?
	
	// also: worth generating or storing edge normals for body?

	static const float PAD = 0.01f;
	static const float CORNER_PAD = 0.01f;

	for ( size_t i = 0; i < nPoly0; i++ ) {
	
		// calculate edge
		ZLVec2D v0 = poly0 [ i ];
        ZLVec2D v1 = poly0 [( i + 1 ) % nPoly0 ];
        ZLVec2D e0 = ZLVec2D::Sub ( v1, v0 );
		e0.Norm ();
        ZLVec2D n0 = e0;
		n0.Rotate90Clockwise ();
		
		ZLVec2D invE0 = e0;
		invE0.Invert ();
		
		ZLVec2D invN0 = n0;
		invN0.Invert ();
		
		float d0 = n0.Dot ( v0 );
		float ve0 = e0.Dot ( v0 );
		float ve1 = e0.Dot ( v1 );
	
		float innerD0 = d0 - PAD;
		float outerD0 = d0 + PAD;
	
		printf ( "EDGE N: (%g, %g) V: (%g, %g)->(%g, %g)\n", n0.mX, n0.mY, v0.mX, v0.mY, v1.mX, v1.mY );
	
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
			
			ZLVec2D e1 = ZLVec2D::Sub ( v3, v2 );
			
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
					
					//if ( n0.mY != -1.0f ) continue;
					
					// overlap
					
					if ( ve3 <= ( ve0 + CORNER_PAD )) {
					

						ZLVec2D wallNorm = ZLVec2D::Sub ( v1, poly0 [( i + 2 ) % nPoly0 ] );
						wallNorm.Norm ();
						wallNorm.Rotate90Clockwise ();
						wallNorm.Add ( n0 );
						wallNorm.Norm ();
						wallNorm.Rotate90Clockwise ();
						
						accumulator.PushCorner ( v3, invE0, invN0, v0, v1, wallNorm );
					}
					else if ( ve2 >= ( ve1 - CORNER_PAD )) {
					
						ZLVec2D wallNorm = ZLVec2D::Sub ( poly0 [( i + ( nPoly0 - 1 )) % nPoly0 ], v0 );
						wallNorm.Norm ();
						wallNorm.Rotate90Clockwise ();
						wallNorm.Add ( n0 );
						wallNorm.Norm ();
						wallNorm.Rotate90Anticlockwise ();
					
						accumulator.PushCorner ( v2, invE0, invN0, v0, v1, wallNorm );
					}
					else {
					
						if ( ve0 < ve2 ) {
							//accumulator.Push ( v2, invE0, invN0, v0, v1, MOAIContactPoint2D::PARALLEL );
						}
						else if ( ve0 <= ( ve2 + CORNER_PAD )) {
						
							ZLVec2D wallNorm = ZLVec2D::Sub ( v1, poly0 [( i + 2 ) % nPoly0 ] );
							wallNorm.Norm ();
							wallNorm.Rotate90Clockwise ();
							wallNorm.Add ( n0 );
							wallNorm.Norm ();
							wallNorm.Rotate90Clockwise ();
						
							
						
							//accumulator.PushCorner ( v0, invE0, invN0, v2, v3, invE0 );
							accumulator.PushCorner ( v0, invE0, invN0, v2, v3, wallNorm );
						}
						else if ( ve0 < ( ve3 - CORNER_PAD )) {
							//accumulator.Push ( v0, invE0, invN0, v3, v2, MOAIContactPoint2D::PARALLEL );
						}
					
						if ( ve1 > ve3 ) {
							//accumulator.Push ( v3, invE0, invN0, v0, v1, MOAIContactPoint2D::PARALLEL );
						}
						else if ( ve1 >= ( ve3 - CORNER_PAD )) {
						
							ZLVec2D wallNorm = ZLVec2D::Sub ( poly0 [( i + ( nPoly0 - 1 )) % nPoly0 ], v0 );
							wallNorm.Norm ();
							wallNorm.Rotate90Clockwise ();
							wallNorm.Add ( n0 );
							wallNorm.Norm ();
							wallNorm.Rotate90Anticlockwise ();
						
							//accumulator.PushCorner ( v1, invE0, invN0, v2, v3, e0 );
							accumulator.PushCorner ( v1, invE0, invN0, v2, v3, wallNorm );
						}
						else if ( ve1 > ( ve2 + CORNER_PAD )) {
							//accumulator.Push ( v1, invE0, invN0, v3, v2, MOAIContactPoint2D::PARALLEL );
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
								
								if ( ve2 <= ( ve0 + CORNER_PAD )) {
									// v0
									corner = v0;
									nextCorner = poly0 [( i + ( nPoly0 - 1 )) % nPoly0 ];
									wallTangent = ZLVec2D::Sub ( v0, nextCorner );
									wallTangent.Norm ();
									wallNorm = wallTangent;
									wallNorm.Rotate90Anticlockwise ();
								}
								else {
									// v1
									corner = v1;
									nextCorner = poly0 [( i + 2 ) % nPoly0 ];
									wallTangent = ZLVec2D::Sub ( v1, nextCorner );
									wallTangent.Norm ();
									wallNorm = wallTangent;
									wallNorm.Rotate90Clockwise ();
								}
								
								ZLVec2D cornerTangent = ZLVec2D::Add ( n0, wallNorm );
								cornerTangent.Norm ();
								
								// get the ray from the corner to the exterior point.
								ZLVec2D ray = ZLVec2D::Sub ( ext, s );
								
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
										
										//if ( n0.Dot ( n1 ) < EPSILON ) {
			
											// surface facing edge
											accumulator.PushCorner ( s, e1, n1, v2, v3, cornerTangent );
										}
									}
									else {
									
										// ray is on the wall exterior, in the 'shadow' of the wall.
										if ( n0.Dot ( n1 ) > -EPSILON ) {
										
											// surface facing *away* from edge
											accumulator.PushCorner ( s, wallTangent, wallNorm, s, nextCorner, cornerTangent );
										}
									}
								}
							}
						}
						else {
							// leaving
							accumulator.Push ( s, invE0, invN0, v0, v1, MOAIContactPoint2D::LEAVING );
						}
					}
					else {
						// crossing
						accumulator.Push ( s, e1, n1, v2, v3, MOAIContactPoint2D::CROSSING );
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------//
// return false if no overlap OR find the shortist displays poly has to move (along N) to no longer overlap poly1
bool MOAICollision::FindOverlapInterval ( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1, ZLVec2D& interval ) {

	float t0, t1;
	ZLVec2D n0, n1;

	if ( !AxisSeparatePolygons ( poly0, nPoly0, poly1, nPoly1, n0, t0 )) return false;
	if ( !AxisSeparatePolygons ( poly1, nPoly1, poly0, nPoly0, n1, t1 )) return false;
	
	if ( ABS ( t0 ) < ABS ( t1 )) {
		interval = n0;
		interval.Scale ( t0 );
	}
	else {
		interval = n1;
		interval.Scale ( -t1 );
	}	
	return true;
}
