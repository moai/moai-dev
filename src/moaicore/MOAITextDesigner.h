// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextBox;
class MOAITextStyle;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
private:
	
	MOAITextStyleSpan*		mStyleSpan;
	MOAITextStyle*			mStyle;
	u32						mSpanIdx;
	
	int						mIdx;
	int						mPrevIdx;
	
	cc8*					mStr;
	
	MOAIGlyphSet*			mDeck;
	float					mDeckScale;
	
	float	mWidth;
	float	mHeight;
	
	int		mLineIdx;
	u32		mLineSpriteID;
	u32		mLineSize;
	float	mLineAscent;
	USRect	mLineRect;
	
	int		mTokenIdx;
	u32		mTokenSpriteID;
	u32		mTokenSize;
	float	mTokenAscent;
	USRect	mTokenRect;
	
	USVec2D	mPen;
	MOAIGlyph* mPrevGlyph;
	
	MOAITextBox* mTextBox;
	
	//----------------------------------------------------------------//
	void			AcceptLine				();
	void			AcceptToken				();
	void			Align					();
	u32				NextChar				();

public:

	//----------------------------------------------------------------//
	void			BuildLayout				();
	void			Init					( MOAITextBox& textBox );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
