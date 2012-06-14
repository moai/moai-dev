// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef POLYSCANNER_H
#define POLYSCANNER_H

#include <math.h>

//================================================================//
// USRenderEdge
//================================================================//
class USRenderEdge {
public:

	float	mX0;
	float	mY0;
	float	mY1;
	
	float	mInvSlope;
	float	mXIntercept;
	
	//----------------------------------------------------------------//
	inline float GetX ( float y ) {
	
		return ( mInvSlope * y ) + this->mXIntercept;
	}
	
	//----------------------------------------------------------------//
	inline void Init ( USVec2D& p0, USVec2D& p1 ) {

		this->mX0 = p0.mX;
		this->mY0 = p0.mY;
		this->mY1 = p1.mY;
		
		this->mInvSlope = ( p1.mX - p0.mX )/( p1.mY - p0.mY );
		this->mXIntercept = p0.mX - ( this->mInvSlope * p0.mY );
	}
};

//================================================================//
// USEdgeList
//================================================================//
template < u32 TOTAL_EDGES >
class USEdgeList {
public:

	float		mLeft;
	float		mRight;
	
	u32				mEdgeID;
	u32				mTotalEdges;
	USRenderEdge	mEdges [ TOTAL_EDGES ];
	
	//----------------------------------------------------------------//
	inline void AddEdge ( USVec2D& p0, USVec2D& p1 ) {
	
		float diff = p1.mY - p0.mY;
		if ( diff < 0.0f ) diff = -diff;
		if ( diff < 0.0001f ) return;
	
		if ( p0.mY <= p1.mY ) {
		
			this->mEdges [ this->mTotalEdges++ ].Init ( p0, p1 );
		}
	}
	
	//----------------------------------------------------------------//
	USEdgeList () :
		mTotalEdges ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	inline void FindExtents ( float y0, float y1 ) {
	
		USRenderEdge* renderEdge = &this->mEdges [ this->mEdgeID ];
		
		this->mLeft = renderEdge->GetX ( y0 );
		this->mRight = this->mLeft;
		
		u32 i = this->mEdgeID + 1;
		while (( renderEdge->mY1 < y1 ) && ( i < this->mTotalEdges )) {
			
			this->mEdgeID = i;
			renderEdge = &this->mEdges [ i ];
			
			float comp = renderEdge->mX0;
			if ( comp < this->mLeft ) this->mLeft = comp;
			if ( comp > this->mRight ) this->mRight = comp;
			
			i++;
		}

		float comp = renderEdge->GetX ( y1 );
		if ( comp < this->mLeft ) this->mLeft = comp;
		if ( comp > this->mRight ) this->mRight = comp;
	}
};

//================================================================//
// USScanline
//================================================================//
class USScanline {
public:
	
	s32		mRow;
	s32		mNextRow;
	
	s32		mLeftCol;
	s32		mRightCol;
};

//================================================================//
// USPolyScanner
//================================================================//
template < u32 TOTAL_SIDES >
class USPolyScanner {
public:

	s32			mXMin;
	s32			mXMax;

	s32			mYMin;
	s32			mYMax;

	USEdgeList < TOTAL_SIDES >	mLeftEdges;
	USEdgeList < TOTAL_SIDES >	mRightEdges;
	
	//----------------------------------------------------------------//
	inline void Init ( USVec2D* poly ) {
		
		// Find the top (i.e. starting) vert
		u32 top = 0;
		
		float yMin = poly [ 0 ].mY;
		float yMax = poly [ 0 ].mY;
		
		float xMin = poly [ 0 ].mX;
		float xMax = poly [ 0 ].mX;
		
		// get the extents
		for ( u32 i = 1; i < TOTAL_SIDES; ++i ) {
			
			float y = poly [ i ].mY;
			
			if ( y < this->mYMin ) {
				yMin = y;
				top = i;
			}
			
			if ( y > yMax ) {
				yMax = y;
			}
			
			float x = poly [ i ].mX;
			if ( x < xMin ) xMin = x;
			if ( x > xMax ) xMax = x;
		}
		
		// Build edge lists
		s32 left = top;
		s32 right = top;
		
		for ( u32 i = 0; i < TOTAL_SIDES; ++i ) {
			
			u32 left0 = left;
			left = ( left - 1 ) % TOTAL_SIDES;
			if ( left < 0 ) left += TOTAL_SIDES;
			u32 left1 = left;
			
			this->mLeftEdges.AddEdge ( poly [ left0 ], poly [ left1 ]);
			
			u32 right0 = right;
			right = ( right + 1 ) % TOTAL_SIDES;
			u32 right1 = right;
			
			this->mRightEdges.AddEdge ( poly [ right0 ], poly [ right1 ]);
		}
		
		assert ( this->mLeftEdges.mTotalEdges );
		assert ( this->mRightEdges.mTotalEdges );
		
		this->mXMin = ( s32 )USFloat::ToInt ( xMin );
		this->mXMax = ( s32 )USFloat::ToInt ( xMax );
		
		this->mYMin = ( s32 )USFloat::ToInt ( yMin );
		this->mYMax = ( s32 )USFloat::ToInt ( yMax ) + 1;
	}
	
	//----------------------------------------------------------------//
	inline void Start ( USScanline& scan ) {
		
		scan.mRow = ( s32 )floorf ( this->mYMin ); // floor yMin to get starting row
		scan.mNextRow = scan.mRow;
		
		scan.mLeftCol = 0;
		scan.mRightCol = 0;
		
		this->mLeftEdges.mEdgeID = 0;
		this->mRightEdges.mEdgeID = 0;
	}
	
	//----------------------------------------------------------------//
	inline bool Step ( USScanline& scan ) {
	
		scan.mRow = scan.mNextRow;
	
		float y0;
		float y1;
		
		y0 = ( float )scan.mRow;
		y1 = y0 + 1.0f;
		
		if ( y0 < this->mYMin ) y0 = this->mYMin;
		if ( y1 > this->mYMax ) y1 = this->mYMax;
		
		this->mLeftEdges.FindExtents ( y0, y1 );
		this->mRightEdges.FindExtents ( y0, y1 );
		
		if ( this->mLeftEdges.mLeft < this->mRightEdges.mLeft ) {
			scan.mLeftCol = ( s32 )USFloat::ToInt ( this->mLeftEdges.mLeft );
		}
		else {
			scan.mLeftCol = ( s32 )USFloat::ToInt ( this->mRightEdges.mLeft );
		}
		
		if ( this->mLeftEdges.mRight > this->mRightEdges.mRight ) {
			scan.mRightCol = ( s32 )USFloat::ToInt ( this->mLeftEdges.mRight );
		}
		else {
			scan.mRightCol = ( s32 )USFloat::ToInt ( this->mRightEdges.mRight );
		}
		
		if ( scan.mLeftCol < this->mXMin ) {
			scan.mLeftCol =  this->mXMin;
		}
		if ( scan.mRightCol > this->mXMax ) {
			scan.mRightCol =  this->mXMax;
		}
		
		if ( y1 < this->mYMax ) {
			scan.mNextRow = scan.mRow + 1;
			return true;
		}
		
		return false;
	}
	
	//----------------------------------------------------------------//
	u32 TotalSides () {
	
		return TOTAL_SIDES;
	}
};

//================================================================//
// QuadScanner
//================================================================//
class QuadScanner :
	public USPolyScanner < 4 > {
public:
};

#endif
