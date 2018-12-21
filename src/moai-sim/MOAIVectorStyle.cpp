// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorStyle.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorStyle
//================================================================//
	
//----------------------------------------------------------------//
void MOAIVectorStyle::Default () {

	this->SetFillStyle ( MOAIVectorStyle::FILL_SOLID );
	this->mFillColor.SetWhite ();
	
	this->SetLineStyle ( MOAIVectorStyle::LINE_NONE );
	this->mLineColor.SetWhite ();
	this->SetLineWidth ( 1.0 );
	
	this->SetStrokeStyle ( MOAIVectorStyle::STROKE_NONE );
	this->mStrokeColor.SetWhite ();
	this->SetStrokeWidth ( 1.0 );
	this->SetStrokeDepthBias ( 0.0 );
	this->SetJoinStyle ( MOAIVectorStyle::JOIN_MITER );
	this->SetCapStyle ( MOAIVectorStyle::CAP_BUTT );
	this->SetMiterLimit ( 5.0 );
	
	this->SetWindingRule (( u32 )TESS_WINDING_ODD );
	this->SetCircleResolution ( MOAIVectorStyle::DEFAULT_CIRCLE_RESOLUTION );
	
	this->mExtrude = 0.0;
	this->mZOffset = 0.0;
	
	this->mLightVec = ZLVec3D::ORIGIN;
	
	this->mLightColor.SetWhite ();
	this->mLightCurve = ZLInterpolate::kLinear;
	
	this->mShadowColor.SetBlack ();
	this->mShadowCurve = ZLInterpolate::kLinear;
	
	this->mDrawingToWorld.Ident ();
	this->mWorldToDrawing.Ident ();
	
	this->mFillExtraID = 0;
	this->mStrokeExtraID = 0;
	
	this->mMergeNormals = 0.0;
	
	this->mPrecision = 100;
}

//----------------------------------------------------------------//
float MOAIVectorStyle::GetMergeNormalsCosine () const {

	return Cos (( float )( this->mMergeNormals * D2R ));
}

//----------------------------------------------------------------//
u32 MOAIVectorStyle::GetResolutionForWedge ( float radians ) const {

	u32 resolution = ( u32 )( this->mCircleResolution * ( radians / ( float )TWOPI ));
	return resolution < 1 ? 1 : resolution;
}
