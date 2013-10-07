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
		FILL_NONE,
		FILL_SOLID,
	};

	ZLColorVec			mFillColor;
	ZLColorVec			mLineColor;
	
	u32					mFillStyle;
	u32					mLineStyle;
	
	float				mLineWidth;
	float				mLineOffset;
	
	u32					mWindingRule;
	
	
	GET_SET ( ZLColorVec&, FillColor, mFillColor )
	GET_SET ( ZLColorVec&, LineColor, mLineColor )
	
	GET_SET ( u32, FillStyle, mFillStyle )
	GET_SET ( u32, LineStyle, mLineStyle )
	
	GET_SET ( float, LineWidth, mLineWidth )
	GET_SET ( float, LineOffset, mLineOffset )
	
	GET_SET ( u32, WindingRule, mWindingRule )
};

#endif
