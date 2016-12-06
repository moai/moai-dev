// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLByteStream.h>
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLFloat.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLMemStream.h>
#include <zl-util/ZLPolygon2D.h>
#include <zl-util/ZLRadixSort32.h>

// based on:
// https://github.com/w8r/GreinerHormann
// https://github.com/tcql/greiner-hormann

// http://www.inf.usi.ch/hormann/papers/Greiner.1998.ECO.pdf
// https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm
// http://getwx.com/Poly/Kim-Kim-Extended-Clipping.pdf - antecedent to Foster, Overfelt
// https://arxiv.org/pdf/1211.3376v1.pdf - heard this was withdrawn (doesn't handle all cases)
// http://stackoverflow.com/questions/23412973/greiner-hormann-clipping-with-degeneracies

// uncomment me to debug log
//#define ZLPOLYGON2D_DEBUG_CLIP

#ifdef ZLPOLYGON2D_DEBUG_CLIP
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

// PCM: my approach to classifying intersections is based on checking the actual geometry at each intersection.
// this means we don't rely on flags or state toggles alone. because we make no special case for degeneracies, an
// intersection may be produced with an alpha of 0, which means it overlaps the proceeding vertex on the source
// polygon. in this case, we simply allow the degenerate intersection to exist and filter it out when writing
// the result polygon be checking that the vertex being written is always different from the vertex just written.

// input polygons are always cleaned as they are prepared for clipping. this means removing non-corners and
// repeated vertices.

// it should also be noted that as intersections are detected and sorted into the polygon edges, intersections
// with matching alphas will be inserted in the order they were encountered.

//================================================================//
// GHVertex
//================================================================//
class GHVertex :
	public ZLVec2D {
public:

	enum {
		INTERSECTION_UNCLASSIFIED,
		INTERSECTION_NONE,
		INTERSECTION_IS_ENTRY,
		INTERSECTION_IS_EXIT,
		
	};

	GHVertex*	mNext;
	GHVertex*	mPrev;
	GHVertex*	mNextEdge;
	u32			mIntersectionType;
	GHVertex*	mNeighbor;
	ZLVec2D		mTangent;
	float		mAlpha;
	bool		mVisited;
	
	//----------------------------------------------------------------//
	GHVertex () :
		mNext ( 0 ),
		mPrev ( 0 ),
		mNextEdge ( 0 ),
		mIntersectionType ( INTERSECTION_UNCLASSIFIED ),
		mNeighbor ( 0 ),
		mTangent ( 0.0f, 0.0f ),
		mAlpha ( 0.0f ),
		mVisited ( false ) {
	}
	
	//----------------------------------------------------------------//
	static cc8* StringForIntersectionType ( u32 type ) {
	
		switch ( type ) {
		
			case INTERSECTION_NONE:				return "INTERSECTION_NONE";
			case INTERSECTION_IS_ENTRY:			return "IS_ENTRY";
			case INTERSECTION_IS_EXIT:			return "IS_EXIT";
			
			default:
			case INTERSECTION_UNCLASSIFIED:		return "UNCLASSIFIED";
		}
	}
};

//================================================================//
// GHVertexFactory
//================================================================//
class GHVertexFactory :
	public ZLByteStream {
public:

	//----------------------------------------------------------------//
	GHVertex* AllocVertex () {
		
		if (( this->GetCursor () - this->GetLength ()) >= sizeof ( GHVertex )) {
		
			GHVertex* vtx = ( GHVertex* )(( size_t )this->GetWriteBuffer () + this->GetCursor ());
			if ( vtx ) {
			
				this->Seek ( sizeof ( GHVertex ), SEEK_CUR );
				
				new ( vtx ) GHVertex;
				return vtx;
			}
		}
		return new GHVertex ();
	}
	
	//----------------------------------------------------------------//
	void FreeVertex ( GHVertex* vtx ) {
		
		size_t bufferAddr	= ( size_t )this->GetWriteBuffer ();
		size_t bufferSize	= this->GetLength ();
		size_t vtxAddr		= ( size_t )vtx;
		
		if (( vtxAddr >= bufferAddr ) && ( vtxAddr < ( bufferAddr + bufferSize ))) {
			vtx->~GHVertex ();
		}
		else {
			delete vtx;
		}
	}
};

//================================================================//
// GHPolygon
//================================================================//
class GHPolygon {
public:

	GHVertexFactory&	mVtxFactory;
	GHVertex*			mHead;
	size_t				mSize;
	
