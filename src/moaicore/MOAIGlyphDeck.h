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
	friend class MOAITextDesigner;
	
	float	mPoints;
	float	mHeight;
	float	mAscent;
	
	// for now
	STLMap < u32, MOAIGlyph > mGlyphMap;
	
	MOAIGlyph* mPending;
	
	//----------------------------------------------------------------//
	void			AffirmGlyph			( u32 c );

public:

	//----------------------------------------------------------------//
	MOAIGlyph*		GetGlyph			( u32 c );
					MOAIGlyphDeck		();
					~MOAIGlyphDeck		();
};

#endif
