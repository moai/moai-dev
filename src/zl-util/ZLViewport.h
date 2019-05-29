// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVIEWPORT_H
#define	ZLVIEWPORT_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLRect.h>
#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLViewport
//================================================================//
class ZLViewport :
	public ZLRect {
private:

	bool		mXScale;
	bool		mYScale;
	
	ZLVec2D		mScale;
	ZLVec2D		mOffset;
	float		mRotation;

public:
	
	GET_SET_CONST ( ZLVec2D&, Offset, mOffset );
	GET_SET_CONST ( float, Rotation, mRotation );
	
	//----------------------------------------------------------------//
	float			GetAspect				() const;
	float			GetInvAspect			() const;
	ZLMatrix4x4		GetNormToWndMtx			() const;
	ZLMatrix4x4		GetProjMtx				() const;
	ZLMatrix4x4		GetProjMtxInv			() const;
	ZLRect			GetRect					() const;
	ZLVec2D			GetScale				() const;
	ZLVec2D			GetUnits				() const;
	ZLMatrix4x4		GetWndToNormMtx			() const;
	void			SetOffset				( float xOffset, float yOffset );
	void			SetScale				( float xScale, float yScale );
					ZLViewport				();
					~ZLViewport				();
};

#endif
