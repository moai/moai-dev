// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USMathConsts.h>
#include <uslscore/USMercator.h>

//================================================================//
// USMercator
//================================================================//

//----------------------------------------------------------------//
USVec2D64 USMercator::Project ( double lon, double lat ) {

	USVec2D64 coord;
		
	coord.mX = lon * D2R;
	coord.mY = log ( tan ( FORTPI + .5 * lat * D2R ));
		
	return coord;
}

//----------------------------------------------------------------//
USVec2D64 USMercator::ProjectInv ( double x, double y ) {
	
	USVec2D64 lonLat;
	
	lonLat.mX = ( x ) * R2D;
	lonLat.mY = ( HALFPI - 2.0 * atan ( exp ( -y ))) * R2D;
	
	return lonLat;
}

//----------------------------------------------------------------//
// TODO: templatize this
USVec2D USMercator::ProjectLonLatToWorld ( double lon, double lat ) {

	USVec2D64 loc2d = Project ( lon, lat );
	loc2d.Scale ( 1.0 / TWOPI );

	USVec2D loc2f;

	loc2f.mX = ( float )( loc2d.mX + 0.5 );
	loc2f.mY = ( float )( loc2d.mY - 0.5 );

	return loc2f;
}

