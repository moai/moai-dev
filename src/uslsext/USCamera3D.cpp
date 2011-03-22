// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USCamera3D.h>
#include <uslsext/USMathConsts.h>

//================================================================//
// USCamera3D
//================================================================//

//----------------------------------------------------------------//
void USCamera3D::BuildFrustum ( USViewport& viewport, USFrustum& frustum ) {
	
	float invAspect = viewport.GetInvAspect ();
	float vfov = ATan ( Tan ( mHFOV * 0.5f ) * ( invAspect )) * 2.0f;
	
	USVec3D viewAxis = this->mLook;
	viewAxis.Sub ( this->mLoc );
	viewAxis.Norm ();
	
	USVec3D worldUp ( 0.0f, 1.0f, 0.0f );
	
	// TODO: fix this so we don't need VFOV
	frustum.Init ( this->mLoc, viewAxis, worldUp, mNear, mFar, this->mHFOV, vfov );
}

//----------------------------------------------------------------//
void USCamera3D::GetLookAtMtx ( USMatrix3D& mtx ) {

	// zaxis = normal ( Eye - At )
	USVec3D zAxis = mLoc;
	zAxis.Sub ( mLook );
	zAxis.Norm ();
	
	// xaxis = normal ( cross ( Up, zaxis ))
	USVec3D xAxis = mUp;
	xAxis.Cross ( zAxis );
	xAxis.Norm ();
	
	// yaxis = cross ( zaxis, xaxis )
	USVec3D yAxis = zAxis;
	yAxis.Cross ( xAxis );
	yAxis.Norm ();
	    
	// xaxis.x           yaxis.x           zaxis.x          0
	// xaxis.y           yaxis.y           zaxis.y          0
	// xaxis.z           yaxis.z           zaxis.z          0
	// -dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l

	mtx.m [ USMatrix3D::C0_R0 ] = xAxis.mX;
	mtx.m [ USMatrix3D::C1_R0 ] = xAxis.mY;
	mtx.m [ USMatrix3D::C2_R0 ] = xAxis.mZ;
	mtx.m [ USMatrix3D::C3_R0 ] = -xAxis.Dot ( mLoc );

	mtx.m [ USMatrix3D::C0_R1 ] = yAxis.mX;
	mtx.m [ USMatrix3D::C1_R1 ] = yAxis.mY;
	mtx.m [ USMatrix3D::C2_R1 ] = yAxis.mZ;
	mtx.m [ USMatrix3D::C3_R1 ] = -yAxis.Dot ( mLoc );
	
	mtx.m [ USMatrix3D::C0_R2 ] = zAxis.mX;
	mtx.m [ USMatrix3D::C1_R2 ] = zAxis.mY;
	mtx.m [ USMatrix3D::C2_R2 ] = zAxis.mZ;
	mtx.m [ USMatrix3D::C3_R2 ] = -zAxis.Dot ( mLoc );
	
	mtx.m [ USMatrix3D::C0_R3 ] = 0.0f;
	mtx.m [ USMatrix3D::C1_R3 ] = 0.0f;
	mtx.m [ USMatrix3D::C2_R3 ] = 0.0f;
	mtx.m [ USMatrix3D::C3_R3 ] = 1.0f;
}

//----------------------------------------------------------------//
void USCamera3D::GetPerspMtx ( USViewport& viewport, USMatrix3D& mtx ) {

	float invAspect = viewport.GetInvAspect ();

	// yScale = cot(fovY/2)
	float xScale = Cot ( this->mHFOV / 2.0f );
	
	// xScale = yScale / aspect ratio
	float yScale = xScale / invAspect;

	// yScale = cot(fovY/2)
	//float yScale = Cot ( this->mVFOV / 2.0f );
	
	// xScale = yScale / aspect ratio
	//float xScale = yScale / this->mAspectWH;
	
	// xScale     0          0              0
	// 0        yScale       0              0
	// 0        0        zf/(zn-zf)        -1
	// 0        0        zn*zf/(zn-zf)      0
	
	float zn = this->mNear;
	float zf = this->mFar;
	
	mtx.m [ USMatrix3D::C0_R0 ] = xScale;
	mtx.m [ USMatrix3D::C1_R0 ] = 0.0f;
	mtx.m [ USMatrix3D::C2_R0 ] = 0.0f;
	mtx.m [ USMatrix3D::C3_R0 ] = 0.0f;

	mtx.m [ USMatrix3D::C0_R1 ] = 0.0f;
	mtx.m [ USMatrix3D::C1_R1 ] = yScale;
	mtx.m [ USMatrix3D::C2_R1 ] = 0.0f;
	mtx.m [ USMatrix3D::C3_R1 ] = 0.0f;
	
	mtx.m [ USMatrix3D::C0_R2 ] = 0.0f;
	mtx.m [ USMatrix3D::C1_R2 ] = 0.0f;
	mtx.m [ USMatrix3D::C2_R2 ] = zf / ( zn - zf );
	mtx.m [ USMatrix3D::C3_R2 ] = ( zn * zf ) / ( zn - zf );
	
	mtx.m [ USMatrix3D::C0_R3 ] = 0.0f;
	mtx.m [ USMatrix3D::C1_R3 ] = 0.0f;
	mtx.m [ USMatrix3D::C2_R3 ] = -1.0f;
	mtx.m [ USMatrix3D::C3_R3 ] = 0.0f;
}

