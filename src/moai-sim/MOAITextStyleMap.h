// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLEMAP_H
#define	MOAITEXTSTYLEMAP_H

class MOAINode;
class MOAITextStyle;
class MOAITextStyleCache;
class MOAITextStyleParser;

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
// this is the style map. it is produced by analyzing the text and creating a
// 'style span' for each styled token. this is the preprocessing step to
// actually layout out a page of text. text is laid out based on the style spans.
// each span represents a stretch of 'styled' text
class MOAITextStyleMap :
	public ZLLeanStack < MOAITextStyleSpan, 64 > {
private:

	friend class MOAITextDesignParser;
	friend class MOAITextStyleParser;
	
	//----------------------------------------------------------------//
	void				PushStyleSpan				( int base, int top, MOAITextStyle& style );

public:
	
	//----------------------------------------------------------------//
	void				BuildStyleMap				( MOAITextStyleCache& styleCache, cc8* str );
	u32					CountSpans					();
						MOAITextStyleMap			();
						~MOAITextStyleMap			();
	void				RefreshStyleGlyphs			( cc8* str );
};

#endif
