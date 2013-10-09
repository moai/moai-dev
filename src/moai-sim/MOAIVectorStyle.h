// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSTYLE_H
#define	MOAIVECTORSTYLE_H

//================================================================//
// MOAIVectorStyle
//================================================================//
class MOAIVectorStyle {
public:

	enum {
		LINE_NONE,
		LINE_STROKE,
		LINE_VECTOR,
	};
	
	enum {
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
		CAP_SQUARE,
		CAP_ROUND,
		CAP_BUTT,
	};

	ZLColorVec			mFillColor;
	ZLColorVec			mLineColor;
	
	u32					mFillStyle;
	u32					mLineStyle;
	u32					mStrokeStyle;
	
	u32					mJoinStyle;
	u32					mCapStyle;
	
	float				mLineWidth;
	float				mMiterLimit;
	
	u32					mWindingRule;
	
	
	GET_SET ( ZLColorVec&, FillColor, mFillColor )
	GET_SET ( ZLColorVec&, LineColor, mLineColor )
	
	GET_SET ( u32, FillStyle, mFillStyle )
	GET_SET ( u32, LineStyle, mLineStyle )
	GET_SET ( u32, StrokeStyle, mStrokeStyle )
	
	GET_SET ( u32, JoinStyle, mJoinStyle )
	GET_SET ( u32, CapStyle, mCapStyle )
	
	GET_SET ( float, LineWidth, mLineWidth )
	GET_SET ( float, MiterLimit, mMiterLimit )
	
	GET_SET ( u32, WindingRule, mWindingRule )
};

#endif
