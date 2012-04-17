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
	
	//----------------------------------------------------------------//
	u32				NextChar				( MOAITextBox& textBox );

public:

	//----------------------------------------------------------------//
	void			BuildLayout				( MOAITextBox& textBox );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
