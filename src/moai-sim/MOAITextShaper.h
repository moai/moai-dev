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
	//MOAIGlyphSet*				mDeck;
	MOAIGlyph*					mGlyph;
	ZLVec2D						mScale;
};

//================================================================//
// MOAITextCharStream
//================================================================//
// parser for producing a layout
class MOAITextStyledCharStream {
public:

	//----------------------------------------------------------------//
	virtual u32						GetCharIdx						() = 0;
									MOAITextStyledCharStream		() {}
	virtual							~MOAITextStyledCharStream		() {}
	virtual MOAITextStyledChar		NextChar						() = 0;
	virtual void					SeekChar						( u32 charIdx ) = 0;
};

//================================================================//
// MOAITextLineLayout
//================================================================//
class MOAITextLineLayout {
public:

	enum {
		PUSH_OK,
		PUSH_OVERRUN,
	};

	//----------------------------------------------------------------//
	virtual u32			GetSpriteIdx			() = 0;
						MOAITextLineLayout		() {}
	virtual				~MOAITextLineLayout		() {}
	virtual u32			PushSprite				( const MOAITextStyledChar& styledChar, float x, float y ) = 0;
	virtual void		SeekSprite				( u32 spriteIdx ) = 0;
};

//================================================================//
// MOAITextShaper
//================================================================//
class MOAITextShaper {
private:
	
public:

	//----------------------------------------------------------------//
						MOAITextShaper				() {}
	virtual				~MOAITextShaper				() {}
	virtual void		ShapeLine					( MOAITextStyledCharStream& charStream, MOAITextLineLayout& lineLayout, const MOAITextLayoutRules& designer ) = 0;
};

//================================================================//
// MOAITextSimpleShaper
//================================================================//
class MOAITextSimpleShaper {
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
	void				ShapeLine					( MOAITextStyledCharStream& charStream, MOAITextLineLayout& lineLayout, const MOAITextLayoutRules& designer );
};

#endif
