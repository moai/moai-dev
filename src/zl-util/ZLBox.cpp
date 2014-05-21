// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <float.h>
#include <zl-util/ZLBox.h>
#include <zl-util/ZLPrism.h>

//================================================================//
// ZLBox
//================================================================//

//----------------------------------------------------------------//
float ZLBox::Area () const {

	return ( this->mMax.mX - this->mMin.mX ) * ( this->mMax.mY - this->mMin.mY ) * ( this->mMax.mZ - this->mMin.mZ );
}

//----------------------------------------------------------------//
void ZLBox::Bless () {

	if ( this->mMin.mX > this->mMax.mX ) {
		float temp = this->mMin.mX;
		this->mMin.mX = this->mMax.mX;
		this->mMax.mX = temp;
	}
	
	if ( this->mMin.mY > this->mMax.mY ) {
		float temp = this->mMin.mY;
		this->mMin.mY = this->mMax.mY;
		this->mMax.mY = temp;
	}
	
	if ( this->mMin.mZ > this->mMax.mZ ) {
		float temp = this->mMin.mZ;
		this->mMin.mZ = this->mMax.mZ;
		this->mMax.mZ = temp;
	}
}

//----------------------------------------------------------------//
void ZLBox::Clip ( const ZLBox& box ) {

	// Clamp XMin
	if ( this->mMin.mX < box.mMin.mX ) this->mMin.mX = box.mMin.mX;
	if ( this->mMin.mX > box.mMax.mX ) this->mMin.mX = box.mMax.mX;
	
	// Clamp XMax
	if ( this->mMax.mX < box.mMin.mX ) this->mMax.mX = box.mMin.mX;
	if ( this->mMax.mX > box.mMax.mX ) {
		this->mMax.mX = box.mMax.mX;
	}
	
	// Clamp YMin
	if ( this->mMin.mY < box.mMin.mY ) this->mMin.mY = box.mMin.mY;
	if ( this->mMin.mY > box.mMax.mY ) this->mMin.mY = box.mMax.mY;
	
	// Clamp YMax
	if ( this->mMax.mY < box.mMin.mY ) this->mMax.mY = box.mMin.mY;
	if ( this->mMax.mY > box.mMax.mY ) this->mMax.mY = box.mMax.mY;
	
	// Clamp ZMin
	if ( this->mMin.mZ < box.mMin.mZ ) this->mMin.mZ = box.mMin.mZ;
	if ( this->mMin.mZ > box.mMax.mZ ) this->mMin.mZ = box.mMax.mZ;
	
	// Clamp ZMax
	if ( this->mMax.mZ < box.mMin.mZ ) this->mMax.mZ = box.mMin.mZ;
	if ( this->mMax.mZ > box.mMax.mZ ) this->mMax.mZ = box.mMax.mZ;
}

