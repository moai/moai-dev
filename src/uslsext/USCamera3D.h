// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef CAMERA_H
#define CAMERA_H

#include <uslsext/USFrustum.h>
#include <uslsext/USMatrix2D.h>
#include <uslsext/USViewport.h>

//================================================================//
// USCamera3D
//================================================================//
class USCamera3D {
public:

	float		mNear;
	float		mFar;

	USVec3D		mLoc;		// The location of the camera
	USVec3D		mLook;		// What the camera is looking at
	USVec3D		mUp;		// The up vector for the camera

	float		mHFOV;		// The horizontal field of view
	
	//USVec3D	mViewNorm;
	//USVec3D	mInvViewNorm;
	
	//----------------------------------------------------------------//
	void		BuildFrustum		( USViewport& viewport, USFrustum& frustum );
	void		GetLookAtMtx		( USMatrix3D& mtx );
	void		GetPerspMtx			( USViewport& viewport, USMatrix3D& mtx );
	void		GetViewProjMtx		( USViewport& viewport, USMatrix3D& mtx );
	void		Init				( float hFOV, float nearClip, float farClip );
	void		LocalMove			( USVec3D& move ); // Normalized joystick vec to camera relative vec
	void		SetFieldOfView		( float hfovRad );
	void		SetLoc				( float x, float y, float z );
	void		SetTarget			( float x, float y, float z );
				USCamera3D			();
				~USCamera3D			();
};

#endif
