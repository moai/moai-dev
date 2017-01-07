// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-util/ZLBounds.h>

//================================================================//
// ZLBounds
//================================================================//

const ZLBounds ZLBounds::EMPTY ( ZLBounds::ZL_BOUNDS_EMPTY );
const ZLBounds ZLBounds::GLOBAL ( ZLBounds::ZL_BOUNDS_GLOBAL );

//----------------------------------------------------------------//
ZLBounds::ZLBounds () {
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( u32 status ) :
	mStatus ( status ) {
	
	this->mMin = ZLVec3D::ORIGIN;
	this->mMax = ZLVec3D::ORIGIN;
}
