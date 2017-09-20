// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHSET_H
#define	MOAIGLYPHSET_H

#include <moai-sim/MOAIGlyph.h>

//================================================================//
// MOAIGlyphSet
//================================================================//
class MOAIGlyphSet :
	public MOAIFontFaceMetrics {
private:

	friend class MOAIFont;
	friend class MOAITextLayoutEngine;
	
	float	mSize;
	
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

	GET_SET_CONST	( float, Size, mSize );
	GET_SET_CONST	( float, Height, mHeight );

	//----------------------------------------------------------------//
	MOAIGlyph*		GetGlyph				( u32 c );
					MOAIGlyphSet			();
					~MOAIGlyphSet			();
	void			SerializeIn				( MOAILuaState& state );
	void			SerializeOut			( MOAILuaState& state );
};

#endif