	//----------------------------------------------------------------//
	void ClassifyIntersections () {
	
		if ( !this->mHead ) return;
		
		if ( !this->mHead->mPrev ) {
		
			// this is an axis, not a polygon
		
			GHVertex* cursor = this->mHead;
			bool isEntry = true;
			
			do {
				if ( cursor->mIntersectionType == GHVertex::INTERSECTION_UNCLASSIFIED ) {
					cursor->mIntersectionType = isEntry ? GHVertex::INTERSECTION_IS_ENTRY : GHVertex::INTERSECTION_IS_EXIT;
					isEntry = !isEntry;
				}
				cursor = cursor->mNext;
			}
			while ( cursor && ( cursor != this->mHead ));
		}
		else {
	
			GHVertex* cursor = this->mHead;
			
			u32 prevIntersectionType = GHVertex::INTERSECTION_NONE;
			
			do {

				u32 intersectionType = GHVertex::INTERSECTION_NONE;
				
				// only care about intersections that are also corners
				if ( cursor->mIntersectionType == GHVertex::INTERSECTION_UNCLASSIFIED ) {
					
					ZLVec2D edgeTangent = cursor->mNeighbor->mTangent;
					
					ZLVec2D edgeNormal = edgeTangent;
					edgeNormal.Rotate90Clockwise ();
					
					DEBUG_LOG ( "edge: (%g, %g)\n", edgeTangent.mX, edgeTangent.mY );
					
					DEBUG_LOG ( "intersection: (%g, %g) (%g, %g) %s\n",
						cursor->mX,
						cursor->mY,
						cursor->mTangent.mX,
						cursor->mTangent.mY,
						GHVertex::StringForIntersectionType ( cursor->mIntersectionType )
					);

					// only consider the first intersection
					if ( cursor->mPrev->mNeighbor != cursor->mNeighbor ) {
					
						DEBUG_LOG ( " -- first intersection\n" );
					
						GHVertex* prev = cursor->mPrev;
						while (( edgeNormal.Dot ( prev->mTangent ) == 0.0f ) || ( prev->Equals ( *cursor ))) {
							prev = prev->mPrev;
						}
						
						DEBUG_LOG ( "prev: (%g, %g) (%g, %g)\n", prev->mX, prev->mY, prev->mTangent.mX, prev->mTangent.mY );
					
						// skip intersections we don't care about
					
						GHVertex* next = cursor;
						while ( edgeNormal.Dot ( next->mTangent ) == 0.0f ) {
							next = next->mNext;
							next->mIntersectionType = GHVertex::INTERSECTION_NONE;
						}
						
						DEBUG_LOG ( "next: (%g, %g) (%g, %g)\n", cursor->mX, cursor->mY, cursor->mTangent.mX, cursor->mTangent.mY );
					
						float d0 = edgeNormal.Dot ( prev->mTangent );
						float d1 = edgeNormal.Dot ( next->mTangent );
						
						if ((( d0 < 0.0f ) && ( d1 < 0.0f )) || (( 0.0f < d0 ) && ( 0.0f < d1 ))) {
							
							intersectionType = d1 > 0.0f ? GHVertex::INTERSECTION_IS_ENTRY : GHVertex::INTERSECTION_IS_EXIT;
							
							if ( intersectionType == prevIntersectionType ) {
								intersectionType = GHVertex::INTERSECTION_NONE;
							}
							else {
								prevIntersectionType = intersectionType;
							}
							DEBUG_LOG ( "crossed: %s\n", GHVertex::StringForIntersectionType ( intersectionType ));
						}
					}
				}
				
				cursor->mIntersectionType = intersectionType;
				
				if ( cursor->mNeighbor && ( intersectionType == GHVertex::INTERSECTION_NONE )) {
					cursor->mNeighbor->mIntersectionType = GHVertex::INTERSECTION_NONE;
				}

				cursor = cursor->mNext;
			}
			while ( cursor != this->mHead );
			
			DEBUG_LOG ( "\n" );
		}
	}
	