//----------------------------------------------------------------//
bool ZLBox::Contains ( const ZLVec3D& loc ) const {

	if (( loc.mX < mMin.mX ) || ( loc.mX > mMax.mX )) return false;
	if (( loc.mY < mMin.mY ) || ( loc.mY > mMax.mY )) return false;
	if (( loc.mZ < mMin.mZ ) || ( loc.mZ > mMax.mZ )) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLBox::Contains ( const ZLVec3D& loc, u32 plane ) const {

	switch ( plane ) {
		
		default:
		case PLANE_XY:
			if (( loc.mX < mMin.mX ) || ( loc.mX > mMax.mX )) return false;
			if (( loc.mY < mMin.mY ) || ( loc.mY > mMax.mY )) return false;
			break;
		
		case PLANE_XZ:
			if (( loc.mX < mMin.mX ) || ( loc.mX > mMax.mX )) return false;
			if (( loc.mZ < mMin.mZ ) || ( loc.mZ > mMax.mZ )) return false;
			break;
		
		case PLANE_YZ:
			if (( loc.mY < mMin.mY ) || ( loc.mY > mMax.mY )) return false;
			if (( loc.mZ < mMin.mZ ) || ( loc.mZ > mMax.mZ )) return false;
			break;
	}
	
	return true;
}

//----------------------------------------------------------------//
void ZLBox::GetCenter ( ZLVec3D& center ) const {
	
	center.mX = this->mMin.mX + (( this->mMax.mX - this->mMin.mX ) * 0.5f );
	center.mY = this->mMin.mY + (( this->mMax.mY - this->mMin.mY ) * 0.5f );
	center.mZ = this->mMin.mZ + (( this->mMax.mZ - this->mMin.mZ ) * 0.5f );
}

//----------------------------------------------------------------//
void ZLBox::GetFitting ( const ZLBox& target, ZLVec3D& offset, ZLVec3D& scale ) const {

	float w = this->Width ();
	float h = this->Height ();
	float d = this->Depth ();

	float tw = target.Width ();
	float th = target.Height ();
	float td = target.Depth ();

	scale.mX = ( w != 0.0f ) && ( tw != 0.0f ) ? tw / w : 1.0f;
	scale.mY = ( h != 0.0f ) && ( th != 0.0f ) ? th / h : 1.0f;
	scale.mZ = ( d != 0.0f ) && ( td != 0.0f ) ? td / d : 1.0f;
	
	offset.mX = target.mMin.mX - ( this->mMin.mX * scale.mX );
	offset.mY = target.mMin.mY - ( this->mMin.mY * scale.mY );
	offset.mZ = target.mMin.mZ - ( this->mMin.mZ * scale.mZ );
}

//----------------------------------------------------------------//
float ZLBox::GetMaxExtent () const {

	float max = 0.0f;
	float comp;
	
	comp = ABS ( this->mMin.mX );
	if ( max < comp ) max = comp;
	
	comp = ABS ( this->mMin.mY );
	if ( max < comp ) max = comp;
	
	comp = ABS ( this->mMin.mZ );
	if ( max < comp ) max = comp;
	
	comp = ABS ( this->mMax.mX );
	if ( max < comp ) max = comp;
	
	comp = ABS ( this->mMax.mY );
	if ( max < comp ) max = comp;
	
	comp = ABS ( this->mMax.mZ );
	if ( max < comp ) max = comp;
	
	return max;
}

//----------------------------------------------------------------//
float ZLBox::GetRadius () const {

	ZLVec3D spans = mMax;
	spans.Sub ( mMin );
	spans.Scale ( 0.5f );
	return spans.Length ();
}

//----------------------------------------------------------------//
ZLRect ZLBox::GetRect ( u32 plane ) const {
	
	ZLRect rect;
	
	switch ( plane ) {
		
		default:
		case PLANE_XY:
			rect.mXMin = this->mMin.mX;
			rect.mXMax = this->mMax.mX;
			rect.mYMin = this->mMin.mY;
			rect.mYMax = this->mMax.mY;
			break;
		
		case PLANE_XZ:
			rect.mXMin = this->mMin.mX;
			rect.mXMax = this->mMax.mX;
			rect.mYMin = this->mMin.mZ;
			rect.mYMax = this->mMax.mZ;
			break;
		
		case PLANE_YZ:
			rect.mXMin = this->mMin.mZ;
			rect.mXMax = this->mMax.mZ;
			rect.mYMin = this->mMin.mY;
			rect.mYMax = this->mMax.mY;
			break;
	}
	
	return rect;
}

//----------------------------------------------------------------//
void ZLBox::Grow ( const ZLBox& box ) {

	if ( mMin.mX > box.mMin.mX ) mMin.mX = box.mMin.mX;
	if ( mMax.mX < box.mMax.mX ) mMax.mX = box.mMax.mX;
	
	if ( mMin.mY > box.mMin.mY ) mMin.mY = box.mMin.mY;
	if ( mMax.mY < box.mMax.mY ) mMax.mY = box.mMax.mY;
	
	if ( mMin.mZ > box.mMin.mZ ) mMin.mZ = box.mMin.mZ;
	if ( mMax.mZ < box.mMax.mZ ) mMax.mZ = box.mMax.mZ;
}

//----------------------------------------------------------------//
void ZLBox::Grow ( const ZLVec3D& vec ) {

	if ( mMin.mX > vec.mX ) mMin.mX = vec.mX;
	else if ( mMax.mX < vec.mX ) mMax.mX = vec.mX;
	
	if ( mMin.mY > vec.mY ) mMin.mY = vec.mY;
	else if ( mMax.mY < vec.mY ) mMax.mY = vec.mY;

	if ( mMin.mZ > vec.mZ ) mMin.mZ = vec.mZ;
	else if ( mMax.mZ < vec.mZ ) mMax.mZ = vec.mZ;
}

//----------------------------------------------------------------//
void ZLBox::Inflate ( float size ) {

	this->mMin.mX -= size;
	this->mMin.mY -= size;
	this->mMin.mZ -= size;
	
	this->mMax.mX += size;
	this->mMax.mY += size;
	this->mMax.mZ += size;
}

//----------------------------------------------------------------//
void ZLBox::Init ( const ZLBox& box ) {

	*this = box;
}

//----------------------------------------------------------------//
void ZLBox::Init ( const ZLPrism& prism ) {
	
	this->mMin = prism.mLoc;
	this->mMax = prism.mLoc;
	
	// X Axis
	if ( prism.mXAxis.mX < 0.0f )	this->mMin.mX += prism.mXAxis.mX;
	else							this->mMax.mX += prism.mXAxis.mX;
	
	if ( prism.mYAxis.mX < 0.0f )	this->mMin.mX += prism.mYAxis.mX;
	else							this->mMax.mX += prism.mYAxis.mX;
	
	if ( prism.mZAxis.mX < 0.0f )	this->mMin.mX += prism.mZAxis.mX;
	else							this->mMax.mX += prism.mZAxis.mX;
	
	// Y Axis
	if ( prism.mXAxis.mY < 0.0f )	this->mMin.mY += prism.mXAxis.mY;
	else							this->mMax.mY += prism.mXAxis.mY;
	
	if ( prism.mYAxis.mY < 0.0f )	this->mMin.mY += prism.mYAxis.mY;
	else							this->mMax.mY += prism.mYAxis.mY;
	
	if ( prism.mZAxis.mY < 0.0f )	this->mMin.mY += prism.mZAxis.mY;
	else							this->mMax.mY += prism.mZAxis.mY;
	
	// Z Axis
	if ( prism.mXAxis.mZ < 0.0f )	this->mMin.mZ += prism.mXAxis.mZ;
	else							this->mMax.mZ += prism.mXAxis.mZ;
	
	if ( prism.mYAxis.mZ < 0.0f )	this->mMin.mZ += prism.mYAxis.mZ;
	else							this->mMax.mZ += prism.mYAxis.mZ;
	
	if ( prism.mZAxis.mZ < 0.0f )	this->mMin.mZ += prism.mZAxis.mZ;
	else							this->mMax.mZ += prism.mZAxis.mZ;
}

//----------------------------------------------------------------//
void ZLBox::Init ( const ZLVec3D& vec ) {

	mMin.mX = mMax.mX = vec.mX;
	mMin.mY = mMax.mY = vec.mY;
	mMin.mZ = mMax.mZ = vec.mZ;
}

//----------------------------------------------------------------//
void ZLBox::Init ( const ZLRect& rect, u32 plane, float back, float front ) {

	switch ( plane ) {
		
		default:
		case PLANE_XY:
			this->mMin.mX = rect.mXMin;
			this->mMax.mX = rect.mXMax;
			this->mMin.mY = rect.mYMin;
			this->mMax.mY = rect.mYMax;
			this->mMin.mZ = back;
			this->mMax.mZ = front;
			break;
		
		case PLANE_XZ:
			this->mMin.mX = rect.mXMin;
			this->mMax.mX = rect.mXMax;
			this->mMin.mZ = rect.mYMin;
			this->mMax.mZ = rect.mYMax;
			this->mMin.mY = back;
			this->mMax.mY = front;
			break;
		
		case PLANE_YZ:
			this->mMin.mZ = rect.mXMin;
			this->mMax.mZ = rect.mXMax;
			this->mMin.mY = rect.mYMin;
			this->mMax.mY = rect.mYMax;
			this->mMin.mX = back;
			this->mMax.mX = front;
			break;
	}
}

//----------------------------------------------------------------//
void ZLBox::Init ( float left, float top, float right, float bottom, float back, float front ) {

	this->mMin.mX = left;
	this->mMax.mX = right;

	this->mMax.mY = top;
	this->mMin.mY = bottom;
	
	this->mMin.mZ = back;
	this->mMax.mZ = front;
}

//----------------------------------------------------------------//
bool ZLBox::IsPoint () {

	if ( this->mMin.mX != this->mMax.mX ) return false;
	if ( this->mMin.mY != this->mMax.mY ) return false;
	if ( this->mMin.mZ != this->mMax.mZ ) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLBox::IsSame ( const ZLBox& box ) const {

	return (( this->mMin.mX == box.mMax.mX ) && ( this->mMin.mY == box.mMax.mY ) && ( this->mMin.mZ == box.mMax.mZ ));
}

//----------------------------------------------------------------//
void ZLBox::Offset ( const ZLVec3D& offset ) {

	mMin.mX += offset.mX;
	mMax.mX += offset.mX;

	mMin.mY += offset.mY;
	mMax.mY += offset.mY;

	mMin.mZ += offset.mZ;
	mMax.mZ += offset.mZ;
}

//----------------------------------------------------------------//
bool ZLBox::Overlap ( const ZLBox& box ) const {

	if (( mMin.mX > box.mMax.mX ) || ( mMax.mX < box.mMin.mX )) return false;
	if (( mMin.mY > box.mMax.mY ) || ( mMax.mY < box.mMin.mY )) return false;
	if (( mMin.mZ > box.mMax.mZ ) || ( mMax.mZ < box.mMin.mZ )) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLBox::Overlap ( const ZLBox& box, u32 plane ) const {

	switch ( plane ) {
		
		default:
		case PLANE_XY:
			if (( mMin.mX > box.mMax.mX ) || ( mMax.mX < box.mMin.mX )) return false;
			if (( mMin.mY > box.mMax.mY ) || ( mMax.mY < box.mMin.mY )) return false;
			break;
		
		case PLANE_XZ:
			if (( mMin.mX > box.mMax.mX ) || ( mMax.mX < box.mMin.mX )) return false;
			if (( mMin.mZ > box.mMax.mZ ) || ( mMax.mZ < box.mMin.mZ )) return false;
			break;
		
		case PLANE_YZ:
			if (( mMin.mY > box.mMax.mY ) || ( mMax.mY < box.mMin.mY )) return false;
			if (( mMin.mZ > box.mMax.mZ ) || ( mMax.mZ < box.mMin.mZ )) return false;
			break;
	}
	
	return true;
}

//----------------------------------------------------------------//
void ZLBox::Pad ( float pad ) {

	this->Pad ( pad, pad, pad );
}

//----------------------------------------------------------------//
void ZLBox::Pad ( float xPad, float yPad, float zPad ) {

	this->mMin.mX -= xPad;
	this->mMin.mY -= yPad;
	this->mMin.mZ -= zPad;

	this->mMax.mX += xPad;
	this->mMax.mY += yPad;
	this->mMax.mZ += zPad;
}

//----------------------------------------------------------------//
void ZLBox::Scale ( float scale ) {

	mMin.mX *= scale;
	mMax.mX *= scale;

	mMin.mY *= scale;
	mMax.mY *= scale;

	mMin.mZ *= scale;
	mMax.mZ *= scale;
}

//----------------------------------------------------------------//
void ZLBox::Scale ( const ZLVec3D& scale ) {

	mMin.mX *= scale.mX;
	mMax.mX *= scale.mX;

	mMin.mY *= scale.mY;
	mMax.mY *= scale.mY;

	mMin.mZ *= scale.mZ;
	mMax.mZ *= scale.mZ;
}

//----------------------------------------------------------------//
void ZLBox::Transform ( const ZLAffine3D& mtx ) {

	ZLPrism prism;
	prism.Init ( *this );
	prism.Transform ( mtx );
	
	this->Init ( prism );
}

//----------------------------------------------------------------//
void ZLBox::Transform ( const ZLMatrix4x4& mtx ) {

	ZLPrism prism;
	prism.Init ( *this );
	prism.Transform ( mtx );
	
	this->Init ( prism );
}
