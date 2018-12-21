// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#include "pch.h"
#include <zl-util/ZLBounds.h>

//================================================================//
// ZLBounds
//================================================================//

const ZLBounds ZLBounds::EMPTY ( ZLBounds::ZL_BOUNDS_EMPTY );
const ZLBounds ZLBounds::GLOBAL ( ZLBounds::ZL_BOUNDS_GLOBAL );

//----------------------------------------------------------------//
void ZLBounds::Init ( const ZLBox& box ) {

	this->mAABB = box;
	this->mOBB.Init ( box );
	this->UpdateStatus ();
}

//----------------------------------------------------------------//
void ZLBounds::Init ( const ZLPrism& prism ) {

	this->mAABB.Init ( prism );
	this->mOBB = prism;
	this->UpdateStatus ();
}

//----------------------------------------------------------------//
void ZLBounds::Init ( const ZLRect& rect ) {

	this->Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin );
}

//----------------------------------------------------------------//
void ZLBounds::Init ( float left, float top, float right, float bottom, float back, float front ) {

	this->mAABB.Init ( left, top, right, bottom, back, front );
	this->mOBB.Init ( this->mAABB );
	this->UpdateStatus ();
}

//----------------------------------------------------------------//
bool ZLBounds::IsOK () const {

	return ( this->mStatus == ZL_BOUNDS_OK );
}

//----------------------------------------------------------------//
void ZLBounds::UpdateStatus () {

	this->mStatus = this->mAABB.IsPoint () ? ZLBounds::ZL_BOUNDS_EMPTY : ZLBounds::ZL_BOUNDS_OK;
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds () {
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( u32 status ) :
	mStatus ( status ) {
	
	this->mAABB		= ZLBox::EMPTY;
	this->mOBB		=  ZLPrism::EMPTY;
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( const ZLBox& box ) {
	this->Init ( box );
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( const ZLPrism& prism ) {
	this->Init ( prism );
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( const ZLRect& rect ) {

	this->Init ( rect );
}

//----------------------------------------------------------------//
ZLBounds::ZLBounds ( float left, float top, float right, float bottom, float back, float front ) {
	this->Init ( left, top, right, bottom, back, front );
}