	//----------------------------------------------------------------//
	static size_t ClassifyAndClip ( GHPolygon& ghSourcePolygon, GHPolygon& ghClipPolygon, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes ) {
	
		#ifdef ZLPOLYGON2D_DEBUG_CLIP
			DEBUG_LOG ( "SOURCE POLYGON (unclassified)\n" );
			ghSourcePolygon.Print ();
			DEBUG_LOG ( "\n" );
		#endif
		
		ghSourcePolygon.ClassifyIntersections ();
		ghClipPolygon.ClassifyIntersections ();

		#ifdef ZLPOLYGON2D_DEBUG_CLIP
			DEBUG_LOG ( "SOURCE POLYGON (classified)\n" );
			ghSourcePolygon.Print ();
			DEBUG_LOG ( "\n" );
			
			DEBUG_LOG ( "CLIP AXIS (classified)\n" );
			ghClipPolygon.Print ();
			DEBUG_LOG ( "\n" );
		#endif

		// construct the polygons

		GHVertex* polyCursor = ghSourcePolygon.mHead;
		size_t polyCount = 0;

		do {
		
			if (( !polyCursor->mVisited ) && ( polyCursor->mIntersectionType == GHVertex::INTERSECTION_IS_ENTRY )) {
		
				GHVertex* current = polyCursor;
				
				size_t clippedPolyVertsStart = clippedPolyVerts.GetCursor ();
				
				ZLVec2D first = *current;
				ZLVec2D vtx = *current;
				clippedPolyVerts.Write ( vtx );
				
				do {
					current->mVisited = true;
					current->mNeighbor->mVisited = true;
					
					if ( current->mIntersectionType == GHVertex::INTERSECTION_IS_ENTRY ) {
						do {
						
							current = current->mNext;
							
							if (( current->mX != vtx.mX ) || ( current->mY != vtx.mY )) {
								vtx = *current;
								clippedPolyVerts.Write ( vtx );
							}
						}
						while ( current->mIntersectionType == GHVertex::INTERSECTION_NONE);

					}
					else {
						do {
						
							current = current->mPrev;
							
							if (( current->mX != vtx.mX ) || ( current->mY != vtx.mY )) {
								vtx = *current;
								clippedPolyVerts.Write ( vtx );
							}
						}
						while ( current->mIntersectionType == GHVertex::INTERSECTION_NONE );
					}
					
					current = current->mNeighbor;
					
				}
				while ( !current->mVisited );

				size_t polySize = ( size_t )(( clippedPolyVerts.GetCursor () - clippedPolyVertsStart ) / sizeof ( ZLVec2D ));

				if ( vtx.Equals ( first )) {
					polySize = polySize - 1;
				}

				clippedPolySizes.Write ( polySize );
				polyCount++;
			}
			
			polyCursor = polyCursor->mNext;
		}
		while ( polyCursor != ghSourcePolygon.mHead );

		return polyCount;
	}
	
	//----------------------------------------------------------------//
	void Clear () {
	
		GHVertex* cursor = this->mHead;
		if ( cursor ) {
			do {
				GHVertex* kill = cursor;
				cursor = cursor->mNext;
				this->mVtxFactory.FreeVertex ( kill );
			}
			while ( cursor && ( cursor != this->mHead ));
		}
		
		this->mHead = 0;
		this->mSize = 0;
	}
	
	//----------------------------------------------------------------//
	GHPolygon ( GHVertexFactory& vtxFactory ) :
		mVtxFactory ( vtxFactory ),
		mHead ( 0 ),
		mSize ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	GHPolygon ( GHVertexFactory& vtxFactory, const ZLPolygon2D& poly ) :
		mVtxFactory ( vtxFactory ),
		mHead ( 0 ),
		mSize ( 0 ) {
		
		size_t polySize = poly.GetSize ();
		size_t newPolySize = 0;
		
		GHVertex* head = 0;
		GHVertex* tail = 0;
		
		for ( size_t i = 0; i < polySize; ++i ) {
		
			ZLVec2D v0 = poly.GetVertex (( i == 0 ? polySize : i ) - 1 );
			ZLVec2D v1 = poly.GetVertex ( i );
			ZLVec2D v2 = poly.GetVertex (( i + 1 ) % polySize );
			
			DEBUG_LOG ( "%d: (%g, %g) (%g, %g) (%g, %g)\n", i, v0.mX, v0.mY, v1.mX, v1.mY, v2.mX, v2.mY );
			
			if (( v0.mX == v1.mX ) && ( v0.mY == v1.mY )) {
				DEBUG_LOG ( " -- skipping repeated vertex\n" );
				continue; // ignore repeats
			}
			
			// skip ahead to find the next unique vertex
			size_t j = 1;
			for ( ; (( v1.mX == v2.mX ) && ( v1.mY == v2.mY )) && ( j < polySize ); ++j ) {
				v2 = poly.GetVertex (( i + j + 1 ) % polySize );
			}
			
			if ( j == polySize ) {
				DEBUG_LOG ( " -- problem polygon encountered\n" );
				return;
			}
			
			if ( j > 1 ) {
				DEBUG_LOG ( " -- fast forward to new final vert\n" );
				DEBUG_LOG ( "%d: (%g, %g) (%g, %g) (%g, %g)\n", i, v0.mX, v0.mY, v1.mX, v1.mY, v2.mX, v2.mY );
			}
			
			ZLVec2D e0 = v1;
			e0.Sub ( v0 );
			e0.Norm ();
			
			ZLVec2D e1 = v2;
			e1.Sub ( v1 );
			e1.Norm ();
			
			float d = e0.Dot ( e1 );
			if (( d == 1.0f ) || ( d == -1.0f )) {
				DEBUG_LOG ( " -- skipping non-corner\n" );
				continue; // ignore non-corners
			}
			
			GHVertex* ghVert			= this->mVtxFactory.AllocVertex ();
			ghVert->mX					= v1.mX;
			ghVert->mY					= v1.mY;
			ghVert->mTangent			= e1;
			ghVert->mIntersectionType	= GHVertex::INTERSECTION_NONE;
			
			if ( tail ) {
				tail->mNext = ghVert;
				tail->mNextEdge = ghVert;
				ghVert->mPrev = tail;
				tail = ghVert;
			}
			else {
				head = ghVert;
				tail = ghVert;
			}
			
			newPolySize++;
		}
		
		tail->mNext = head;
		tail->mNextEdge = head;
		head->mPrev = tail;
		
		this->mHead = head;
		this->mSize = newPolySize;
		
		DEBUG_LOG ( "finished polygon with %d vertices\n\n", newPolySize );
	}
	
