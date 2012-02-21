// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNER_H
#define	MOAITEXTDESIGNER_H

class MOAITextBox;
class MOAITextStyleSpan;
class MOAITextStyleState;

//================================================================//
// MOAITextDesigner
//================================================================//
class MOAITextDesigner {
private:
	
	MOAITextStyleSpan*		mStyleSpan;
	MOAITextStyleState*		mStyleState;
	u32						mSpanIdx;
	int						mIdx;
	
	cc8*					mStr;
	
	MOAIGlyphDeck*			mDeck;
	
	//----------------------------------------------------------------//
	u32				NextChar				( MOAITextBox& textBox );

public:

	//----------------------------------------------------------------//
	void			BuildLayout				( MOAITextBox& textBox );
					MOAITextDesigner		();
	virtual			~MOAITextDesigner		();
};

#endif
