// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMERCATOR_H
#define USMERCATOR_H

#include <uslscore/USVec2D.h>

//================================================================//
// USMercator
//================================================================//
namespace USMercator {

	//----------------------------------------------------------------//
	USVec2D64	Project					( double lon, double lat );
	USVec2D64	ProjectInv				( double x, double y );
	USVec2D		ProjectLonLatToWorld	( double lon, double lat );
};

#endif
