// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-util/ZLBounds.h>

//================================================================//
// ZLBounds
//================================================================//

const ZLBounds ZLBounds::EMPTY ( ZLBounds::ZL_BOUNDS_EMPTY );
const ZLBounds ZLBounds::GLOBAL ( ZLBounds::ZL_BOUNDS_GLOBAL );

//----------------------------------------------------------------//
void ZLBounds::Init ( const ZLBox& box ) {

	this->ZLBox::Init ( box );
	this->UpdateStatus ();
}

//----------------------------------------------------------------//
void ZLBounds::Init ( const ZLRect& rect ) {

	this->Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin );
}

//----------------------------------------------------------------//
void ZLBounds::Init ( float left, float top, float right, float bottom, float back, float front ) {

	this->ZLBox::Init ( left, top, right, bottom, back, front );
	this->UpdateStatus ();
}

//----------------------------------------------------------------//
void ZLBounds::UpdateStatus () {

	this->mStatus = this->IsPoint () ? ZLBounds::ZL_BOUNDS_EMPTY : ZLBounds::ZL_BOUNDS_OK;
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds () {
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( u32 status ) :
	mStatus ( status ) {
	
	this->mMin = ZLVec3D::ORIGIN;
	this->mMax = ZLVec3D::ORIGIN;
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( const ZLRect& rect ) {

	this->Init ( rect );
}