//----------------------------------------------------------------//
void USCamera3D::GetViewProjMtx ( USViewport& viewport, USMatrix3D& mtx ) {

	GetLookAtMtx ( mtx );
	
	USMatrix3D projMtx;
	GetPerspMtx ( viewport, projMtx );

	mtx.Append ( projMtx );
}

//----------------------------------------------------------------//
void USCamera3D::Init ( float hFOV, float nearClip, float farClip ) {

	this->mLoc.Init ( 0.0f, 0.0f, 0.0f );
	this->mLook.Init ( 0.0f, 0.0f, 1.0f );
	this->mUp.Init ( 0.0f, 1.0f, 0.0f );
	
	//this->mAspectHW = ( float )height / ( float )width;
	//this->mAspectWH = ( float )width / ( float )height;
	
	//this->mViewWidth = width;
	//this->mViewHeight = height;

	this->mNear = nearClip;
	this->mFar = farClip;

	this->mHFOV = hFOV * ( float )D2R;
	//mVFOV = ATan ( Tan ( mHFOV * 0.5f ) * ( this->mAspectHW )) * 2.0f;
}

//----------------------------------------------------------------//
void USCamera3D::LocalMove ( USVec3D& move ) {

	USVec3D zAxis = mLook;
	zAxis.Sub ( mLoc );

	zAxis.Project ( mUp );
	zAxis.Norm ();
	
	USVec3D xAxis = zAxis;
	xAxis.Cross ( mUp );
	
	xAxis.Scale ( move.mX );
	zAxis.Scale ( move.mZ );
	
	move = xAxis;
	move.Add ( zAxis );
}

//----------------------------------------------------------------//
/*
void USCamera3D::SetAspect ( float width, float height ) {

	this->mAspectHW = ( float )height / ( float )width;
	this->mAspectWH = ( float )width / ( float )height;
	
	mVFOV = ATan ( Tan ( mHFOV * 0.5f ) * ( this->mAspectHW)) * 2.0f;
	
	this->mViewWidth = width;
	this->mViewHeight = height;
}
*/

//----------------------------------------------------------------//
void USCamera3D::SetFieldOfView ( float hfovRad ) {

	mHFOV = hfovRad;
	//mVFOV = ATan ( Tan ( mHFOV * 0.5f ) * ( this->mAspectHW)) * 2.0f;
}

//----------------------------------------------------------------//
void USCamera3D::SetLoc ( float x, float y, float z ) {

	this->mLoc.Init ( x, y, z );
}

//----------------------------------------------------------------//
void USCamera3D::SetTarget ( float x, float y, float z ) {

	this->mLook.Init ( x, y, z );
}

//----------------------------------------------------------------//
/*
void USCamera3D::Update () {

	this->mViewNorm = this->mLook;
	this->mViewNorm.Sub ( this->mLoc );
	this->mViewNorm.NormSafe ();
	
	this->mInvViewNorm = this->mViewNorm;
	this->mInvViewNorm.Invert ();
}
*/

//----------------------------------------------------------------//
USCamera3D::USCamera3D () :
	mNear ( 0.25f ),
	mFar ( 500.0f ),
	mLoc ( 0.0f, 0.0f, 0.0f ),
	mLook ( 0.0f, 0.0f, 1.0f ),
	mUp ( 0.0f, 1.0f, 0.0f ) {
	
	this->SetFieldOfView ( 60.0f * ( float )D2R );
	//this->SetAspect ( 640.0f, 480.0f );
};

//----------------------------------------------------------------//
USCamera3D::~USCamera3D () {
}
