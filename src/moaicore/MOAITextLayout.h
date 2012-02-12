// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUT_H
#define	MOAITEXTLAYOUT_H

class MOAITextStyle;

//================================================================//
// MOAITextToken
//================================================================//
class MOAITextToken {
public:

	friend class MOAITextLayout;
	friend class MOAIFont;
	friend class MOAITextStyler;

	u32		mBase;		// base index of first utf-8 character in word
	float	mX;			// x offset of word
	u16		mStyle;		// index of style for word
	u16		mFlags;		// flags (such as compound word)
};

//================================================================//
// MOAITextLayout
//================================================================//
class MOAITextLayout :
	public USLeanStack < MOAITextToken, 64 > {
	
	USLeanStack < MOAITextStyle*, 8 > mStyles;
	
	//----------------------------------------------------------------//
	u32			AffirmStyle			( MOAITextStyle& style );
	void		LoadGlyphMetrics	();
	void		SizeTokens			();
	
public:

	friend class MOAITextStyler;

	//----------------------------------------------------------------//
	
				MOAITextLayout		();
				~MOAITextLayout		();
	void		PushToken			( cc8* str, int base, int size, u32 flags, MOAITextStyle& style );
};

#endif
