// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USCAMERAFITTER_H
#define USCAMERAFITTER_H

//================================================================//
// USFitRect
//================================================================//
class USFitRect {
public:

	USVec2D	mWorldLoc;
	USRect	mScreenRect;
};

//================================================================//
// USCameraFitter
//================================================================//
class USCameraFitter {
private:

	bool mIsReady;

	USRect mWorldRect;

	typedef STLList < USFitRect >::iterator FitRectIt;
	STLList < USFitRect > mFitRects;

	//----------------------------------------------------------------//
	void		SetTargetFromRect	( USCamera2D& camera, USViewport& viewport, USRect fittingRect, USRect rect );

public:

	USVec2D		mTargetLoc;
	float		mTargetScale;

	//----------------------------------------------------------------//
	void		AddPin				( USVec2D& worldLoc, float radius );
	void		AddPin				( USVec2D& worldLoc, USRect& screenRect );
	void		AddWorldLoc			( USVec2D& worldLoc );
	void		AddWorldRect		( USRect& worldRect );
	void		Center				( USCamera2D& camera, USViewport& viewport );
	void		Fit					( USCamera2D& camera, USViewport& viewport, USRect& viewRect, u32 steps );
	void		Reset				();
				USCameraFitter		();
				~USCameraFitter		();
};

#endif
