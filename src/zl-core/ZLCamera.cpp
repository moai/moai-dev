// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLCamera.h>
#include <zl-core/ZLViewport.h>

#define DEFAULT_HFOV			60.0f
#define DEFAULT_NEAR_PLANE		1.0f
#define DEFAULT_FAR_PLANE		10000.0f

//================================================================//
// ZLCamera
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 ZLCamera::GetBillboardMtx () const {

	ZLMatrix4x4 mtx = this->mViewMtx;
	mtx.m [ ZLMatrix4x4::C3_R0 ] = 0.0f;
	mtx.m [ ZLMatrix4x4::C3_R1 ] = 0.0f;
	mtx.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
	return mtx;
}

//----------------------------------------------------------------//
float ZLCamera::GetFocalLength ( float width ) const {

	if ( this->mType == CAMERA_TYPE_3D ) {
		float c = Cot ( this->mFieldOfView * 0.5f * ( float )D2R );
		return width * c * 0.5f;
	}
	return 0.0f;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLCamera::GetProjMtx ( const ZLViewport& viewport ) const {
	
	ZLMatrix4x4 proj;
	ZLMatrix4x4 mtx;

	// rotate
	proj.RotateZ ( -viewport.GetRotation () * ( float )D2R );
	
	// project
	
	ZLVec2D viewScale = viewport.GetScale (); // TODO: bit confusing in the 3d; clarify
	
	if ( this->mUseProjectionMtx ) {
	
		mtx = this->mProjectionMtx;
	}
	else {
		
		switch ( this->mType ) {
		
			case CAMERA_TYPE_ORTHO: {
				
				float xs = ( 2.0f / viewport.Width ()) * viewScale.mX;
				float ys = ( 2.0f / viewport.Height ()) * viewScale.mY;
				
				mtx.Ortho ( xs, ys, this->mNearPlane, this->mFarPlane );
				break;
			}
			case CAMERA_TYPE_3D: {
				
				float xs = Tan (( this->mFieldOfView * ( float )D2R ) / 2.0f ) * this->mNearPlane;
				float ys = xs / viewport.GetAspect ();
				
				xs *= viewScale.mX;
				ys *= viewScale.mY;

				mtx.Frustum ( -xs, ys, xs, -ys, this->mNearPlane, this->mFarPlane );
				
				break;
			}
			case CAMERA_TYPE_WINDOW:
			default: {
				
				float xScale = ( 2.0f / viewport.Width ()) * viewScale.mX;
				float yScale = ( 2.0f / viewport.Height ()) * viewScale.mY;
				
				mtx.Scale ( xScale, yScale, -1.0 ); // Z must be non-zero to produce invertible projection matrix
			}
		}
	}
	
	proj.Append ( mtx );
	
	// offset
	ZLVec2D offset = viewport.GetOffset ();
	mtx.Translate ( offset.mX, offset.mY, 0.0f );
	proj.Append ( mtx );
	
	return proj;
}

//----------------------------------------------------------------//
ZLVec3D ZLCamera::GetViewVector () const {

	ZLVec3D viewVec = this->mViewMtx.GetZAxis ();
	viewVec.Norm ();
	viewVec.Scale ( -1.0f );
	return viewVec;
}

//----------------------------------------------------------------//
ZLCamera::ZLCamera () :
	mUseProjectionMtx ( false ),
	mFieldOfView ( DEFAULT_HFOV ),
	mNearPlane ( DEFAULT_NEAR_PLANE ),
	mFarPlane ( DEFAULT_FAR_PLANE ),
	mType ( CAMERA_TYPE_WINDOW ) {
	
	RTTI_SINGLE ( ZLCamera, RTTIBase )
	
	this->mViewMtx = ZLMatrix4x4::IDENT;
	this->mProjectionMtx = ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
ZLCamera::~ZLCamera () {
}

//----------------------------------------------------------------//
void ZLCamera::SetProjMtx () {

	this->mProjectionMtx = ZLMatrix4x4::IDENT;
	this->mUseProjectionMtx = false;
}

//----------------------------------------------------------------//
void ZLCamera::SetProjMtx ( const ZLMatrix4x4& mtx ) {

	this->mProjectionMtx = mtx;
	this->mUseProjectionMtx = true;
}
