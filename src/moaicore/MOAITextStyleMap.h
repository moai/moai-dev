// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLEMAP_H
#define	MOAITEXTSTYLEMAP_H

class MOAITextStyle;
class MOAITextStyleSet;

//================================================================//
// MOAITextStyleSpan
//================================================================//
class MOAITextStyleSpan {
public:

	int					mBase;		// base index of first utf-8 character in span
	int					mTop;		// size of span
	MOAITextStyle*		mStyle;		// style for span
};

//================================================================//
// MOAITextStyleMap
//================================================================//
class MOAITextStyleMap :
	public USLeanStack < MOAITextStyleSpan, 64 > {
	
	USLeanStack < MOAITextStyle*, 8 > mStyles;
	
	//----------------------------------------------------------------//
	u32			AffirmStyle			( MOAITextStyle& style );
	void		LoadGlyphMetrics	();
	void		PushSpan			( cc8* str, int base, int top, MOAITextStyle& style );
	
public:

	friend class MOAITextDesigner;
	friend class MOAITextStyler;

	//----------------------------------------------------------------//
				MOAITextStyleMap		();
				~MOAITextStyleMap		();
	void		Tokenize				( MOAITextStyleSet& styleSet, cc8* str );
};

#endif
