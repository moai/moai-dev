// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSHAPER_H
#define	MOAITEXTSHAPER_H

class MOAIGlyph;
class MOAITextStyleState;

//================================================================//
// MOAITextStyledChar
//================================================================//
// parser for producing a layout
class MOAITextStyledChar {
public:

	u32							mIdx;
	u32							mChar;
	MOAITextStyleState*			mStyle;
	MOAIGlyph*					mGlyph;
	ZLVec2D						mScale;
};

//================================================================//
// MOAITextShaperClient
//================================================================//
class MOAITextShaperClient {
public:

	enum {
		PUSH_OK,
		PUSH_OVERRUN,
	};

	//----------------------------------------------------------------//
	virtual void					BeginChar						() = 0; // set the resume point for characters
	virtual void					BeginToken						() = 0; // set the resume point for tokens
									MOAITextShaperClient			() {}
	virtual							~MOAITextShaperClient			() {}
	virtual MOAITextStyledChar		NextChar						() = 0; // read the next styled char from the input stream
	virtual u32						PushSprite						( const MOAITextStyledChar& styledChar, float x, float y ) = 0; // push a visible character
};

//================================================================//
// MOAITextShaper
//================================================================//
class MOAITextShaper {
public:

	//----------------------------------------------------------------//
						MOAITextShaper				() {}
	virtual				~MOAITextShaper				() {}
	virtual u32			ShapeLine					( MOAITextShaperClient& client, const MOAITextLayoutRules& layoutRules ) = 0;
};

//================================================================//
// MOAITextSimpleShaper
//================================================================//
class MOAITextSimpleShaper :
	public MOAITextShaper {
private:

	int					mTokenIdx;
	u32					mTokenSpriteID;
	u32					mTokenSize;
	
	//----------------------------------------------------------------//
	void				AcceptToken					();
	
public:

	//----------------------------------------------------------------//
						MOAITextSimpleShaper		();
						~MOAITextSimpleShaper		();
	u32					ShapeLine					( MOAITextShaperClient& client, const MOAITextLayoutRules& layoutRules );
};

#endif
