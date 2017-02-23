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

	ZLAffine3D& mtx = this->mTransform;

	mtx.m [ ZLAffine3D::C0_R0 ] = this->mPatternTrans [ 0 ][ 0 ]; // R1C1
    mtx.m [ ZLAffine3D::C1_R0 ] = this->mPatternTrans [ 0 ][ 1 ]; // R1C2
    mtx.m [ ZLAffine3D::C2_R0 ] = this->mPatternTrans [ 0 ][ 2 ];
    mtx.m [ ZLAffine3D::C3_R0 ] = this->mPatternTrans [ 0 ][ 3 ];
	
    mtx.m [ ZLAffine3D::C0_R1 ] = -this->mPatternTrans [ 1 ][ 0 ]; // R2
    mtx.m [ ZLAffine3D::C1_R1 ] = -this->mPatternTrans [ 1 ][ 1 ];
    mtx.m [ ZLAffine3D::C2_R1 ] = -this->mPatternTrans [ 1 ][ 2 ];
    mtx.m [ ZLAffine3D::C3_R1 ] = -this->mPatternTrans [ 1 ][ 3 ];
	
	mtx.m [ ZLAffine3D::C0_R2 ] = -this->mPatternTrans [ 2 ][ 0 ]; // R3
    mtx.m [ ZLAffine3D::C1_R2 ] = -this->mPatternTrans [ 2 ][ 1 ];
    mtx.m [ ZLAffine3D::C2_R2 ] = -this->mPatternTrans [ 2 ][ 2 ];
    mtx.m [ ZLAffine3D::C3_R2 ] = -this->mPatternTrans [ 2 ][ 3 ];
	
//    if (scale != 0.0f) {
//        m_modelview[12] *= scale;
//        m_modelview[13] *= scale;
//        m_modelview[14] *= scale;
//    }
}

#endif
