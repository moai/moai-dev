// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextLayout;
class MOAITextStyleMap;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
private:
	
	MOAITextStyleMap*		mStyleMap;
	MOAITextStyleSpan*		mStyleSpan;
	u32						mSpanIdx;
	int						mIdx;
	
	cc8*					mStr;
	
	MOAIGlyphDeck*			mDeck;
	
	//MOAIAnimCurve**		mCurves;
	//u32					mTotalCurves;
	//bool				mRightToLeft;

	//----------------------------------------------------------------//
	u32				NextChar				();

public:

	//----------------------------------------------------------------//
	int				Layout					( cc8* str, int idx, const USRect& frame, u32 hAlign, u32 vAlign, MOAITextStyleMap& styleMap, MOAITextLayout& layout );
	//void			SetCurves				( MOAIAnimCurve** curves, u32 totalCurves );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
