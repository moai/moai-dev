// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-vr/MOAIVrMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_WITH_LIBGVR

// TODO: should come from MOAISim
static const uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

//================================================================//
// MOAIVrMgr
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIVrMgr::GvrMatrixToZLMatrix ( const gvr_mat4f& gvrMtx ) {

	ZLMatrix4x4 mtx;
	memcpy ( mtx.m, gvrMtx.m, sizeof ( float ) * 16 );
	return mtx;
}

//----------------------------------------------------------------//
MOAIVrMgr::MOAIVrMgr () {

	this->mGVRContext = gvr_create ();
	this->mHeadTransform.Ident ();
}

//----------------------------------------------------------------//
MOAIVrMgr::~MOAIVrMgr () {

	gvr_destroy ( &this->mGVRContext );
}

//----------------------------------------------------------------//
void MOAIVrMgr::Update () {

	gvr_clock_time_point targetTime = gvr_get_time_point_now ();
	targetTime.monotonic_system_time_nanos += kPredictionTimeWithoutVsyncNanos;

	gvr_mat4f headView = gvr_get_head_space_from_start_space_rotation ( this->mGVRContext, targetTime );
	
	this->mHeadTransform.RotateZ ( D2R * 90.0f );
	this->mHeadTransform.Append ( MOAIVrMgr::GvrMatrixToZLMatrix ( headView ));
	
	
	//this->mHeadTransform.Inverse ();
	
	//gvr::Mat4f left_eye_view_pose = MatrixMul(gvr_api_->GetEyeFromHeadMatrix(GVR_LEFT_EYE), head_view_);
	//gvr::Mat4f right_eye_view_pose = MatrixMul(gvr_api_->GetEyeFromHeadMatrix(GVR_RIGHT_EYE),head_view_);
}

#endif
