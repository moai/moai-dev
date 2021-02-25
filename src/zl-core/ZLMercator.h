// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMERCATOR_H
#define ZLMERCATOR_H

#include <zl-core/ZLVec2D.h>

//================================================================//
// ZLMercator
//================================================================//
namespace ZLMercator {

	//----------------------------------------------------------------//
	ZLVec2D64	Project					( double lon, double lat );
	ZLVec2D64	ProjectInv				( double x, double y );
	ZLVec2D		ProjectLonLatToWorld	( double lon, double lat );
};

#endif
