// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <math.h>
#include <uslsext/USCamera2D.h>
#include <uslsext/USCameraFitter.h>
#include <uslsext/USCanvas.h>

//================================================================//
// USCameraFitter
//================================================================//

//----------------------------------------------------------------//
void USCameraFitter::AddPin ( USVec2D& worldLoc, float radius ) {

	this->AddWorldLoc ( worldLoc );
	
	USFitRect fitRect;
	fitRect.mWorldLoc = worldLoc;
	fitRect.mScreenRect.Init ( -radius, -radius, radius, radius );
	
	this->mFitRects.push_back ( fitRect );
}

//----------------------------------------------------------------//
void USCameraFitter::AddPin ( USVec2D& worldLoc, USRect& screenRect ) {

	this->AddWorldLoc ( worldLoc );
	
	USFitRect fitRect;
	fitRect.mWorldLoc = worldLoc;
	fitRect.mScreenRect = screenRect;
	
	this->mFitRects.push_back ( fitRect );
}

//----------------------------------------------------------------//
void USCameraFitter::AddWorldLoc ( USVec2D& worldLoc ) {

	if ( this->mIsReady ) {
		this->mWorldRect.Grow ( worldLoc );
	}
	else {
		this->mWorldRect.Init ( worldLoc );
	}

	this->mIsReady = true;
}

//----------------------------------------------------------------//
void USCameraFitter::AddWorldRect ( USRect& worldRect ) {

	if ( this->mIsReady ) {
		this->mWorldRect.Grow ( worldRect );
	}
	else {
		this->mWorldRect = worldRect;
	}

	this->mIsReady = true;
}

//----------------------------------------------------------------//
USCameraFitter::USCameraFitter () :
	mIsReady ( false ) {
}

//----------------------------------------------------------------//
USCameraFitter::~USCameraFitter () {
}

//----------------------------------------------------------------//
void USCameraFitter::Center ( USCamera2D& camera, USViewport& viewport ) {

	this->mTargetLoc = camera.mLoc;
	this->mTargetScale = camera.mScale;

	if ( !this->mIsReady ) return;
	
	USCamera2D fitCam = camera;
	
	USAffine2D worldToWndMtx;
	USCanvas::GetWorldToWndMtx ( viewport, fitCam, worldToWndMtx );
	
	USAffine2D wndToWorldMtx;
	USCanvas::GetWndToWorldMtx ( viewport, fitCam, wndToWorldMtx );
	
	USRect worldRect = this->mWorldRect;
	
	if ( this->mFitRects.size ()) {
	
		bool initFrame = true;
		USRect frameRect;
		
		FitRectIt fitRectIt = this->mFitRects.begin ();
		for ( ; fitRectIt != this->mFitRects.end (); ++fitRectIt ) {
			
			USFitRect& fitRect = *fitRectIt;
			
			USVec2D loc = fitRect.mWorldLoc;
			worldToWndMtx.Transform ( loc );
			
			USRect rect = fitRect.mScreenRect;
			rect.Offset ( loc.mX, loc.mY );
			
			if ( initFrame ) {
				frameRect = rect;
				initFrame = false;
			}
			else {
				frameRect.Grow ( rect );
			}
		}
		
		wndToWorldMtx.Transform ( frameRect );
		frameRect.Bless ();
		worldRect.Grow ( frameRect );
	}
	
	worldRect.GetCenter ( this->mTargetLoc );
}

//----------------------------------------------------------------//
void USCameraFitter::Fit ( USCamera2D& camera, USViewport& viewport, USRect& viewRect, u32 steps ) {

	this->mTargetLoc = camera.mLoc;
	this->mTargetScale = camera.mScale;

	if ( !this->mIsReady ) return;
	
	USCamera2D fitCam = camera;
	
	for ( u32 i = 0; i < steps; ++i ) {
	
		USAffine2D worldToWndMtx;
		USCanvas::GetWorldToWndMtx ( viewport, fitCam, worldToWndMtx );
		
		USAffine2D wndToWorldMtx;
		USCanvas::GetWndToWorldMtx ( viewport, fitCam, wndToWorldMtx );
		
		USRect worldRect = this->mWorldRect;
		
		if ( this->mFitRects.size ()) {
		
			bool initFrame = true;
			USRect frameRect;
			
			FitRectIt fitRectIt = this->mFitRects.begin ();
			for ( ; fitRectIt != this->mFitRects.end (); ++fitRectIt ) {
				
				USFitRect& fitRect = *fitRectIt;
				
				USVec2D loc = fitRect.mWorldLoc;
				worldToWndMtx.Transform ( loc );
				
				USRect rect = fitRect.mScreenRect;
				rect.Offset ( loc.mX, loc.mY );
				
				if ( initFrame ) {
					frameRect = rect;
					initFrame = false;
				}
				else {
					frameRect.Grow ( rect );
				}
			}
			
			wndToWorldMtx.Transform ( frameRect );
			frameRect.Bless ();
			worldRect.Grow ( frameRect );
		}
		
		this->SetTargetFromRect ( fitCam, viewport, viewRect, worldRect );
	}
	
	this->mTargetLoc = fitCam.mLoc;
	this->mTargetScale = fitCam.mScale;
}

//----------------------------------------------------------------//
void USCameraFitter::SetTargetFromRect ( USCamera2D& camera, USViewport& viewport, USRect fittingRect, USRect rect ) {

	// figure the scaled view rect
	USRect viewRect = viewport.GetRect ();
	
	fittingRect.ClipTo ( viewRect );
	
	fittingRect.TransformWorldToLocal ( viewRect );
	//rect.FitAspectRect ( fittingRect );
	
	float fitAspect = fittingRect.Aspect ();
	
	float fitWidth;
	float fitHeight;
	
	if ( rect.Aspect () >= fitAspect ) {
		// rect is fatter
		fitWidth = rect.Width () * 0.5f;
		fitHeight = ( fitWidth / fitAspect );
		
	}
	else {
		// rect is thinner
		fitHeight = rect.Height () * 0.5f;
		fitWidth = ( fitHeight * fitAspect );
	}
	
	USVec2D rectCenter;
	rect.GetCenter ( rectCenter );
	
	fittingRect.Init (
		rectCenter.mX - fitWidth,
		rectCenter.mY - fitHeight,
		rectCenter.mX + fitWidth,
		rectCenter.mY + fitHeight
	);
	
	// compensate for y flip
	viewRect.mYMin = 1.0f - viewRect.mYMin;
	viewRect.mYMax = 1.0f - viewRect.mYMax;
	viewRect.Bless ();
	
	fittingRect.TransformLocalToWorld ( viewRect );
	
	viewRect.GetCenter ( camera.mLoc );
	
	float s0 = 1.0f / viewRect.Width ();
	float s1 = 1.0f / viewRect.Height ();
	
	camera.mScale = ( s0 < s1 ) ? s0 : s1;
}

//----------------------------------------------------------------//
void USCameraFitter::Reset () {

	this->mFitRects.clear ();
	this->mIsReady = false;
}
