// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDYNAMICGLYPHCACHEPAGE_H
#define	MOAIDYNAMICGLYPHCACHEPAGE_H

#include <moai-sim/MOAISpanList.h>

class MOAIDynamicGlyphCache;
class MOAIGlyph;
class MOAIImageTexture;
class MOAIFont;

//================================================================//
// MOAIDynamicGlyphCachePage
//================================================================//
// allocate space for glyphs using a two dimentional 'heap' made of span lists
// there is one span list for the rows and a span list for each row
// more generically, any kind of memory allocator could be used
// could also factor out a two-dimensional allocator as the base class
class MOAIDynamicGlyphCachePage {
private:

	friend class MOAIDynamicGlyphCache;

	typedef MOAISpanList < MOAIGlyph* > GlyphList;
	typedef MOAISpanList < MOAIGlyph* >::Span GlyphSpan;
	
	typedef MOAISpanList < GlyphList > RowList;
	typedef MOAISpanList < GlyphList >::Span RowSpan;

	RowList mRows;

	MOAIImageTexture* mImageTexture;
	
	//ZLColor::ColorFormat mColorFormat;

	float mThreshold;

public:
	
	//----------------------------------------------------------------//
	void			AffirmCanvas					( MOAIDynamicGlyphCache& owner, MOAIFont& font );
	GlyphSpan*		Alloc							( MOAIDynamicGlyphCache& owner, MOAIFont& font, MOAIGlyph& glyph );
	RowSpan*		AllocRow						( u32 height );
	void			Clear							( MOAIDynamicGlyphCache& owner );
	bool			ExpandToNextPowerofTwo			();
					MOAIDynamicGlyphCachePage		();
					~MOAIDynamicGlyphCachePage		();
};

#endif
