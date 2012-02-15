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
	
	//u32					mLineBottom;
	//u32					mLineTop;
	//
	//u32					mLineCount;
	//u32					mTotalLines;
	//
	//float				mLineXMax;
	//
	//USVec2D				mPen;
	//cc8*				mStr;
	//USRect				mFrame;
	//float				mLineSpacing;
	//u32					mJustify;
	//MOAITextLayout*		mLayout;
	//
	//MOAIAnimCurve**		mCurves;
	//u32					mTotalCurves;
	//bool				mRightToLeft;

	//----------------------------------------------------------------//
	//u32			DecodeChar			();
	//void			FinishChar			( u32 c );
	//void			FinishLine			();
	//void			FinishWord			();
	//void			Flush				();
	//void			FlushLine			();
	//void			FlushToken			();
	static bool		IsWhitespace		( u32 c );
	u32				NextChar			();
	//void			Parse				();
	void			UpdateStyle			();

public:

	//SET ( cc8*, Text, mStr )
	//SET ( MOAIFont*, Font, mFont )
	//SET ( const USRect&, Frame, mFrame )
	//SET ( float, LineSpacing, mLineSpacing )
	//SET ( float, Points, mPoints )
	//SET ( u32, Alignment, mJustify )
	//SET ( bool, RightToLeft, mRightToLeft )

	//----------------------------------------------------------------//
	void			Layout					( cc8* str, MOAITextStyleMap& styleMap, MOAITextLayout& layout );
	//void			SetCurves				( MOAIAnimCurve** curves, u32 totalCurves );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
