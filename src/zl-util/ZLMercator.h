// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMERCATOR_H
#define ZLMERCATOR_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLMercator
//================================================================//
namespace ZLMercator {

	//----------------------------------------------------------------//
	ZLDoubleVec2D		Project					( double lon, double lat );
	ZLDoubleVec2D		ProjectInv				( double x, double y );
	ZLDoubleVec2D		ProjectLonLatToWorld	( double lon, double lat );
};

#endif
