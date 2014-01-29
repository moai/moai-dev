// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	this->SetLineWidth ( 1.0f );
	
	this->SetStrokeStyle ( MOAIVectorStyle::STROKE_NONE );
	this->mStrokeColor.SetWhite ();
	this->SetStrokeWidth ( 1.0f );
	this->SetStrokeDepthBias ( 0.0f );
	this->SetJoinStyle ( MOAIVectorStyle::JOIN_MITER );
	this->SetCapStyle ( MOAIVectorStyle::CAP_BUTT );
	this->SetMiterLimit ( 5.0f );
	
	this->SetWindingRule (( u32 )TESS_WINDING_ODD );
	this->SetCircleResolution ( MOAIVectorStyle::DEFAULT_CIRCLE_RESOLUTION );
	
	this->mExtrude = 0.0f;
	
	this->mLightVec.Init ( 0.0f, 0.0f, 0.0f );
	
	this->mLightColor.SetWhite ();
	this->mLightCurve = ZLInterpolate::kLinear;
	
	this->mShadowColor.SetBlack ();
	this->mShadowCurve = ZLInterpolate::kLinear;
	
	this->mDrawingToWorld.Ident ();
	this->mWorldToDrawing.Ident ();
	
	this->mFillExtraID = 0;
	this->mStrokeExtraID = 0;
}

//----------------------------------------------------------------//
u32 MOAIVectorStyle::GetResolutionForWedge ( float radians ) const {

	u32 resolution = ( u32 )( this->mCircleResolution * ( radians / ( float )TWOPI ));
	return resolution < 1 ? 1 : resolution;
}