	//----------------------------------------------------------------//
	~GHPolygon () {
	
		this->Clear ();
	}
	
	//----------------------------------------------------------------//
	GHVertex* InsertIntersection ( const ZLVec2D& vert, const ZLVec2D& tangent ) {
	
		GHVertex* ghVert		= this->mVtxFactory.AllocVertex ();
		ghVert->mX				= vert.mX;
		ghVert->mY				= vert.mY;
		ghVert->mTangent		= tangent;
		ghVert->mAlpha			= tangent.Dot ( vert );
	
		if ( !this->mHead ) {
			this->mHead = ghVert;
		}
		else {
		
			GHVertex* cursor = this->mHead;
			while ( cursor && cursor->mAlpha < ghVert->mAlpha ) {
			
				if ( !cursor->mNext ) {
					cursor->mNext = ghVert;
					ghVert->mPrev = cursor;
					return ghVert;
				}
			
				cursor = cursor->mNext;
			}
			
			assert ( cursor && ( cursor->mAlpha >= ghVert->mAlpha ));
			
			if ( cursor == this->mHead ) {
			
				ghVert->mNext = cursor;
				cursor->mPrev = ghVert;
				
				this->mHead = ghVert;
			}
			else {
			
				ghVert->mNext = cursor;
				ghVert->mPrev = cursor->mPrev;

				cursor->mPrev->mNext = ghVert;
				cursor->mPrev = ghVert;
			}
		}
		
		return ghVert;
	}
	
	//----------------------------------------------------------------//
	GHVertex* InsertIntersection ( GHVertex* start, const ZLVec2D& vert ) {
	
		GHVertex* ghVert		= this->mVtxFactory.AllocVertex ();
		ghVert->mX				= vert.mX;
		ghVert->mY				= vert.mY;
		ghVert->mTangent		= start->mTangent;
		ghVert->mAlpha			= ghVert->mTangent.Dot ( vert );
	
		GHVertex* cursor = start->mNext;
		GHVertex* end = start->mNextEdge;

		while ( cursor != end && cursor->mAlpha < ghVert->mAlpha ) {
			cursor = cursor->mNext;
		}
		
		ghVert->mNext = cursor;
		ghVert->mPrev = cursor->mPrev;

		cursor->mPrev->mNext = ghVert;
		cursor->mPrev = ghVert;
		
		return ghVert;
	}
	
	//----------------------------------------------------------------//
	void Print () {
	
		size_t i = 0;
	
		GHVertex* cursor = this->mHead;
		if ( cursor ) {
		
			do {
				printf ( "%d: (%g, %g) (%g, %g) - %s\n",
					i++,
					cursor->mX, cursor->mY,
					cursor->mTangent.mX, cursor->mTangent.mY,
					GHVertex::StringForIntersectionType ( cursor->mIntersectionType )
				);
				cursor = cursor->mNext;
			}
			while ( cursor && ( cursor != this->mHead ));
		}
	}
	
