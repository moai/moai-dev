// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHPAGE_H
#define	MOAIGLYPHPAGE_H

#include <moaicore/MOAISpanList.h>

class MOAIGlyph;
class MOAIImageTexture;

//================================================================//
// MOAIGlyphPage
//================================================================//
class MOAIGlyphPage {
private:

	friend class MOAIGlyph;
	friend class MOAIFont;
	friend class MOAIFreeTypeFont;

	typedef MOAISpanList < MOAIGlyph* > GlyphList;
	typedef MOAISpanList < MOAIGlyph* >::Span GlyphSpan;
	
	typedef MOAISpanList < GlyphList > RowList;
	typedef MOAISpanList < GlyphList >::Span RowSpan;

	RowList mRows;

	MOAIImageTexture* mImageTexture;
	
	MOAIGlyphPage* mNext;

	float mThreshold;

	float mUScale;
	float mVScale;

	//----------------------------------------------------------------//
	void			AffirmCanvas				();
	GlyphSpan*		Alloc						( MOAIGlyph& glyph );
	RowSpan*		AllocRow					( u32 height );
	bool			ExpandToNextPowerofTwo		();

public:
	
	//----------------------------------------------------------------//
					MOAIGlyphPage				();
					~MOAIGlyphPage				();
};

#endif
