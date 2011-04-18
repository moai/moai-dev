// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USDEBUGLINES_H
#define	USDEBUGLINES_H

#include <uslsext/USAffine2D.h>
#include <uslsext/USGLLine.h>
#include <uslsext/USQuad.h>

//================================================================//
// USDebugLine
//================================================================//
class USDebugLine :
	public USGLLine {
private:

	friend class USDebugLines;

	USVec2D		mVtx [ 2 ];
	u32			mColor;
	float		mWidth;
	
	//----------------------------------------------------------------//
	void	Draw		();
	void	SetVerts	( float x0, float y0, float x1, float y1 );
};
//================================================================//
// USDebugLines
//================================================================//
class USDebugLines {
protected:

	u32								mPenColor;
	u32								mPenWidth;
	u32								mPenSpace;

	u32								mTop;
	USLeanArray < USDebugLine >		mLineBuffer;
	USAffine2D						mModelToWorldMtx;

	//----------------------------------------------------------------//
	USDebugLine*	NextLine				();

public:

	enum {
		MODEL_SPACE,
		WORLD_SPACE,
	};

	GET_SET ( u32, PenColor, mPenColor )
	GET_SET ( u32, PenWidth, mPenWidth )
	GET_SET ( u32, PenSpace, mPenSpace )

	//----------------------------------------------------------------//
	void			Draw					();
	void			DrawEllipse				( USVec2D& loc, float xRad, float yRad, u32 steps );
	void			DrawLine				( const USVec2D& v0, const USVec2D& v1 );
	void			DrawLine				( float x0, float y0, float x1, float y1 );
	void			DrawRect				( const USRect& rect );
	void			DrawQuad				( const USQuad& quad );
	void			Reset					();
	void			SetWorldMtx				();
	void			SetWorldMtx				( const USAffine2D& mtx );
	void			SetPen					( u32 penColor, float penWidth, u32 penSpace = MODEL_SPACE );
					USDebugLines			();
					~USDebugLines			();
};

#endif
