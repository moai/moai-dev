// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMARKER_H
#define MOAIMARKER_H

#if MOAI_WITH_ARTOOLKIT

#import <AR/ar.h>
#import <AR/video.h>
#import <AR/sys/CameraVideo.h>

class MOAIPattern;
class MOAIVideoTexture;

//================================================================//
// MOAIMarker
//================================================================//
class MOAIMarker {
private:

	friend class MOAIMarkerMgr;

	u32				mMarkerID;
	ZLAffine3D		mTransform;
	MOAIPattern*	mPattern;
	ZLVec2D			mPosition;
	ARdouble		mPatternTrans [ 3 ][ 4 ]; 

	ZLLeanLink < MOAIMarker* >	mLink;
	MOAIMarker*					mNext;

	//----------------------------------------------------------------//
	void		Reset					();
	void		Update					();

public:

	//----------------------------------------------------------------//
				MOAIMarker				( u32 idx );
				~MOAIMarker				();
};

#endif
#endif