	//----------------------------------------------------------------//
	void Transform ( const ZLAffine3D& mtx ) {
	
		GHVertex* cursor = this->mHead;
		if ( cursor ) {
		
			do {
				ZLVec3D vtx ( cursor->mX, cursor->mY, 0.0f );
				mtx.Transform ( vtx );
				cursor->mX = vtx.mX;
				cursor->mY = vtx.mY;
				
				cursor = cursor->mNext;
			}
			while ( cursor && ( cursor != this->mHead ));
		}
	}
};

//================================================================//
// GHIntersection
//================================================================//
class GHIntersection :
	public ZLVec2D {
public:

	ZLVec2D		mSourceTangent;
	ZLVec2D		mClipTangent;
	bool		mIsValid;

	//----------------------------------------------------------------//
	GHIntersection ( const GHVertex& sourceVertex, const ZLPlane2D& clipPlane ) :
		mSourceTangent ( 0.0f, 0.0f ),
		mClipTangent ( 0.0f, 0.0f ),
		mIsValid ( false ) {
	
		ZLVec2D v0 = sourceVertex;
		
		*( ZLVec2D* )this = v0;
		
		this->mSourceTangent = sourceVertex.mTangent;
		
		this->mClipTangent = clipPlane.mNorm;
		this->mClipTangent.Rotate90Anticlockwise ();
		
		if ( ZLDist::PointToPlane2D ( v0, clipPlane ) == 0.0f ) {
			
			this->mIsValid = true;
		}
		else {
			
			ZLVec2D edge = *sourceVertex.mNextEdge;
			edge.Sub ( v0 );
			
			float t;
			
			if ( ZLSect::VecToPlane ( v0, edge, clipPlane, t ) == ZLSect::SECT_HIT ) {
				
				this->Add ( edge, t );
				this->mIsValid = (( 0.0f < t ) && ( t < 1.0f ));
			}
		}
	}

	//----------------------------------------------------------------//
	GHIntersection ( const GHVertex& sourceVertex, const GHVertex& clipVertex ) :
		mIsValid ( false ) {
	
		this->mSourceTangent = sourceVertex.mTangent;
		this->mClipTangent = clipVertex.mTangent;
	
		ZLVec2D s0 = sourceVertex;
		ZLVec2D s1 = *sourceVertex.mNextEdge;
	
		ZLVec2D c0 = clipVertex;
		ZLVec2D c1 = *clipVertex.mNextEdge;
		
		float d = (( c1.mY - c0.mY ) * ( s1.mX - s0.mX )) - (( c1.mX - c0.mX ) * ( s1.mY - s0.mY ));
		
		if ( d != 0.0f ) {
		
			float toSource		= ((( c1.mX - c0.mX ) * ( s0.mY - c0.mY )) - (( c1.mY - c0.mY ) * ( s0.mX - c0.mX ))) / d;
			float toClip		= ((( s1.mX - s0.mX ) * ( s0.mY - c0.mY )) - (( s1.mY - s0.mY ) * ( s0.mX - c0.mX ))) / d;
			
			this->mIsValid		= (( 0.0f < toSource ) && ( toSource < 1.0f )) && (( 0.0f < toClip ) && ( toClip < 1.0f ));
			
			if ( this->mIsValid ) {
				this->mX = s0.mX + ( toSource * ( s1.mX - s0.mX ));
				this->mY = s0.mY + ( toSource * ( s1.mY - s0.mY ));
			}
		}
	}
};

//================================================================//
// ZLPolygon2D
//================================================================//

//----------------------------------------------------------------//
size_t ZLPolygon2D::Clip ( const ZLPlane2D& plane, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes ) {

	size_t bufferSize = this->GetSize () * sizeof ( GHVertex ) * 3;
	void* buffer = alloca ( bufferSize );

	return this->Clip ( plane, clippedPolyVerts, clippedPolySizes, buffer, bufferSize );
}

