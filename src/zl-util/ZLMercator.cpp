// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLMercator.h>

//================================================================//
// ZLMercator
//================================================================//

//----------------------------------------------------------------//
ZLVec2D64 ZLMercator::Project ( double lon, double lat ) {

	ZLVec2D64 coord;
		
	coord.mX = lon * D2R;
	coord.mY = log ( tan ( FORTPI + .5 * lat * D2R ));
		
	return coord;
}

//----------------------------------------------------------------//
ZLVec2D64 ZLMercator::ProjectInv ( double x, double y ) {
	
	ZLVec2D64 lonLat;
	
	lonLat.mX = ( x ) * R2D;
	lonLat.mY = ( HALFPI - 2.0 * atan ( exp ( -y ))) * R2D;
	
	return lonLat;
}

//----------------------------------------------------------------//
// TODO: templatize this
ZLVec2D ZLMercator::ProjectLonLatToWorld ( double lon, double lat ) {

	ZLVec2D64 loc2d = Project ( lon, lat );
	loc2d.Scale ( 1.0 / TWOPI );

	ZLVec2D loc2f;

	loc2f.mX = ( float )( loc2d.mX + 0.5 );
	loc2f.mY = ( float )( loc2d.mY - 0.5 );

	return loc2f;
}

