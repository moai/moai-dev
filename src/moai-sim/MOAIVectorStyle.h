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
	u32					mJoinStyle;
	u32					mCapStyle;
	float				mMiterLimit;
	
	u32					mWindingRule;
	u32					mCircleResolution;
	
	GET_SET ( u32, FillStyle, mFillStyle )
	GET_SET ( ZLColorVec&, FillColor, mFillColor )
	
	GET_SET ( u32, LineStyle, mLineStyle )
	GET_SET ( ZLColorVec&, LineColor, mLineColor )
	GET_SET ( float, LineWidth, mLineWidth )
	
	GET_SET ( u32, StrokeStyle, mStrokeStyle )
	GET_SET ( ZLColorVec&, StrokeColor, mStrokeColor )
	GET_SET ( float, StrokeWidth, mStrokeWidth )
	GET_SET ( u32, JoinStyle, mJoinStyle )
	GET_SET ( u32, CapStyle, mCapStyle )
	GET_SET ( float, MiterLimit, mMiterLimit )
	
	GET_SET ( u32, WindingRule, mWindingRule )
	GET_SET ( u32, CircleResolution, mCircleResolution )
};

#endif
