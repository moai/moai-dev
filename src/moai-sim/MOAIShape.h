// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHAPE_H
#define	MOAISHAPE_H

//================================================================//
// MOAIShape
//================================================================//
class MOAIShape {
protected:

	ZLColorVec		mFillColor;
	ZLColorVec		mLineColor;
	
	u32				mFillStyle;
	u32				mLineStyle;

public:

	enum {
		LINE_NONE,
		LINE_VECTOR,
	};
	
	enum {
		FILL_NONE,
		FILL_SOLID,
	};
	
	GET_SET ( ZLColorVec&, FillColor, mFillColor )
	GET_SET ( ZLColorVec&, LineColor, mLineColor )
	
	GET_SET ( u32, FillStyle, mFillStyle )
	GET_SET ( u32, LineStyle, mLineStyle )
	
	//----------------------------------------------------------------//
						MOAIShape				();
	virtual				~MOAIShape				();
	virtual void		Render					() = 0;
};

#endif
