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
	
	int		mTokenIdx;
	int		mLineIdx;
	
	float	mWidth;
	float	mHeight;
	
	u32		mLineStart;
	u32		mLineSize;
	
	u32		mTokenStart;
	u32		mTokenSize;
	
	USRect	mLineRect;
	USRect	mTokenRect;
	
	float	mTokenAscent;
	float	mLineAscent;

	USVec2D	mPen;
	
	MOAIGlyph* mGlyph;
	MOAIGlyph* mPrevGlyph;
	
	MOAITextBox* mTextBox;
	
	//----------------------------------------------------------------//
	void			AcceptLine				();
	void			AcceptToken				();
	u32				NextChar				();

public:

	//----------------------------------------------------------------//
	void			BuildLayout				();
	void			Init					( MOAITextBox& textBox );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
