// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHEPAGE_H
#define	MOAIGLYPHCACHEPAGE_H

#include <moaicore/MOAISpanList.h>

class MOAIGlyph;
class MOAIImageTexture;
class MOAIFont;

//================================================================//
// MOAIGlyphCachePage
//================================================================//
class MOAIGlyphCachePage {
private:

	friend class MOAIGlyphCache;

	typedef MOAISpanList < MOAIGlyph* > GlyphList;
	typedef MOAISpanList < MOAIGlyph* >::Span GlyphSpan;
	
	typedef MOAISpanList < GlyphList > RowList;
	typedef MOAISpanList < GlyphList >::Span RowSpan;

	RowList mRows;

	MOAIImageTexture* mImageTexture;
	
	USColor::Format mColorFormat;

	float mThreshold;

public:
	
	//----------------------------------------------------------------//
	void			AffirmCanvas				( MOAIFont& font );
	GlyphSpan*		Alloc						( MOAIFont& font, MOAIGlyph& glyph );
	RowSpan*		AllocRow					( u32 height );
	void			Clear						();
	bool			ExpandToNextPowerofTwo		();
	void			InitCanvas					( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt );
					MOAIGlyphCachePage			();
					~MOAIGlyphCachePage			();
};

#endif
