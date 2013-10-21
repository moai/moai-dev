// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextStyle;
class MOAITextStyler;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
private:

	friend class MOAITextDesignParser;

	MOAINode*	mOwner;

	ZLVec2D		mLoc;

	float		mWidth;
	float		mHeight;
	
	bool		mLimitWidth;
	bool		mLimitHeight;
	
	u32			mHAlign;
	u32			mVAlign;
	
	u32			mWordBreak; // rule for breaking words across lines
	
	float		mGlyphScale;
	float		mLineSpacing;
	
	ZLLeanArray < MOAIAnimCurve* > mCurves;

	//----------------------------------------------------------------//
	void		ReleaseCurve				( MOAIAnimCurve* curve  );
	void		RetainCurve					( MOAIAnimCurve* curve  );

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

	GET_SET ( MOAINode*, Owner, mOwner )
	
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

	//----------------------------------------------------------------//
	void		ClearCurves					();
	void		Layout						( MOAITextLayout& layout, MOAITextStyler& styler, cc8* str, u32 idx, bool* more, u32* nextIdx );
				MOAITextDesigner			();
				~MOAITextDesigner			();
	void		ReserveCurves				( u32 total );
	void		SetCurve					( u32 idx, MOAIAnimCurve* curve );
	void		SetLoc						( float x, float y );
};

#endif
