// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLFloat.h>
#include <zl-util/ZLHull2D.h>
#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLRadixSort32.h>
#include <zl-util/ZLStream.h>

typedef ZLRadixKey32 < ZLVec2D > ZLRadixVec2D;

//================================================================//
// ZLHull2D
//================================================================//

//----------------------------------------------------------------//
ZLSizeResult ZLHull2D::MonotoneChain ( ZLVec2D* hull, ZLVec2D* points, size_t nPoints, u32 sort ) {

	size_t nHull = 0;
	size_t size = nPoints * sizeof ( ZLVec2D );

	switch ( sort ) {

		case SORT_RADIX: {

			ZL_DECLARE_SCOPED_BUFFER ( radixBuffer, ZLRadixVec2D, 2 * size, 1024 )
			if ( !radixBuffer ) ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR );
			
			ZLRadixVec2D* swapBuffer = ( ZLRadixVec2D* )(( size_t )radixBuffer + size );

			for ( size_t i = 0; i < nPoints; ++i ) {
				const ZLVec2D& p = points [ i ];
				radixBuffer [ i ].mData = p;
				radixBuffer [ i ].mKey = ZLFloat::FloatToIntKey (( p.mX * ( float )nPoints ) + p.mY ); // sort by x then y
			}

			radixBuffer = RadixSort32 < ZLRadixVec2D >( radixBuffer, swapBuffer, nPoints );
			
			for ( size_t i = 0; i < nPoints; ++i ) {
				points [ i ] = radixBuffer [ i ].mData;
			}
			return ZLHull2D::MonotoneChain ( hull, points, nPoints, SORT_NONE );
		}
		
		case SORT_CSTDLIB: {
		
			qsort ( points, nPoints, sizeof ( ZLVec2D ), ZLHull2D::MonotoneChainComp );
			return ZLHull2D::MonotoneChain ( hull, points, nPoints, SORT_NONE );
		}
		
		case SORT_NONE:
		default: {
		
			for ( size_t i = 0; i < nPoints; i++ ) {
				while ( nHull >= 2 && hull [ nHull - 2 ].CrossJoint ( hull [ nHull - 1 ], points [ i ]) <= 0 ) nHull--;
				hull [ nHull++ ] = points [ i ];
			}

			for ( size_t i = nPoints - 2, t = nHull + 1; i != ( size_t )-1; i-- ) {
				while ( nHull >= t && hull [ nHull - 2 ].CrossJoint ( hull [ nHull - 1 ], points [ i ]) <= 0 ) nHull--;
				hull [ nHull++ ] = points [ i ];
			}
		}
	}

	ZL_RETURN_SIZE_RESULT ( nHull - 1, ZL_OK );
}

//----------------------------------------------------------------//
ZLSizeResult ZLHull2D::MonotoneChain ( ZLStream& output, ZLStream& input, size_t nPoints, u32 sort ) {

	u32 resultCode = ZL_OK;
	size_t nHull = 0;
	
	size_t size = nPoints * sizeof ( ZLVec2D );

	ZL_DECLARE_SCOPED_BUFFER ( points, ZLVec2D, size, 512 )
	ZL_DECLARE_SCOPED_BUFFER ( hull, ZLVec2D, 2 * size, 512 )

	if ( !( points && hull )) ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR )

	ZLSizeResult readResult = input.ReadBytes ( points, size );
	ZL_RETURN_SIZE_RESULT_IF_NOT_EQUAL ( readResult, size, 0, ZL_RANGE_ERROR )

	ZLSizeResult chainResult = ZLHull2D::MonotoneChain ( hull, points, nPoints, sort );
	
	if ( chainResult.mCode == ZL_OK ) {
	
		size = chainResult.mValue * sizeof ( ZLVec2D );
	
		ZLSizeResult writeResult = output.WriteBytes ( hull, size );
		ZL_RETURN_SIZE_RESULT_IF_NOT_EQUAL ( writeResult, size, 0, ZL_RANGE_ERROR )
	}
	return chainResult;
}

//----------------------------------------------------------------//
int ZLHull2D::MonotoneChainComp ( const void* p1, const void* p2 ) {

	const ZLVec2D* v0 = ( ZLVec2D* )p1;
	const ZLVec2D* v1 = ( ZLVec2D* )p2;
	
	if ( v0->mX < v1->mX ) return -1;
	if ( v0->mX > v1->mX ) return 1;
	
	if ( v0->mY < v1->mY ) return -1;
	if ( v0->mY > v1->mY ) return 1;
	
	return 0;
}
