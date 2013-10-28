// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNPARSER_H
#define	MOAITEXTDESIGNPARSER_H

class MOAITextStyle;
class MOAITextStyler;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesignParser
//================================================================//
class MOAITextDesignParser {
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
	float				mLineHeight;
	float				mLineAscent;
	ZLRect				mLineRect;
	
	int					mTokenIdx;
	u32					mTokenSpriteID;
	u32					mTokenSize;
	float				mTokenHeight;
	float				mTokenAscent;
	ZLRect				mTokenRect;
	
	float				mLayoutWidth;
	float				mLayoutHeight;
	
	ZLVec2D				mPen;
	MOAIGlyph*			mPrevGlyph;
	bool				mMore;
	
	//----------------------------------------------------------------//
	// layout settings
	
	MOAITextDesigner*	mDesigner;
	MOAITextLayout*		mLayout;
	MOAITextStyler*		mStyler;
	
	//----------------------------------------------------------------//
	void				AcceptLine					();
	void				AcceptToken					();
	void				Align						();
	void				BuildLayout					();
	u32					NextChar					();

public:

	GET ( u32, Index, mIdx )

	//----------------------------------------------------------------//
	void				BuildLayout					( MOAITextLayout& layout, MOAITextStyler& styler, MOAITextDesigner& designer, cc8* str, u32 idx );
						MOAITextDesignParser		();
	virtual				~MOAITextDesignParser		();
	bool				More						();
	
};

#endif
