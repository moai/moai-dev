// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextLayout;
class MOAITextStyle;
class MOAITextStyler;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
protected:

	friend class MOAITextDesignParser;

	MOAINode*	mOwner;

	ZLRect		mFrame;
	
	bool		mLimitWidth;
	bool		mLimitHeight;
	
	u32			mHAlign;
	u32			mVAlign;
	bool		mYFlip;
	
	u32			mWordBreak; // rule for breaking words across lines
	
	float		mGlyphScale;
	float		mLineSpacing;
	
	ZLLeanArray < MOAIAnimCurve* > mCurves;

	//----------------------------------------------------------------//
	void		ReleaseCurve				( MOAIAnimCurve* curve  );
	void		RetainCurve					( MOAIAnimCurve* curve  );

public:

	enum {
		BASELINE_JUSTIFY,
		BOTTOM_JUSTIFY,
		LEFT_JUSTIFY,
		CENTER_JUSTIFY,
		RIGHT_JUSTIFY,
		TOP_JUSTIFY,
	};

	enum {
		WORD_BREAK_NONE,
		WORD_BREAK_CHAR,
	};

	GET_SET ( MOAINode*, Owner, mOwner )
	
	GET_SET ( ZLRect&, Frame, mFrame )
	
	GET_SET ( bool, LimitWidth, mLimitWidth )
	GET_SET ( bool, LimitHeight, mLimitHeight )
	
	GET_SET ( u32, HAlign, mHAlign )
	GET_SET ( u32, VAlign, mVAlign )
	GET_SET ( bool, YFlip, mYFlip )
	
	GET_SET ( u32, WordBreak, mWordBreak )
	GET_SET ( float, GlyphScale, mGlyphScale )
	GET_SET ( float, LineSpacing, mLineSpacing )

	//----------------------------------------------------------------//
	void		ClearCurves					();
	void		Layout						( MOAITextLayout& layout, MOAITextStyler& styler, cc8* str, u32 idx, bool* more, u32* nextIdx );
				MOAITextDesigner			();
				~MOAITextDesigner			();
	void		ReserveCurves				( u32 total );
	void		SetCurve					( u32 idx, MOAIAnimCurve* curve );
};

#endif
