// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSTYLE_H
#define	MOAIVECTORSTYLE_H

//================================================================//
// MOAIVectorStyle
//================================================================//
class MOAIVectorStyle {
public:

	static const u32 DEFAULT_CIRCLE_RESOLUTION = 32;

	enum {
		LINE_NONE,
		LINE_VECTOR,
	};
	
	enum {
		STROKE_NONE,
		STROKE_CENTER,
		STROKE_INTERIOR,
		STROKE_EXTERIOR,
	};
	
	enum {
		FILL_NONE,
		FILL_SOLID,
	};

	enum {
		JOIN_BEVEL,
		JOIN_MITER,
		JOIN_ROUND,
	};
	
	enum {
		CAP_BUTT,
		CAP_POINTY,
		CAP_ROUND,
		CAP_SQUARE,
	};

	u32					mFillStyle;
	ZLColorVec			mFillColor;
	
	u32					mLineStyle;
	ZLColorVec			mLineColor;
	float				mLineWidth;
	
	u32					mStrokeStyle;
	ZLColorVec			mStrokeColor;
	float				mStrokeWidth;
	float				mStrokeDepthBias;
	u32					mJoinStyle;
	u32					mCapStyle;
	float				mMiterLimit;
	
	u32					mWindingRule;
	u32					mCircleResolution;
	
	float				mExtrude;
	
	ZLVec3D				mLightVec;
	
	ZLColorVec			mLightColor;
	u32					mLightCurve;
	
	ZLColorVec			mShadowColor;
	u32					mShadowCurve;
	
	ZLAffine2D			mDrawingToWorld;
	ZLAffine2D			mWorldToDrawing;
	
	u32					mFillExtraID;
	u32					mStrokeExtraID;
	
	GET_SET_CONST ( u32, FillStyle, mFillStyle )
	GET_SET_CONST ( ZLColorVec&, FillColor, mFillColor )
	
	GET_SET_CONST ( u32, LineStyle, mLineStyle )
	GET_SET_CONST ( ZLColorVec&, LineColor, mLineColor )
	GET_SET_CONST ( float, LineWidth, mLineWidth )
	
	GET_SET_CONST ( u32, StrokeStyle, mStrokeStyle )
	GET_SET_CONST ( ZLColorVec&, StrokeColor, mStrokeColor )
	GET_SET_CONST ( float, StrokeWidth, mStrokeWidth )
	GET_SET_CONST ( float, StrokeDepthBias, mStrokeDepthBias )
	GET_SET_CONST ( u32, JoinStyle, mJoinStyle )
	GET_SET_CONST ( u32, CapStyle, mCapStyle )
	GET_SET_CONST ( float, MiterLimit, mMiterLimit )
	
	GET_SET_CONST ( u32, WindingRule, mWindingRule )
	GET_SET_CONST ( u32, CircleResolution, mCircleResolution )
	
	GET_SET_CONST ( float, Extrude, mExtrude )
	
	GET_SET_CONST ( ZLVec3D&, LightVec, mLightVec )
	
	GET_SET_CONST ( ZLColorVec&, LightColor, mLightColor )
	GET_SET_CONST ( u32, LightCurve, mLightCurve );
	
	GET_SET_CONST ( ZLColorVec&, ShadowColor, mShadowColor )
	GET_SET_CONST ( u32, ShadowCurve, mShadowCurve )
	
	GET_SET_CONST ( ZLAffine2D&, DrawingToWorld, mDrawingToWorld )
	GET_SET_CONST ( ZLAffine2D&, WorldToDrawing, mWorldToDrawing )
	
	GET_SET_CONST ( u32, FillExtraID, mFillExtraID )
	GET_SET_CONST ( u32, StrokeExtraID, mStrokeExtraID )
	
	//----------------------------------------------------------------//
	void		Default						();
	u32			GetResolutionForWedge		( float radians ) const;
};

#endif
