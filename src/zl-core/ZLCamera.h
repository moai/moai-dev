// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLCAMERA_H
#define	ZLCAMERA_H

#include <zl-core/ZLAccessors.h>
#include <zl-core/ZLMatrix4x4.h>
#include <zl-core/ZLRtti.h>
#include <zl-core/ZLViewport.h>

class ZLViewport;

//================================================================//
// ZLCamera
//================================================================//
class ZLCamera :
	public virtual RTTIBase {
protected:

	ZLMatrix4x4		mViewMtx;
	ZLMatrix4x4		mProjectionMtx;
	bool			mUseProjectionMtx;

	float			mFieldOfView;
	float			mNearPlane;
	float			mFarPlane;

	u32				mType;
	
	//----------------------------------------------------------------//
	
public:
	
	enum {
		CAMERA_TYPE_3D,
		CAMERA_TYPE_ORTHO,
		CAMERA_TYPE_WINDOW,
	};
	
	GET_SET ( float, FieldOfView, mFieldOfView )
	GET_SET ( float, NearPlane, mNearPlane )
	GET_SET ( float, FarPlane, mFarPlane )
	GET_SET ( u32, Type, mType )
	
	GET_SET_CONST ( ZLMatrix4x4, ViewMtx, mViewMtx )
	
	//----------------------------------------------------------------//
	ZLMatrix4x4		GetBillboardMtx			() const;
	float			GetFocalLength			( float width ) const;
	ZLMatrix4x4		GetProjMtx				( const ZLViewport& viewport ) const;
	ZLVec3D			GetViewVector			() const;
	void			SetProjMtx				();
	void			SetProjMtx				( const ZLMatrix4x4& mtx );
					ZLCamera				();
					~ZLCamera				();
};

#endif
