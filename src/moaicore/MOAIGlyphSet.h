// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHSET_H
#define	MOAIGLYPHSET_H

#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIGlyphSet
//================================================================//
class MOAIGlyphSet {
private:

	friend class MOAIFont;
	friend class MOAITextDesigner;
	
	float	mSize;
	float	mHeight;
	float	mAscent;
	
	// TODO: optimize for printable ASCII characters
	// TODO: find better container than STLMap
	typedef STLMap < u32, MOAIGlyph >::iterator GlyphMapIt;
	STLMap < u32, MOAIGlyph > mGlyphMap;
	
	MOAIGlyph* mPending; // queue of glyphs remaining to be processed
	MOAIGlyph* mGlyphs; // processed glyphs
	
	//----------------------------------------------------------------//
	const MOAIGlyph&	AffirmGlyph			( u32 c );
	MOAIGlyph&			EditGlyph			( u32 c );

public:

	GET_SET ( float, Size, mSize );
	GET_SET ( float, Height, mHeight );
	GET_SET ( float, Ascent, mAscent );

	//----------------------------------------------------------------//
	MOAIGlyph*			GetGlyph			( u32 c );
						MOAIGlyphSet		();
						~MOAIGlyphSet		();
	void				SerializeIn			( MOAILuaState& state );
	void				SerializeOut		( MOAILuaState& state );
};

#endif
