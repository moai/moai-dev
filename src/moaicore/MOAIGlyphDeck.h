// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHDECK_H
#define	MOAIGLYPHDECK_H

#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIGlyphDeck
//================================================================//
class MOAIGlyphDeck {
private:

	friend class MOAIFont;
	friend class MOAIFreeTypeFontBuilder;
	friend class MOAITextDesigner;
	
	float	mPoints;
	float	mHeight;
	float	mAscent;
	
	// TODO: optimize for printable ASCII characters
	// TODO: find better container than STLMap
	STLMap < u32, MOAIGlyph > mGlyphMap;
	
	MOAIGlyph* mPending; // queue of glyphs remaining to be processed
	MOAIGlyph* mGlyphs; // processed glyphs
	
	//----------------------------------------------------------------//
	void			AffirmGlyph			( u32 c );

public:

	//----------------------------------------------------------------//
	MOAIGlyph*		GetGlyph			( u32 c );
					MOAIGlyphDeck		();
					~MOAIGlyphDeck		();
};

#endif
