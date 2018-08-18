// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ar/MOAIMarker.h>
#include <moai-ar/MOAIMarkerMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_ARTOOLKIT

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIMarker
//================================================================//

//----------------------------------------------------------------//
MOAIMarker::MOAIMarker ( u32 idx ) :
	mMarkerID ( idx ),
	mPattern ( 0 ),
	mLink ( this ),
	mNext ( 0 ) {
	
	this->mPosition = ZLVec2D::ORIGIN;
}

//----------------------------------------------------------------//
MOAIMarker::~MOAIMarker () {
}

//----------------------------------------------------------------//
void MOAIMarker::Reset () {

	this->mPattern = 0;
}

//----------------------------------------------------------------//
void MOAIMarker::Update () {

	MOAIMarkerMgr::ConvertTransform ( this->mTransform, this->mPatternTrans );
}

#endif
