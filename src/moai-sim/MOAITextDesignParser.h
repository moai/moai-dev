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
	
	friend class MOAITextDesigner;
	
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
	
	MOAITextDesigner*	mDesigner;
	MOAITextLayout*		mLayout;
	MOAITextStyler*		mStyler;
	
	GET ( u32, Index, mIdx )
	GET ( u32, More, mMore )
	
	//----------------------------------------------------------------//
	void				AcceptLine				();
	void				AcceptToken				();
	void				Align					();
	void				BuildLayout				();
	void				Init					( MOAITextLayout& layout, MOAITextStyler& styler, MOAITextDesigner& designer, cc8* str, u32 idx );
	bool				More					();
	u32					NextChar				();

public:

	//----------------------------------------------------------------//
					MOAITextDesignParser	();
	virtual			~MOAITextDesignParser	();
	
};

#endif
