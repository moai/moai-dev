// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextStyle;
class MOAITextStyler;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesignerParams
//================================================================//
//class MOAITextDesignerParams {
//private:

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
private:
	
	//----------------------------------------------------------------//
	// layout state
	
	MOAITextStyleSpan*	mStyleSpan;
	MOAITextStyle*		mStyle;
	u32					mSpanIdx;
	
	int					mIdx;
	int					mPrevIdx;
	
	cc8*				mStr;
	
	MOAIGlyphSet*		mDeck;
	float				mDeckScale;
	
	int					mLineIdx;
	u32					mLineSpriteID;
	u32					mLineSize;
	float				mLineAscent;
	ZLRect				mLineRect;
	
	int					mTokenIdx;
	u32					mTokenSpriteID;
	u32					mTokenSize;
	float				mTokenAscent;
	ZLRect				mTokenRect;
	
	ZLVec2D				mPen;
	MOAIGlyph*			mPrevGlyph;
	bool				mMore;
	
	//----------------------------------------------------------------//
	// layout settings
	
	ZLVec2D				mLoc;
	
	float				mWidth;
	float				mHeight;
	
	bool				mLimitWidth;
	bool				mLimitHeight;
	
	u32					mHAlign;
	u32					mVAlign;
	
	u32					mWordBreak;
	
	float				mGlyphScale;
	float				mLineSpacing;
	
	u32					mTotalCurves;
	MOAIAnimCurve**		mCurves;
	
	MOAITextLayout*		mLayout;
	MOAITextStyler*		mStyler;
	
	//----------------------------------------------------------------//
	void				AcceptLine				();
	void				AcceptToken				();
	void				Align					();
	u32					NextChar				();

public:

	enum {
		LEFT_JUSTIFY,
		CENTER_JUSTIFY,
		RIGHT_JUSTIFY,
	};

	enum {
		WORD_BREAK_NONE,
		WORD_BREAK_CHAR,
	};

	GET_SET ( ZLVec2D&, Loc, mLoc )
	
	GET_SET ( float, Width, mWidth )
	GET_SET ( float, Height, mHeight )
	
	GET_SET ( bool, LimitWidth, mLimitWidth )
	GET_SET ( bool, LimitHeight, mLimitHeight )
	
	GET_SET ( u32, HAlign, mHAlign )
	GET_SET ( u32, VAlign, mVAlign )
	
	GET_SET ( u32, WordBreak, mWordBreak )
	GET_SET ( float, GlyphScale, mGlyphScale )
	GET_SET ( float, LineSpacing, mLineSpacing )
	
	GET ( u32, Index, mIdx )
	GET ( u32, More, mMore )

	//----------------------------------------------------------------//
	void			BuildLayout				();
	void			Init					( MOAITextLayout& layout, MOAITextStyler& styler, cc8* str, u32 idx );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
	bool			More					();
	void			SetCurves				( MOAIAnimCurve** curves, u32 totalCurves );
};

#endif
