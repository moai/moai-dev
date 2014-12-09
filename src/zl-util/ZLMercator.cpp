// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLMercator.h>

//================================================================//
// ZLMercator
//================================================================//

//----------------------------------------------------------------//
ZLDoubleVec2D ZLMercator::Project ( double lon, double lat ) {

	ZLDoubleVec2D coord;
		
	coord.mX = lon * D2R;
	coord.mY = log ( tan ( FORTPI + .5 * lat * D2R ));
		
	return coord;
}

//----------------------------------------------------------------//
ZLDoubleVec2D ZLMercator::ProjectInv ( double x, double y ) {
	
	ZLDoubleVec2D lonLat;
	
	lonLat.mX = ( x ) * R2D;
	lonLat.mY = ( HALFPI - 2.0 * atan ( exp ( -y ))) * R2D;
	
	return lonLat;
}

//----------------------------------------------------------------//
// TODO: templatize this
ZLDoubleVec2D ZLMercator::ProjectLonLatToWorld ( double lon, double lat ) {

	ZLDoubleVec2D loc2d = Project ( lon, lat );
	loc2d.Scale ( 1.0 / TWOPI );

	ZLDoubleVec2D result;

	result.mX = ( real )( loc2d.mX + 0.5 );
	result.mY = ( real )( loc2d.mY - 0.5 );

	return result;
}