//----------------------------------------------------------------//
size_t ZLPolygon2D::Clip ( const ZLPlane2D& plane, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes, void* buffer, size_t bufferSize ) {

	DEBUG_LOG ( "CLIP!\n" );
	
	GHVertexFactory vtxFactory;
	vtxFactory.SetBuffer ( buffer, bufferSize, bufferSize );
	
	//bool sourceForwards = true;
	//bool clipForwards = true;
	
	GHPolygon ghSourcePolygon ( vtxFactory, *this );
	GHPolygon ghClipPolygon ( vtxFactory );
	
	GHVertex* sourceVertex = ghSourcePolygon.mHead;

	#ifdef ZLPOLYGON2D_DEBUG_CLIP
		DEBUG_LOG ( "SOURCE POLYGON (pre-intersections)\n" );
		ghSourcePolygon.Print ();
		DEBUG_LOG ( "\n" );
	#endif

	size_t pointsOn		= 0;
	size_t pointsFront	= 0;
	size_t polySize		= this->GetSize ();

	// find all intersections
	
    do {
        if ( sourceVertex->mIntersectionType == GHVertex::INTERSECTION_NONE ) {
			
			float dist = ZLDist::PointToPlane2D ( *sourceVertex, plane );
			
			if ( dist == 0.0f ) pointsOn++;
			else if ( dist > 0.0f ) pointsFront++;
			
			GHIntersection intersection ( *sourceVertex, plane );
			
			if ( intersection.mIsValid ) {
				
				GHVertex* sourceIntersection = ghSourcePolygon.InsertIntersection ( sourceVertex, intersection );
				GHVertex* clipIntersection = ghClipPolygon.InsertIntersection ( intersection, intersection.mClipTangent );

				sourceIntersection->mNeighbor = clipIntersection;
				clipIntersection->mNeighbor = sourceIntersection;
			}
        }
		sourceVertex = sourceVertex->mNext;
	}
	while ( sourceVertex != ghSourcePolygon.mHead );
	
	if ( pointsFront == 0 ) return 0;
	
	if (( pointsFront + pointsOn ) == polySize ) {
				
		for ( size_t j = 0; j < polySize; ++j ) {
			const ZLVec2D& vert = this->mVertices [ j ];
			clippedPolyVerts.Write ( vert );
		}
		clippedPolySizes.Write ( polySize );
		return 1;
	}
	
	return GHPolygon::ClassifyAndClip ( ghSourcePolygon, ghClipPolygon, clippedPolyVerts, clippedPolySizes );
}

//----------------------------------------------------------------//
size_t ZLPolygon2D::Clip ( const ZLPolygon2D& poly, const ZLAffine3D* mtx, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes ) {

	size_t bufferSize = this->GetSize () * sizeof ( GHVertex ) * 3;
	void* buffer = alloca ( bufferSize );

	return this->Clip ( poly, mtx, clippedPolyVerts, clippedPolySizes, buffer, bufferSize );
}

//----------------------------------------------------------------//
size_t ZLPolygon2D::Clip ( const ZLPolygon2D& poly, const ZLAffine3D* mtx, ZLStream& clippedPolyVerts, ZLStream& clippedPolySizes, void* buffer, size_t bufferSize ) {

	DEBUG_LOG ( "CLIP!\n" );
	
	GHVertexFactory vtxFactory;
	vtxFactory.SetBuffer ( buffer, bufferSize, bufferSize );
	
	//bool sourceForwards = true;
	//bool clipForwards = true;
	
	GHPolygon ghSourcePolygon ( vtxFactory, *this );
	GHPolygon ghClipPolygon ( vtxFactory, poly );
	
	if ( mtx ) {
		ghClipPolygon.Transform ( *mtx );
	}
	
	GHVertex* sourceVertex = ghSourcePolygon.mHead;
	GHVertex* clipVertex = ghClipPolygon.mHead;

	// find all intersections
	
    do {
	
        if ( sourceVertex->mIntersectionType == GHVertex::INTERSECTION_NONE ) {
		
            do {
                if ( clipVertex->mIntersectionType == GHVertex::INTERSECTION_NONE ) {
					
					GHIntersection intersection ( *sourceVertex, *clipVertex );
					
                    if ( intersection.mIsValid ) {
						
						GHVertex* sourceIntersection = ghSourcePolygon.InsertIntersection ( sourceVertex, intersection );
						GHVertex* clipIntersection = ghClipPolygon.InsertIntersection ( clipVertex, intersection );

						sourceIntersection->mNeighbor = clipIntersection;
						clipIntersection->mNeighbor = sourceIntersection;
                    }
                }
				
                clipVertex = clipVertex->mNext;
			}
			while ( clipVertex != ghClipPolygon.mHead );
        }

		sourceVertex = sourceVertex->mNext;
	}
	while ( sourceVertex != ghSourcePolygon.mHead );
	
	return GHPolygon::ClassifyAndClip ( ghSourcePolygon, ghClipPolygon, clippedPolyVerts, clippedPolySizes );
}
